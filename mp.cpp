//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "mp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMnemoshemaMain *FormMnemoshemaMain;

//threed in thread pool, for synchronize the UI
class TThreadHTTPDataUISynchronize : public sysThread::__TThread {
	public:
	__fastcall TThreadHTTPDataUISynchronize(sysThread::TThreadPool *threadPool) : sysThread::__TThread(threadPool) {}

	__fastcall virtual ~TThreadHTTPDataUISynchronize() {}

	//UIUpdate function is sync with main thread for update UI
	virtual void __fastcall UIUpdate() {
		TMnemoshemaDataManager &mnemoshemaDataManager = FormMnemoshemaMain->mnemoshemaDataManager;
		mnemoshemaDataManager.NotifyObservers(mnemoshemaDataManager.GetFinishedTask());
	}
};

//---------------------------------------------------------------------------
void CreateHTTPRequestPool() {
	HTTP_REQUEST_POOL = new sysPatterns::TPool<TTaskRequestHTTPData>();
}

//---------------------------------------------------------------------------
/*
void CreateFileRequestPool() {
	FILE_REQUEST_POOL = new sysPatterns::TPool<TTaskRequestFileData>();
}
*/

//---------------------------------------------------------------------------
void CreateDeviceFileRequestPool() {
	//deviceFileRequestPool = new sysPatterns::TPool<TTaskRequestDeviceFileData>();
}

//---------------------------------------------------------------------------
void CreateDeviceFileFormatRequestPool() {
	DEVICE_DATA_FILE_FORMAT_REQUEST_POOL = new sysPatterns::TPool<TTaskRequestDeviceDataFileFormat>();
}

//---------------------------------------------------------------------------
void CreateDeviceEventFileFormatRequestPool() {
	DEVICE_EVENT_DATA_FILE_FORMAT_REQUEST_POOL = new sysPatterns::TPool<TTaskRequestDeviceEventDataFileFormat>();
}

//---------------------------------------------------------------------------
void CreateMnemoshemaDataHistoryRequestPool() {
	MNEMOSHEMA_DATA_HISTORY_REQUEST_POOL = new sysPatterns::TPool<TTaskRequestMnemoshemaDataHistory>();
}

//---------------------------------------------------------------------------
/*
void CreateFileEventRequestPool() {
	//sensorEventRequestPool = new sysPatterns::TPool<TTaskRequestFileEventData>();
}
*/

//---------------------------------------------------------------------------
/*
void CreateSensorDataPool() {
	//sensorDataPool = new sysPatterns::TPool<TSensorData>();
}
*/

//---------------------------------------------------------------------------
__fastcall TFormMnemoshemaMain::TFormMnemoshemaMain(TComponent* Owner) : TForm(Owner) {

}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::RestoreLastUserSettings() {
	sysApp::RestoreFormSettings(this);

	if (WindowState == wsMinimized) {
		WindowState = wsNormal;
	}

	//restore prev col width settings
	for (int col = 0; col < stringGridEvents->ColCount; ++col) {
		String name = String(L"StringGridEventColumnWidth") + IntToStr(col);
		stringGridEvents->ColWidths[col] = sysApp::GetSettingInt(name.c_str(), 100);
	}

	PanelEvents->Width = sysApp::GetSettingDouble(L"PanelEventsWidth", 190);
	PanelMnemoshemaLeft->Width = sysApp::GetSettingDouble(L"PanelMnemoshemaLeftWidth", 185);

	//do not change the order od CheckBoxEventByDate, because check box manage for update data event table
	DateTimePickerEventDate->DateTime = sysApp::GetSettingDouble(L"DateTimePickerEventDateVal", Now());
	CheckBoxEventByDate->Checked = sysApp::GetSettingDouble(L"CheckBoxEventByDateChecked", 0) != 0;

	MemoEventDescription->Height = sysApp::GetSettingDouble(L"MemoEventDescriptionHeight", 100);
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::SaveLastUserSettings() {
	sysApp::SaveFormSettings(this);

	//save signal column width
	for (int col = 0; col < stringGridEvents->ColCount; ++col) {
		String name = String(L"StringGridEventColumnWidth") + IntToStr(col);
		String value = IntToStr(stringGridEvents->ColWidths[col]);
		sysApp::SetSetting(name.c_str(), value.c_str());
	}

	sysApp::SetSetting(L"PanelEventsWidth", (double)PanelEvents->Width);
	sysApp::SetSetting(L"PanelMnemoshemaLeftWidth", (double)PanelMnemoshemaLeft->Width);

	sysApp::SetSetting(L"DateTimePickerEventDateVal", DateTimePickerEventDate->DateTime.Val);
	sysApp::SetSetting(L"CheckBoxEventByDateChecked", (double)CheckBoxEventByDate->Checked);

	sysApp::SetSetting(L"MemoEventDescriptionHeight", (double)MemoEventDescription->Height);
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::FormCreate(TObject *Sender) {
	std::wstring fileVersion = sysFile::getExeVersion(Application->ExeName.c_str(), 1251);
	Caption = Caption + String(L" ") + String(fileVersion.c_str());

	sysLogger::PROFILE_A("begin");

	InitializeCriticalSection(&csHttpDataManagerAccess);

	sysLogger::PROFILE_A("init cs http data manager access");

	UPDATE_TREE_VIEW_OPTIONS.display_sensor_bit = 1;
	UPDATE_TREE_VIEW_OPTIONS.display_empty_nodes = 1;
	UPDATE_TREE_VIEW_OPTIONS.sFirespecGroups = "";
	UPDATE_TREE_VIEW_OPTIONS.sDefaultPrecision = "";

	BuildTreeViewDevice(TreeViewDevice);

	sysLogger::PROFILE_A("build tree view device");

	CreateThreadPool();

	sysLogger::PROFILE_A("create thread pool");

	//CreateRequestPool();
	CreateHTTPRequestPool();

	//CreateFileRequestPool();

	//CreateDeviceFileRequestPool();

	//CreateFileEventRequestPool();

	CreateDeviceFileFormatRequestPool();

	CreateDeviceEventFileFormatRequestPool();

	CreateMnemoshemaDataHistoryRequestPool();

	int dataFileFormat = DataModuleMP->GetSettingInt("data_file_format");
	if (dataFileFormat == DATA_FILE_FORMAT_DEVICE) {
		REQUEST_TIME_RANGE_DATA = &RequestTimeRangeDataFileFormatDevice;
	} else {
		sysLogger::ERR_A("unsupport data file format");
		assert(dataFileFormat != DATA_FILE_FORMAT_DEVICE);
	}

	REQUEST_MNEMOSHEMA_DATA = &RequestMnemoshemaData;

	/*
	if (dataFileFormat == DATA_FILE_FORMAT_SENSOR) {
		REQUEST_TIME_RANGE_DATA = &RequestTimeRangeDataFileFormatSensor;

	} else if (dataFileFormat == DATA_FILE_FORMAT_DEVICE) {
		REQUEST_TIME_RANGE_DATA = &RequestTimeRangeDataFileFormatDevice;
	}
	*/

	sysLogger::PROFILE_A("create request pools");

	//CreateSensorDataPool();

	//sysLogger::PROFILE_A("create sensor data pool");

	CreateStringGridEvents();

	RestoreLastUserSettings();

	sysLogger::PROFILE_A("restore last user settings");

	InitUpdateHTTPTimer();

	sysLogger::PROFILE_A("initialize http timer");

	InitUpdateEventsData();

	sysLogger::PROFILE_A("initialize event update timer");

	InitChartFilters();
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::InitChartFilters() {
	GAUSSIAN_FILTER_ENABLED = true;
	MEDIAN_FILTER_ENABLED = true;
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::InitUpdateHTTPTimer() {
	String sTimerUpdateDataInterval = DataModuleMP->GetSetting("TimerUpdateDataInterval");
	if (sTimerUpdateDataInterval.IsEmpty() == true) {
		sTimerUpdateDataInterval = "1000";
	}
	TimerUpdateHTTPData->Interval = StrToIntDef(sTimerUpdateDataInterval, 1000);
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::InitUpdateEventsData() {
	String sTimerUpdateActiveAlarmsInterval = DataModuleMP->GetSetting("TimerUpdateActiveAlarmsInterval");
	if (sTimerUpdateActiveAlarmsInterval.IsEmpty() == true) {
		sTimerUpdateActiveAlarmsInterval = "10000";
	}
	TimerUpdateEventsData->Interval = StrToIntDef(sTimerUpdateActiveAlarmsInterval, 10000);
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::CreateStringGridEvents() {
	TDatasourceVector<TDatasourceObjectEvent> *datasource = new TDatasourceVector<TDatasourceObjectEvent>();

	stringGridEvents = new TStringGridDatasource<TDatasourceObjectEvent>(PanelEvents, datasource);
	stringGridEvents->Align = alClient;
	stringGridEvents->Parent = PanelEvents;
	stringGridEvents->FixedRows = 1;
	stringGridEvents->FixedCols = 0;
	stringGridEvents->ColCount = 3;
	stringGridEvents->RowCount = 2;
	stringGridEvents->Options << goColSizing;

	stringGridEvents->PopupMenu = PopupMenuStringGridEvent;
	stringGridEvents->OnClick = StringGridEventClick;

	stringGridEvents->Cells[0][0] = "Дата/Время";
	stringGridEvents->Cells[1][0] = "Датчик";
	stringGridEvents->Cells[2][0] = "Событие";

	stringGridEventsObserver.SetStringGrid(stringGridEvents);
}

//---------------------------------------------------------------------------
//thread pool callback function with finished task as a parameter
static void callback(sysThread::TThreadTask *task) {
	EnterCriticalSection(&FormMnemoshemaMain->csHttpDataManagerAccess);

	TTaskRequestMnemoshemaData *taskRequestData = static_cast<TTaskRequestMnemoshemaData *>(task);

	//require the thread correspond with this task
	TThreadHTTPDataUISynchronize *thread = (TThreadHTTPDataUISynchronize *)(taskRequestData->thread);

	//FormMnemoshemaMain->mnemoshemaDataManager.finishedTask = taskRequestData;
	FormMnemoshemaMain->mnemoshemaDataManager.SetFinishedTask(taskRequestData);

	//synchronize the UI main thread with current thread to udpate UI
	System::Classes::TThread::Synchronize(thread, (TThreadMethod)&thread->UIUpdate);

	//requestPool->Release(taskRequestData);
	//taskRequestData->GetPool()->Release(taskRequestData);

	int observerableType = taskRequestData->GetObserverableType();

	if (observerableType == TObserverableTypes::DEVICE_HTTP_DATA) {
		HTTP_REQUEST_POOL->Release(static_cast<TTaskRequestHTTPData *>(taskRequestData));

	} else if (observerableType == TObserverableTypes::SENSOR_FILE_DATA) {
		//FILE_REQUEST_POOL->Release(static_cast<TTaskRequestFileData *>(taskRequestData));

	} else if (observerableType == TObserverableTypes::DEVICE_FILE_DATA) {

	} else if (observerableType == TObserverableTypes::SENSOR_FILE_EVENT_DATA) {
		//sensorEventRequestPool->Release(static_cast<TTaskRequestFileEventData *>(taskRequestData));

	} else if (observerableType == TObserverableTypes::DEVICE_FILE_DATA_FORMAT) {
		DEVICE_DATA_FILE_FORMAT_REQUEST_POOL->Release(static_cast<TTaskRequestDeviceDataFileFormat *>(taskRequestData));

	} else if (observerableType == TObserverableTypes::DEVICE_FILE_EVENT_DATA_FORMAT) {
		DEVICE_EVENT_DATA_FILE_FORMAT_REQUEST_POOL->Release(static_cast<TTaskRequestDeviceEventDataFileFormat *>(taskRequestData));

	} else if (observerableType == TObserverableTypes::MNEMOSHEMA_DATA_HISTORY) {
		MNEMOSHEMA_DATA_HISTORY_REQUEST_POOL->Release(static_cast<TTaskRequestMnemoshemaDataHistory *>(taskRequestData));

	} else {
		sysLogger::ERR_A("task request data unknown observerable type:");
		sysLogger::ERR_W(IntToStr(taskRequestData->GetObserverableType()).c_str());
		assert(FALSE);
    }

	LeaveCriticalSection(&FormMnemoshemaMain->csHttpDataManagerAccess);
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::CreateThreadPool() {
	DWORD cores = sysSystem::GetCPUCores();

	sysLogger::DEBUG_A("thread pool created depend by CPU cores: ");
	sysLogger::DEBUG_W(IntToStr((int)cores).c_str());

	THREAD_POOL = new sysThread::TThreadPoolBorland<TThreadHTTPDataUISynchronize>(cores, callback);
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::TimerUpdateHTTPDataTimer(TObject *Sender) {
	if (FormDashboard->CheckBoxDataHistory->Checked == true) {
		//offline mnemoshema data display
		return;
	}

	for (std::list<TDevice *>::iterator it = DEVICES.begin(), itEnd = DEVICES.end(); it != itEnd; ++it) {
		std::list<TTaskRequestHTTPData *>::iterator iTask = HTTP_REQUEST_POOL->Get<TTaskRequestHTTPData>();

		TTaskRequestHTTPData *task = static_cast<TTaskRequestHTTPData *>(*iTask);
		task->device = *it;

		if (THREAD_POOL->Contain(task)) {
			HTTP_REQUEST_POOL->Release(iTask);
		} else {
			THREAD_POOL->Push(task);
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::FormDestroy(TObject *Sender) {
	DeleteCriticalSection(&csHttpDataManagerAccess);

    SaveLastUserSettings();

	sysLogger::SaveProfile(L"profile.txt");
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::BuildChart(const TRecord *record) {
	if (record->record_type == RECORD_TYPE_SENSOR ||
		record->record_type == RECORD_TYPE_SENSOR_BIT) {

		FormChart->AddChart(record);

		FormChart->RequestData();

	} else if (record->record_type == RECORD_TYPE_DEVICE) {
		FormChart->AddCharts(record);

		FormChart->RequestData();
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::PMBuildChartClick(TObject *Sender) {
	//activate chart window...
	FormChart->WindowState = wsNormal;

	TTreeView *treeViewDevice = dynamic_cast<TTreeView *>(PopupMenuTreeViewDevice->PopupComponent);
	TTreeNode *node = treeViewDevice->Selected;
	if (node == NULL){
		return;
	}

	TRecord *record = static_cast<TRecord *>(node->Data);
	if (record == NULL){
		return;
	}

	//iterate over all tree childs and build chart for each record
	if (record->record_type == RECORD_TYPE_DEVICE) {
		TTreeNode *child = node->getFirstChild();
		TTreeNode *lastChild = node->GetLastChild();

		TRecordViewChart *firstRecordViewChart = NULL;

		for (;child != lastChild; child = child->GetNext()) {
			if (child->Data == NULL) {
				continue;
			}

			TRecord *record = static_cast<TRecord *>(child->Data);
			if (record == NULL) {
				continue;
			}

			TRecordViewChart *recordViewChart = NULL;
			if (record->record_type == RECORD_TYPE_SENSOR) {
				recordViewChart = FormChart->AddChart(record);

			} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
				recordViewChart = FormChart->AddChart(record);
			}

			if (firstRecordViewChart == NULL) {
				firstRecordViewChart = recordViewChart;
			}
		}

		if (firstRecordViewChart != NULL) {
			firstRecordViewChart->Highlight();
		}

		FormChart->RequestData();
	} else if (record->record_type == RECORD_TYPE_SENSOR ||
			   record->record_type == RECORD_TYPE_SENSOR_BIT) {

		BuildChart(record);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::MMCloseClick(TObject *Sender) {
	Close();
}

//---------------------------------------------------------------------------
void GroupSensors(std::set<const TSensor *> &sensors, std::map<TDevice *, std::list<const TSensor *> *> &sensorGroup) {
	for (std::set<const TSensor *>::iterator i = sensors.begin(), iEnd = sensors.end(); i != iEnd; ++i) {
		const TSensor *sensor = *i;
		TDevice *device = GetDevice(sensor->device_id);
		if (device == NULL) {
			continue;
		}

		if (sensorGroup.find(device) == sensorGroup.end()) {
			sensorGroup[device] = new std::list<const TSensor *>;
		}
		sensorGroup[device]->push_back(sensor);
	}
}

//---------------------------------------------------------------------------
void TFormMnemoshemaMain::RequestMnemoshemaData(std::set<const TSensor *> &sensors, double dtGMT) {
	std::map<TDevice *, std::list<const TSensor *> *> sensorGroup;
	GroupSensors(sensors, sensorGroup);

	//iterate over device and create request task with list of device require sensor
	for (std::map<TDevice *, std::list<const TSensor *> *>::iterator i = sensorGroup.begin(), iEnd = sensorGroup.end(); i != iEnd; ++i) {
		const TDevice *device = i->first;
		std::list<const TSensor *> *deviceSensors = i->second;

		std::list<TTaskRequestMnemoshemaDataHistory *>::iterator iTask = MNEMOSHEMA_DATA_HISTORY_REQUEST_POOL->Get<TTaskRequestMnemoshemaDataHistory>();
		TTaskRequestMnemoshemaDataHistory *task = static_cast<TTaskRequestMnemoshemaDataHistory *>(*iTask);
		task->device = device;
		task->SetSensors(*deviceSensors);
		task->dt1GMT = dtGMT;
		task->dt2GMT = dtGMT;
		task->resolution = 0;

		if (THREAD_POOL->Contain(task)) {
			MNEMOSHEMA_DATA_HISTORY_REQUEST_POOL->Release(iTask);
		} else {
			THREAD_POOL->Push(task);
        }
	}

	//release resource
	for (std::map<TDevice *, std::list<const TSensor *> *>::iterator i = sensorGroup.begin(), iEnd = sensorGroup.end(); i != iEnd; ++i) {
		std::list<const TSensor *> *deviceSensors = i->second;
		delete deviceSensors;
	}
}

//---------------------------------------------------------------------------
void TFormMnemoshemaMain::RequestTimeRangeDataFileFormatDevice(std::set<const TSensor *> &sensors, double dt1GMT, double dt2GMT, double resolution) {
	//group by device...
	std::map<TDevice *, std::list<const TSensor *> *> sensorGroup;
	GroupSensors(sensors, sensorGroup);

	//iterate over device and create request task with list of device require sensor
	for (std::map<TDevice *, std::list<const TSensor *> *>::iterator i = sensorGroup.begin(), iEnd = sensorGroup.end(); i != iEnd; ++i) {
		const TDevice *device = i->first;
		std::list<const TSensor *> *deviceSensors = i->second;

		std::list<TTaskRequestDeviceDataFileFormat *>::iterator iTask = DEVICE_DATA_FILE_FORMAT_REQUEST_POOL->Get<TTaskRequestDeviceDataFileFormat>();

		TTaskRequestDeviceDataFileFormat *task = static_cast<TTaskRequestDeviceDataFileFormat *>(*iTask);
		task->device = device;
		task->SetSensors(*deviceSensors);
		task->dt1GMT.Val = dt1GMT;
		task->dt2GMT.Val = dt2GMT;
		task->resolution = resolution;

		if (THREAD_POOL->Contain(task)) {
			DEVICE_DATA_FILE_FORMAT_REQUEST_POOL->Release(iTask);
		} else {
			THREAD_POOL->Push(task);
		}
	}

	//release resource
	for (std::map<TDevice *, std::list<const TSensor *> *>::iterator i = sensorGroup.begin(), iEnd = sensorGroup.end(); i != iEnd; ++i) {
		std::list<const TSensor *> *deviceSensors = i->second;
		delete deviceSensors;
	}
}

//---------------------------------------------------------------------------
/*
void TFormMnemoshemaMain::RequestTimeRangeDataFileFormatSensor(std::set<const TSensor *> &sensors, double dt1GMT, double dt2GMT, double resolution) {
	const TSensor *sensor = *sensors.begin();
	std::list<TTaskRequestFileData *>::iterator iTask = FILE_REQUEST_POOL->Get<TTaskRequestFileData>();

	TTaskRequestFileData *task = static_cast<TTaskRequestFileData *>(*iTask);
	task->sensor = sensor;
	task->dt1GMT.Val = dt1GMT;
	task->dt2GMT.Val = dt2GMT;

	if (THREAD_POOL->Contain(task)) {
		FILE_REQUEST_POOL->Release(iTask);
	} else {
		THREAD_POOL->Push(task);
	}
}
*/

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::ApplicationRun() {
	FormDashboard->SetMnemoshemaDataManager(mnemoshemaDataManager);
	FormChart->SetMnemoshemaDataManager(mnemoshemaDataManager);
	FormSignals->SetMnemoshemaDataManager(mnemoshemaDataManager);

	InitializeDashboardRecordViewPopupMenu();

	mnemoshemaDataManager.AddObserver(static_cast<sysObserver::IObserver *>(&stringGridEventsObserver));
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::N2Click(TObject *Sender) {
	FormDebug->Show();
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::TreeViewDeviceChange(TObject *Sender, TTreeNode *Node) {
	FormSignals->OnTreeViewNodeChange(Node);
	FormRecordInfo->OnTreeViewNodeChange(Node);
	FormDashboard->OnTreeViewNodeChange(Node);
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::N8Click(TObject *Sender) {
	if (FormRecordInfo->WindowState == wsNormal) {
		FormRecordInfo->WindowState = wsMinimized;
	} else {
		FormRecordInfo->WindowState = wsNormal;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::N9Click(TObject *Sender) {
	/*
	if (FormDashboard->WindowState == wsNormal) {
		FormDashboard->WindowState = wsMinimized;
	}

	FormDashboard->WindowState = wsNormal;
	*/

	//minimaze all other windows insead of the dashboard
	FormSignals->WindowState = wsMinimized;
	FormChart->WindowState = wsMinimized;
	FormRecordInfo->WindowState = wsMinimized;
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::N10Click(TObject *Sender) {
	if (FormChart->WindowState == wsNormal) {
		FormChart->WindowState = wsMinimized;
	} else {
		FormChart->WindowState = wsNormal;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::N11Click(TObject *Sender) {
	if (FormSignals->WindowState == wsNormal) {
		FormSignals->WindowState = wsMinimized;
	} else {
		FormSignals->WindowState = wsNormal;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::RequestEventsByDate(double dtLocalTime) {
	static double fLocalTimeBiasInDays = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;
	double dt1GMT = fLocalTimeBiasInDays + floor(dtLocalTime);
	double dt2GMT = dt1GMT + 1;

	for (std::list<TDevice *>::iterator it = DEVICES.begin(), itEnd = DEVICES.end(); it != itEnd; ++it) {
		std::list<TTaskRequestDeviceEventDataFileFormat *>::iterator iTask = DEVICE_EVENT_DATA_FILE_FORMAT_REQUEST_POOL->Get<TTaskRequestDeviceEventDataFileFormat>();
		(*iTask)->device = *it;

		if (THREAD_POOL->Contain(*iTask)) {
			DEVICE_EVENT_DATA_FILE_FORMAT_REQUEST_POOL->Release(iTask);
		} else {
			(*iTask)->dt1GMT = dt1GMT;
			(*iTask)->dt2GMT = dt2GMT;
			THREAD_POOL->Push(*iTask);
        }
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::TimerUpdateEventsDataTimer(TObject *Sender) {
	if (CheckBoxEventByDate->Checked == true) {
		return;
	}

	RequestEventsByDate(Now().Val);
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::UpdateStringGridEvents(TTaskRequestDeviceEventDataFileFormat *request) {
	std::map<const TSensor *, std::list<TSensorEventData *> *> &data = request->data;

}

//---------------------------------------------------------------------------
void TFormMnemoshemaMain::UpdateData(sysObserverable::IObserverable *data) {
	if (data->GetObserverableType() == DEVICE_FILE_EVENT_DATA_FORMAT) {
    	UpdateStringGridEvents(static_cast<TTaskRequestDeviceEventDataFileFormat *>(data));
	}

	/*
	if (data == NULL) {
		return;
	}

	if (data->GetObserverableType() != TObserverableTypes::SENSOR_FILE_EVENT_DATA) {
		return;
	}

	TTaskRequestFileEventData *task = static_cast<TTaskRequestFileEventData *>(data);
	for (std::map<const TSensor *, std::list<TSensorEventData *> *>::iterator i = task->data.begin(),
		iEnd = task->data.end();
		i != iEnd; ++i) {


	}
	*/
}
void __fastcall TFormMnemoshemaMain::Updateblob1Click(TObject *Sender) {
	sysLogger::PROFILE_A("...");
/*
	TByteDynArray bytes;
	try {
		DataModuleMP->ADOQuery->SQL->Text = "SELECT `data` FROM `data` WHERE sensor_id = 1";
		DataModuleMP->ADOQuery->Open();
		int recordCount = DataModuleMP->ADOQuery->RecordCount;

		bytes = DataModuleMP->ADOQuery->Fields->Fields[0]->AsBytes;
	} catch (Exception &e) {
		sysLogger::ERR_W(e.ToString().c_str());
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
	}

	//DataModuleMP->ADOQuery->Close();

	sysLogger::PROFILE_A("bytes count selected from server:");
	sysLogger::TRACE_W(IntToStr(bytes.Length).c_str());


	TByteDynArray bytesToWrite;
	bytesToWrite.Length = 10 * 1024 * 1024;
	for (int i = 0; i < bytesToWrite.Length; i++) {
		bytesToWrite[i] = 1;
	}

	char *b = sysFile::ByteBufferToString(bytesToWrite);
*/
	try {
		DataModuleMP->ADOQuery->SQL->Text = "UPDATE data SET data = CONCAT(CAST(data AS BLOB), CAST(0xFF AS BLOB))";
		DataModuleMP->ADOQuery->ExecSQL();
	} catch (Exception &e) {
		sysLogger::ERR_W(e.ToString().c_str());
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
	}
	//DataModuleMP->ADOQuery->Close();

//	delete []b;

	sysLogger::PROFILE_A("inserting 1 byte");

	sysLogger::SaveProfile(L"profile.txt");
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::PMBuildChartForRecordViewClick(TObject *Sender) {
	//activate chart window...
	FormChart->WindowState = wsNormal;

	String recordUUID = PopupMenuRecordView->PopupComponent->Name;
	TRecord *record = GetRecord(recordUUID);
	if (record == NULL) {
		return;
	}

	BuildChart(record);
	/*
	std::map<const TSensor *, std::list<IMnemoshemaView *> *> &mshViews = FormDashboard->mshViews;
	for (std::map<const TSensor *, std::list<IMnemoshemaView *> *>::iterator i = mshViews.begin(), iEnd = mshViews.end(); i != iEnd; ++i) {
		const TRecord *record = i->first;
		if (record->uuid == recordUUID) {
			BuildChart(record);
			break;
		}
	}
	*/
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::InitializeDashboardRecordViewPopupMenu() {
	std::map<const TSensor *, std::list<IMnemoshemaView *> *> &mshViews = FormDashboard->mshViews;
	for (std::map<const TSensor *, std::list<IMnemoshemaView *> *>::iterator i = mshViews.begin(), iEnd = mshViews.end(); i != iEnd; ++i) {
		std::list<IMnemoshemaView *> *controls = i->second;
		for (std::list<IMnemoshemaView *>::iterator iControl = controls->begin(), iControlEnd = controls->end(); iControl != iControlEnd; ++iControl) {
			TRecordView *view = dynamic_cast<TRecordView *>(*iControl);
			if (view != NULL) {
				view->SetPopupMenu(PopupMenuRecordView);
			}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::EditSearchChange(TObject *Sender) {
	FilterTree(TreeViewDevice, TreeViewDeviceFiltered, EditSearch->Text);
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::ButtonClearFilterTextClick(TObject *Sender) {
	EditSearch->Text = "";
}

//---------------------------------------------------------------------------
bool __fastcall TFormMnemoshemaMain::RegisterSkyReportLibrary(const String &sOfficeFile) {
	wchar_t *wcExeFolder = sysFile::GetExecutableFolder();

    try {
    	try {
            //save SiamsReportBuilder.oxt from resources
            TResourceStream *resourceStream = new TResourceStream((int)HInstance, String("ID_SIAMS_REPORT_BUILDER"), RT_RCDATA);

            String siamsReportBuilderFile = String(wcExeFolder) + String("\\SiamsReportBuilder.oxt");

            resourceStream->SaveToFile(siamsReportBuilderFile);

            if (!sysFile::IsFileExist(siamsReportBuilderFile.c_str())) {
				String log = "File '" + siamsReportBuilderFile + "' not found";
				sysLogger::ERR_W(log.c_str());
				return false;
            }

            delete resourceStream;

            //save SiamsReportLib.oxt from resources
            resourceStream = new TResourceStream((int)HInstance, String("ID_SIAMS_REPORT_LIB"), RT_RCDATA);

            String siamsReportLibFile = String(wcExeFolder) + String("\\SiamsReportLib.oxt");

            resourceStream->SaveToFile(siamsReportLibFile);

            if (!sysFile::IsFileExist(siamsReportLibFile.c_str())) {
				String log = "File '" + siamsReportLibFile + "' not found";
				sysLogger::ERR_W(log.c_str());
				return false;
            }

            delete resourceStream;

            String sOfficePath = ExtractFilePath(sOfficeFile);
            String sUnopkgFile = sOfficePath + String(L"unopkg.exe");

            TStringBuilder *parameters = new TStringBuilder();

            parameters->Clear();
            parameters->Append("\"")->Append(sUnopkgFile)->Append("\" remove SiamsReportBuilder.oxt");

            bool isRegistrationComplete = true;

            if (!sysProcess::ExecuteCmd(NULL, parameters->ToString().c_str(), 5000)) {
                char log[256];
				sprintf(log, "Can't execute unopkg.exe remove SiamsReportBuilder.oxt, GetLastError = %d", GetLastError());
				sysLogger::ERR_A(log);
				isRegistrationComplete = false;
            }

            parameters->Clear();
            parameters->Append("\"")->Append(sUnopkgFile)->Append("\" remove SiamsReportLib.oxt");

            if (!sysProcess::ExecuteCmd(NULL, parameters->ToString().c_str(), 5000)) {
                char log[256];
				sprintf(log, "Can't execute unopkg.exe remove SiamsReportLib.oxt, GetLastError = %d", GetLastError());
				sysLogger::ERR_A(log);
                isRegistrationComplete = false;
            }

            parameters->Clear();
            parameters->Append("\"")->Append(sUnopkgFile)->Append("\"")
                ->Append(" add -f \"")->Append(siamsReportBuilderFile)->Append("\"");

            if (!sysProcess::ExecuteCmd(NULL, parameters->ToString().c_str(), 5000)) {
                char log[256];
				sprintf(log, "Can't execute unopkg.exe add -f SiamsReportBuilder.oxt, GetLastError = %d", GetLastError());
				sysLogger::ERR_A(log);
                isRegistrationComplete = false;
            }

            parameters->Clear();
            parameters->Append("\"")->Append(sUnopkgFile)->Append("\"")
                ->Append(" add -f \"")->Append(siamsReportLibFile)->Append("\"");

            if (!sysProcess::ExecuteCmd(NULL, parameters->ToString().c_str(), 5000)) {
                char log[256];
				sprintf(log, "Can't execute unopkg.exe add -f SiamsReportLib.oxt, GetLastError = %d", GetLastError());
				sysLogger::ERR_A(log);
                isRegistrationComplete = false;
            }

            return isRegistrationComplete;
		} catch (Exception &e) {
			sysLogger::ERR_W(e.ToString().c_str());
        }
    } __finally {
	    delete []wcExeFolder;
	}

	return false;
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::MMOpenOfficeClick(TObject *Sender) {
	String sOfficeFile;
	if (sysOO::IsOpenOfficeInstalled("4.1.1", sOfficeFile)) {

		if (!RegisterSkyReportLibrary(sOfficeFile)) {
        	ShowMessage("Необходимые компоненты Open Office не зарегестрированны");
        } else {
            String msg = String("Программное обеспечение Open Office 4.1.1 установленно.\nНеобходимые компоненты зарегестрированны ") + sOfficeFile;
            ShowMessage(msg);
        }
	} else {
        ShowMessage("Не установленно программное обеспечение Open Office 4.1.1.");
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::MMReportTemplateManagerClick(TObject *Sender) {
	if (FormReports->WindowState == wsNormal) {
		FormReports->WindowState = wsMinimized;
	} else {
		FormReports->WindowState = wsNormal;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::PMOpenEventOnChartClick(TObject *Sender) {
	int row = stringGridEvents->Row - 1;
	TDatasource<TDatasourceObjectEvent> *datasource = stringGridEvents->datasource;
	if (0 <= row && row < datasource->Size()) {
		TDatasourceObjectEvent oEvent = (*stringGridEvents->datasource)[row];

		//add chart to the chart form
		//specify vertical line with event time

		TRecordViewChart *chart = FormChart->AddChart(oEvent.sensor);
		if (chart == NULL) {
			//chart can't be added
			return;
		}

		String caption;
		caption = caption.sprintf(L"Выход за пределы уставки\n%s\n%s %s",
			oEvent.datetime.c_str(),
			oEvent.sensorName.c_str(),
			oEvent.event.c_str());

		TChartVerticalLine *verticalLine = new TChartVerticalLine(chart, oEvent.sensor);
		verticalLine->SetCaption(caption);
		verticalLine->SetLocalTime(oEvent.dtLocalTime);

		if (chart->IsExistTrackableElement(verticalLine) == true) {
			delete verticalLine;
		} else {
            chart->AddTrackableElement(verticalLine);
        }

		//request chart data near event date time
		double timeRange = 15 * sysTime::MIN2DAY;
		FormChart->Date1->DateTime = oEvent.dtLocalTime - timeRange;
		FormChart->Time1->DateTime = oEvent.dtLocalTime - timeRange;
		FormChart->Date2->DateTime = oEvent.dtLocalTime + timeRange;
		FormChart->Time2->DateTime = oEvent.dtLocalTime + timeRange;

		FormChart->RadioButtonOffline->Checked = true;

		FormChart->RequestData();

		//activate chart window...
		FormChart->WindowState = wsNormal;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::DateTimePickerEventDateChange(TObject *Sender) {
	if (CheckBoxEventByDate->Checked == false) {
		return;
	}

	RequestEventsByDate(DateTimePickerEventDate->DateTime.Val);
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::CheckBoxEventByDateClick(TObject *Sender) {
	if (CheckBoxEventByDate->Checked == true) {
		RequestEventsByDate(DateTimePickerEventDate->DateTime.Val);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::PMOpenEventFolderClick(TObject *Sender) {
	static double fLocalTimeBiasInDays = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;

	int row = stringGridEvents->Row - 1;
	TDatasource<TDatasourceObjectEvent> *datasource = stringGridEvents->datasource;
	if (row < 0) {
		return;
	}

	if (row >= datasource->Size()) {
		return;
	}

	TDatasourceObjectEvent oEvent = (*stringGridEvents->datasource)[row];

	if (oEvent.jsonExternalData.IsEmpty()) {
		return;
	}

	if (oEvent.device == NULL) {
		return;
	}

	String eventFile = "";

	try {
		TJSONObject *jsonData = (TJSONObject *)TJSONObject::ParseJSONValue(oEvent.jsonExternalData);
		String file1 = jsonData->GetValue(L"file1")->Value();
		String file2 = jsonData->GetValue(L"file2")->Value();

		TDateTime dtGMT;
		dtGMT.Val = fLocalTimeBiasInDays + oEvent.dtLocalTime;
		String date = dtGMT.FormatString("yyyymmdd");

		//event stores relative path to the event data file
		String centraDBEventFile = DataModuleMP->sPathToCentralDB + "\\" + oEvent.device->uuid + "\\" + date + "\\" + file1;
		if (sysFile::IsFileExist(centraDBEventFile.c_str())) {
			eventFile = centraDBEventFile;

		} else if (DataModuleMP->isArchiveDBExist == true) {
			String archiveDBEventFile = DataModuleMP->sPathToArchiveDB + "\\" + oEvent.device->uuid + "\\" + date + "\\" + file1;
			if (sysFile::IsFileExist(archiveDBEventFile.c_str())) {
				eventFile = archiveDBEventFile;
			}
		}

		if (eventFile.IsEmpty() == false) {
			sysFile::ShowFileInExplorer(eventFile.c_str());
		}
	} catch (Exception &e) {
		sysLogger::ERR_W(e.Message.c_str());
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::PMCreateEventReportClick(TObject *Sender) {
	if (FormReportList->ShowModal() == mrCancel) {
		return;
	}

	int id = FormReportList->GetReportId();
	if (id == -1) {
		return;
	}

	//save report settings xml on dist from resource
	wchar_t *wcExeFolder = sysFile::GetExecutableFolder();
	String exeFolder(wcExeFolder);
	delete []wcExeFolder;

	String reportFolder = exeFolder + "\\report";
	if (sysFile::IsFolderExist(reportFolder.c_str()) == false) {
		sysFile::CreateFolder(reportFolder.c_str());
	}

	if (sysFile::IsFolderExist(reportFolder.c_str()) == false) {
		sysLogger::ERR_A("Can't create folder:");
		sysLogger::ERR_W(reportFolder.c_str());
		return;
    }

	//save report template on disk
	String reportTemplate = reportFolder + "\\template.odt";
	FormReports->SaveReportToFile(id, reportTemplate.c_str());

	TResourceStream *resourceStream = new TResourceStream((int)HInstance, String("ID_REPORT_SETTINGS_XML"), RT_RCDATA);
	if (resourceStream == NULL) {
		sysLogger::ERR_A("ID_REPORT_SETTINGS_XML not found in resource stream");
		return;
	}

	String reportSettingsFile = reportFolder + String("\\settings.xml");
	resourceStream->SaveToFile(reportSettingsFile);
	delete resourceStream;

	if (!sysFile::IsFileExist(reportSettingsFile.c_str())) {
		sysLogger::ERR_A("report settings file 'settings.xml' not found");
		return;
	}

	std::string settings = sysFile::ReadFile(reportSettingsFile.c_str());
	if (settings.empty() == true) {
		sysLogger::ERR_A("report settings.xml is empty");
		return;
	}

	wchar_t *buffer;
	int bufferSize;
	if (sysStr::UTF8ToWideChar(settings.c_str(), settings.size(), buffer, bufferSize) == false) {
		sysLogger::ERR_A("can't convert settings.xml UTF8 into wide char string sequence");
		return;
	}

	std::wstring wSettings(buffer);
	delete []buffer;

	String dataFile = reportFolder + "\\data.xml";
	String reportOutputFile = reportFolder + "\\output.odt";

	sysStr::Replace(wSettings, L"{$template}", reportTemplate.c_str());
	sysStr::Replace(wSettings, L"{$data}", dataFile.c_str());
	sysStr::Replace(wSettings, L"{$odt}", reportOutputFile.c_str());

	sysFile::WriteToFileUTF8(reportSettingsFile.c_str(), wSettings.c_str());

	//prepare data.xml in UTF8 encoding

	String date = DateTimePickerEventDate->DateTime.FormatString("YYYY.MM.DD");

	TStringBuilder *dataXml = new TStringBuilder();
	dataXml->Append(String(L"<datasources>"));
		//${date}
		dataXml->Append(String(L"<replace>"));
			dataXml->Append(String(L"<text find=\"${date}\">"));
			dataXml->Append(date);
			dataXml->Append("</text>");
			dataXml->Append(String(L"<text find=\"\\$\\{.*\\}\" type=\"regexp\"></text>"));
		dataXml->Append(String(L"</replace>"));

/*
	<replace>
		<text find="[[key1]]">Value</text>
		<text find="${TableCaption}">Распределение пор по размерам</text>
		<text find="\$\{.*\}" type="regexp"></text>
	</replace>
*/

		dataXml->Append(String(L"<tables><table id=\"tableEvents\" title=\"Таблица событий\">"));
			dataXml->Append(String("<columns>"));
				dataXml->Append(String("<column id=\"sensor\" type=\"string\"/>"));
				dataXml->Append(String("<column id=\"date\" type=\"string\"/>"));
				dataXml->Append(String("<column id=\"event\" type=\"string\"/>"));
			dataXml->Append(String("</columns>"));
			for (int i = 0; i < stringGridEvents->datasource->Size(); ++i) {
				TDatasourceObjectEvent oEvent = (*stringGridEvents->datasource)[i];
				dataXml->Append(String(L"<tr>"));
					dataXml->Append(String(L"<td>"));
                    dataXml->Append(oEvent.sensorName);
					dataXml->Append(String(L"</td>"));

					dataXml->Append(String(L"<td>"));
					dataXml->Append(oEvent.datetime);
					dataXml->Append(String(L"</td>"));

					dataXml->Append(String(L"<td>"));
					dataXml->Append(oEvent.event);
					dataXml->Append(String(L"</td>"));
				dataXml->Append(String(L"</tr>"));
			}

		dataXml->Append(String(L"</table></tables>"));
	dataXml->Append(String(L"</datasources>"));

	String textDataXml = dataXml->ToString();
	delete dataXml;

	sysFile::WriteToFileUTF8(dataFile.c_str(), textDataXml.c_str());

	String sOfficeFile;
    if (!sysOO::IsOpenOfficeInstalled(L"4.1.1", sOfficeFile)) {
    	ShowMessage("Ну установленно программное обеспечение Open Office 4.1.1");
        return;
	}

	sysOO::BuildReport(sOfficeFile, reportSettingsFile);

	if (MessageBox(NULL, L"Открыть папку с отчетом?", L"Открыть папку", MB_YESNO) == ID_YES) {
		sysFile::ShowFileInExplorer(reportOutputFile.c_str());
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::StringGridEventClick(TObject *Sender) {
	static double fLocalTimeBiasInDays = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;

	int row = stringGridEvents->Row - 1;
	TDatasource<TDatasourceObjectEvent> *datasource = stringGridEvents->datasource;
	if (row < 0) {
		return;
	}

	if (row >= datasource->Size()) {
		return;
	}

	TDatasourceObjectEvent oEvent = (*stringGridEvents->datasource)[row];

	if (oEvent.jsonExternalData.IsEmpty()) {
		return;
	}

	if (oEvent.device == NULL) {
		return;
	}

	String textEventFile = "";

	try {
		TJSONObject *jsonData = (TJSONObject *)TJSONObject::ParseJSONValue(oEvent.jsonExternalData);
		String file1 = jsonData->GetValue(L"file1")->Value();
		String file2 = jsonData->GetValue(L"file2")->Value();

		TDateTime dtGMT;
		dtGMT.Val = fLocalTimeBiasInDays + oEvent.dtLocalTime;
		String date = dtGMT.FormatString("yyyymmdd");

		const wchar_t *ext1 = sysFile::GetFileExt(file1.c_str());
		const wchar_t *ext2 = sysFile::GetFileExt(file2.c_str());

		if (wcscmp(ext1, L"txt") == 0) {
			textEventFile = FindFilePath(oEvent.device->uuid, date, file1);

		} else if (wcscmp(ext2, L"txt") == 0) {
			textEventFile = FindFilePath(oEvent.device->uuid, date, file2);
		}

		if (textEventFile.IsEmpty() == false) {
			MemoEventDescription->Text = sysFile::ReadFile(textEventFile.c_str()).c_str();
		}
	} catch (Exception &e) {
		sysLogger::ERR_W(e.Message.c_str());
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMnemoshemaMain::FindSensorClick(TObject *Sender) {
	//find sensor under context menu
	String recordUUID = PopupMenuRecordView->PopupComponent->Name;
	TRecord *record = GetRecord(recordUUID);
	if (record == NULL) {
		return;
	}

	//make visible main device tree, even if filter is enabled
	TreeViewDeviceFiltered->Visible = false;
	TreeViewDevice->Visible = true;

	int nodeIndex = FindRecordInTreeViewDevice(TreeViewDevice, record);
	if (nodeIndex == -1) {
		ShowMessage(L"Датчик не найден в дереве устройств");
		return;
	}

	TreeViewDevice->Select(TreeViewDevice->Items->Item[nodeIndex]);
	TreeViewDevice->SetFocus();
}
//---------------------------------------------------------------------------

