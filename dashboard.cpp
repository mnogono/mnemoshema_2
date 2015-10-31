//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "dashboard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//store mouse state for perform dashboard moving by mouse
//TMshMouseState mshMouseState;

TFormDashboard *FormDashboard;

//---------------------------------------------------------------------------
__fastcall TFormDashboard::TFormDashboard(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::CreateTimeRangeDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	timeRangeDataObserver = new TTimeRangeDataObserver(mshViews);
	mnemoshemaDataManager.AddObserver(timeRangeDataObserver);
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::CreateHTTPDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	httpDataObserver = new THTTPDataObserver(mshViews);
	mnemoshemaDataManager.AddObserver(httpDataObserver);

	if (craneMimicPanel != NULL) {
		mnemoshemaDataManager.AddObserver(craneMimicPanel);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::CreateMnemoshemaDataHistoryObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	mnemoshemaDataHistoryObserver = new TMnemoshemaDataHistoryObserver(mshViews);
	mnemoshemaDataManager.AddObserver(mnemoshemaDataHistoryObserver);
}

//---------------------------------------------------------------------------
/*
void __fastcall TFormDashboard::CreateFileDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	fileDataObserver = new TFileDataObserver(mnemoshemaDataManager, recordViews);
	mnemoshemaDataManager.AddObserver(fileDataObserver);
}
*/
//---------------------------------------------------------------------------
/*
void __fastcall TFormDashboard::CreateDeviceFileDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	deviceFileDataObserver = new TDeviceFileDataObserver(mnemoshemaDataManager, mshViews);
	mnemoshemaDataManager.AddObserver(deviceFileDataObserver);
}
*/

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::FormCreate(TObject *Sender)
{
	CreateMnemoshemsPages();

	LoadDashboardElements();

	RestoreLastUserSettings();

}

// ---------------------------------------------------------------------------
void __fastcall TFormDashboard::AddSensorView(TWinControl *parent, TRecord *record, int x, int y) {
	TSensor *sensor;

	if (record->record_type == RECORD_TYPE_SENSOR){
		sensor = (TSensor *)record;

	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT){
		TSensorBit *sensorBit = (TSensorBit *)(record);
		sensor = GetSensor(sensorBit->sensor_id);
	}

	IMnemoshemaView *mshView = TSensorViewFactory::Build(parent, record);
	if (mshView == NULL) {
		return;
	}

	if (mshViews.find(sensor) == mshViews.end()){
		mshViews[sensor] = new std::list<IMnemoshemaView *>();
	}

	mshViews[sensor]->push_back(mshView);
}

// ---------------------------------------------------------------------------
void TFormDashboard::InitCreatedRecordView(TSensorView *createdRecordView) {
}

// ---------------------------------------------------------------------------
void __fastcall OnDashboardMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
}

// ---------------------------------------------------------------------------
void __fastcall TFormDashboard::LoadDashboardElements() {
	sysLogger::TRACE_A("Creating mnemoshema user interface...");

	std::stringstream ss;
	std::wstringstream wss;

	for (std::list<TMnemoshema *>::iterator it = MNEMOSHEMS.begin(), itEnd = MNEMOSHEMS.end(); it != itEnd; ++it) {
		TMnemoshema *mnemoshema = *it;

		sysLogger::TRACE_A("Creating mnemoshema:");
		sysLogger::TRACE_W(mnemoshema->name.c_str());

		TScrollBox *scrollBoxImage = NULL;
		try {
			TComponent *component = sysVCL::StringToComponent(mnemoshema->structure);

			if (component == NULL) {
				sysLogger::ERR_A("Mnemoshema structure can't be converted into visual component");
				sysLogger::ERR_W(mnemoshema->name.c_str());
				sysLogger::ERR_W(mnemoshema->structure.c_str());
				continue;
			}

			scrollBoxImage = dynamic_cast<TScrollBox *>(component);
			if (scrollBoxImage == NULL) {
				sysLogger::ERR_A("The mnemoshema structure can't be converted into scroll box component");
				sysLogger::ERR_W(mnemoshema->name.c_str());
				sysLogger::ERR_W(mnemoshema->structure.c_str());
				continue;
			}
		} catch (Exception &e) {
			sysLogger::ERR_W(e.Message.c_str());
			continue;
		}

		//find the page sheet
		String pageName = "TabSheetMnemoshema" + IntToStr(mnemoshema->id);
		TTabSheet *tabSheet = dynamic_cast<TTabSheet *>(sysVCL::FindControl(PageControlMP, pageName));
		if (!tabSheet) {
			sysLogger::ERR_A("The tab sheet can't be found:");
			sysLogger::ERR_W(pageName.c_str());
			continue;
		}

		scrollBoxImage->Parent = tabSheet;
		scrollBoxImage->Align = alClient;
		scrollBoxImage->Visible = true;
		scrollBoxImage->VertScrollBar->Tracking = true;
		scrollBoxImage->HorzScrollBar->Tracking = true;
		scrollBoxImage->DoubleBuffered = true;

		String mimePanelBkgImgName = "bkgImg" + mnemoshema->uuid;

		//if crane mimic panel, make initialization of craneMimicPanel object
		if (mnemoshema->uuid == "CraneMimicPanel") {
			InitializeCraneMimicPanel(scrollBoxImage, mimePanelBkgImgName);
		}

		for (int ctr = 0; ctr < scrollBoxImage->ControlCount; ++ctr){
			TControl *control = scrollBoxImage->Controls[ctr];
			TRecord *record = GetRecord(control->Name);
			if (record){
				AddSensorView(scrollBoxImage, record, control->Left, control->Top);

			} else if (control->Name == mimePanelBkgImgName){
				TImage *image = dynamic_cast<TImage *>(control);
				image->Align = alNone;
				image->AutoSize = true;

				image->OnMouseDown = dashboardMouseStrategy.OnMouseDown;
				image->OnMouseUp = dashboardMouseStrategy.OnMouseUp;
				image->OnMouseMove = dashboardMouseStrategy.OnMouseMove;
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDashboard::InitializeCraneMimicPanel(TScrollBox *ScrollBoxImage, String &mimePanelBkgImgName) {
	if (sysApp::GetSettingInt(L"EnableCrane", 1) == 0) {
		return;
	}

	if (craneMimicPanel) {
		delete craneMimicPanel;
	}

	// initialize crane1

	TImage *img = dynamic_cast<TImage *>(sysVCL::FindControl(ScrollBoxImage, mimePanelBkgImgName.c_str()));

	//make copy of picture for send it to craneMimicPanel
	TBitmap *bitmap = new TBitmap();
	bitmap->Assign(img->Picture->Graphic);

	img->Picture->Graphic = 0;

	craneMimicPanel = new TCraneMimicPanel(img->Canvas, bitmap);

	craneMimicPanel->clear();
	craneMimicPanel->draw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDashboard::CreateMnemoshemsPages() {
    sysLogger::TRACE_A("Updating users mnemoshems as panel pages");

	int count = PageControlMP->PageCount;
	for (int page = count - 1; page >= 0; page--){
		delete PageControlMP->Pages[page];
	}

	for (std::list<TMnemoshema *>::iterator it = MNEMOSHEMS.begin(), itEnd = MNEMOSHEMS.end(); it != itEnd; ++it) {
		TMnemoshema *mnemoshema = *it;

		TTabSheet *TabSheet = new TTabSheet(PageControlMP);
		TabSheet->Parent = PageControlMP;
		TabSheet->PageControl = PageControlMP;
		TabSheet->Name = "TabSheetMnemoshema" + IntToStr(mnemoshema->id);
		TabSheet->Caption = mnemoshema->name;

		//registers tab sheet events
		//TabSheet->OnMouse
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::SaveLastUserSettings() {
	//save user settings before close the application
	sysApp::SaveFormSettings(this);

	String prefix = L"dashboard_";
	sysApp::SaveControlState(CheckBoxDataHistory, prefix);
	sysApp::SaveControlState(DateDashboardHistory, prefix);
	sysApp::SaveControlState(TimeDashboardHistory, prefix);
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::RestoreLastUserSettings() {
//	sysApp::RestoreFormSettings(this);
	std::list<TControl *> controls;
	controls.push_back(CheckBoxDataHistory);
	controls.push_back(DateDashboardHistory);
	controls.push_back(TimeDashboardHistory);

	String prefix = L"dashboard_";
	sysApp::RestoreControlsState(controls, prefix);

}
//---------------------------------------------------------------------------
void __fastcall TFormDashboard::FormDestroy(TObject *Sender) {
	SaveLastUserSettings();
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::SetMnemoshemaDataManager(TMnemoshemaDataManager &mnemoshemaDataManager) {
	CreateHTTPDataObserver(mnemoshemaDataManager);

	CreateTimeRangeDataObserver(mnemoshemaDataManager);

	//CreateDeviceFileDataObserver(mnemoshemaDataManager);
}

//---------------------------------------------------------------------------
void TFormDashboard::OnTreeViewNodeChange(TTreeNode *node) {
	//do we need to find the record view or not?
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::FormShow(TObject *Sender) {
	/*
	static bool maximazeOnce = false;
	if (!maximazeOnce) {
		maximazeOnce = true;
		WindowState = wsMaximized;
		Left = 0;
		Top = 0;
		Width = 600;
		Height = 800;
	}
	*/
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::RequestDataMnemoshemaHistory() {
	if (CheckBoxDataHistory->Checked == false) {
		return;
	}

	std::set<const TSensor *> sensors;
	for (std::list<TSensor *>::iterator i = SENSORS.begin(), iEnd = SENSORS.end(); i != iEnd; ++i) {
		sensors.insert(*i);
	}

	static double fLocalTimeBiasInDays = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;

	double dtGMT = fLocalTimeBiasInDays + floor(DateDashboardHistory->Date.Val)/*date*/ + TimeDashboardHistory->Time.Val - floor(TimeDashboardHistory->Time.Val) /*time*/;

	REQUEST_MNEMOSHEMA_DATA(sensors, dtGMT);
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::CheckBoxDataHistoryClick(TObject *Sender) {
	RequestDataMnemoshemaHistory();
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::ButtonRequestDataMnemoshemaHistoryClick(TObject *Sender) {
	RequestDataMnemoshemaHistory();
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::DateDashboardHistoryChange(TObject *Sender) {
	RequestDataMnemoshemaHistory();
}

//---------------------------------------------------------------------------
void __fastcall TFormDashboard::TimeDashboardHistoryChange(TObject *Sender) {
	RequestDataMnemoshemaHistory();
}
//---------------------------------------------------------------------------

