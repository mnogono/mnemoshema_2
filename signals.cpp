//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "signals.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSignals *FormSignals;

//---------------------------------------------------------------------------
__fastcall TFormSignals::TFormSignals(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TFormSignals::CreateHTTPDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	httpDataObserver = new THTTPDataObserver(mshViews);
	mnemoshemaDataManager.AddObserver(httpDataObserver);
}

//---------------------------------------------------------------------------
void __fastcall TFormSignals::CreateTimeRangeDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	timeRangeDataObserver = new TTimeRangeDataObserver(mshViews);
	mnemoshemaDataManager.AddObserver(timeRangeDataObserver);
}

//---------------------------------------------------------------------------
void __fastcall TFormSignals::CreateMnemoshemaDataHistoryObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	mnemoshemaDataHistoryObserver = new TMnemoshemaDataHistoryObserver(mshViews);
	mnemoshemaDataManager.AddObserver(mnemoshemaDataHistoryObserver);
}

//---------------------------------------------------------------------------
void __fastcall TFormSignals::CreateRecordViews() {
	//iterate over all main tree view devices, and create for each of the record, correspond record view for current signal
	TTreeView *treeViewDevice = FormMnemoshemaMain->TreeViewDevice;
	int count = treeViewDevice->Items->Count;
	int row = 1;
	for (int i = 0; i < count; ++i) {
		TTreeNode *node = treeViewDevice->Items->Item[i];
		if (node->Data == NULL) {
			continue;
		}

		TRecord *record = (TRecord *)node->Data;
		if (record == NULL) {
			continue;
		}

		if (record->record_type == RECORD_TYPE_SENSOR) {
			const TSensor *sensor = static_cast<const TSensor *>(record);
			if (mshViews.find(sensor) == mshViews.end()){
				mshViews[sensor] = new std::list<IMnemoshemaView *>();
			}

			IMnemoshemaView *mshView = new TRecordViewStringGridContainer(StringGridSignals, record, row++);

			mshViews[sensor]->push_back(mshView);
		} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
			TSensorBit *sensorBit = static_cast<TSensorBit *>(record);
			const TSensor *sensor = sensorBit->sensor; //GetSensor(sensorBit->sensor_id);
			if (sensor == NULL) {
				continue;
			}

			if (mshViews.find(sensor) == mshViews.end()){
				mshViews[sensor] = new std::list<IMnemoshemaView *>();
			}

			IMnemoshemaView *mshView = new TRecordViewStringGridContainer(StringGridSignals, record, row++);

			mshViews[sensor]->push_back(mshView);
		}
	}

	if (row > 1) {
		StringGridSignals->RowCount = row + 1;

		//update sensors captions for first string grid column
		for (std::map<const TSensor *, std::list<IMnemoshemaView *> *>::iterator i = mshViews.begin(), iEnd = mshViews.end(); i != iEnd; ++i) {
			std::list<IMnemoshemaView *> *views = i->second;
			for (std::list<IMnemoshemaView *>::iterator iView = views->begin(), iViewEnd = views->end(); iView != iViewEnd; ++iView) {
				const TRecord *record = (*iView)->GetRecord();
				TRecordViewStringGridContainer *view = static_cast<TRecordViewStringGridContainer *>(*iView);

				String name;
				if (record->record_type == RECORD_TYPE_SENSOR) {
					const TSensor *sensor = static_cast<const TSensor *>(record);
					const TDevice *device = GetDevice(sensor->device_id);
					name = device->GetName() + " " + sensor->GetName();
				} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
					const TSensorBit *sensorBit = static_cast<const TSensorBit *>(record);
					const TDevice *device = GetDevice(sensorBit->sensor->device_id);
					name = device->GetName() + " " + record->GetName();

				} else {
                    name = record->GetName();
                }

				StringGridSignals->Cells[0][view->row] = name;
			}
		}

	}

	StringGridSignals->Cells[0][0] = "Датчик";
	StringGridSignals->Cells[1][0] = "Сигнал";
	StringGridSignals->Cells[2][0] = "Время";
}

//---------------------------------------------------------------------------
void __fastcall TFormSignals::FormCreate(TObject *Sender) {
	TileMode = tbHorizontal;

	CreateRecordViews();

	RestoreLastUserSettings();

//	HMENU systemMenu = GetSystemMenu(Application->Handle, FALSE);
	/*
	if (DeleteMenu(systemMenu, 0, MF_BYPOSITION) == 0) {
	}
	*/
	/*
	while (GetMenuItemCount(systemMenu) > 1) {
		DeleteMenu(systemMenu, 1, MF_BYPOSITION);
	}
	DrawMenuBar(Application->Handle);
	*/
}
//---------------------------------------------------------------------------
void __fastcall TFormSignals::SaveLastUserSettings() {
	//save user settings before close the application
	sysApp::SaveFormSettings(this);

	//save signal column width
	for (int col = 0; col < StringGridSignals->ColCount; ++col) {
		String name = String(L"StringGridSignalsColWidth") + IntToStr(col);
		String value = IntToStr(StringGridSignals->ColWidths[col]);
		sysApp::SetSetting(name.c_str(), value.c_str());
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormSignals::RestoreLastUserSettings() {
	sysApp::RestoreFormSettings(this);

	//restore prev col width settings
	for (int col = 0; col < StringGridSignals->ColCount; ++col) {
		String name = String(L"StringGridSignalsColWidth") + IntToStr(col);
		StringGridSignals->ColWidths[col] = sysApp::GetSettingInt(name.c_str(), 100);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormSignals::SetMnemoshemaDataManager(TMnemoshemaDataManager &mnemoshemaDataManager) {
	CreateHTTPDataObserver(mnemoshemaDataManager);

	CreateTimeRangeDataObserver(mnemoshemaDataManager);

	CreateMnemoshemaDataHistoryObserver(mnemoshemaDataManager);
}

//---------------------------------------------------------------------------
void TFormSignals::OnTreeViewNodeChange(TTreeNode *node) {
	const TRecord *record = (TRecord *)node->Data;
	if (record == NULL) {
		return;
	}

	if (record->record_type == RECORD_TYPE_SENSOR) {
		const TSensor *sensor = static_cast<const TSensor *>(record);
		if (mshViews.find(sensor) == mshViews.end()) {
			return;
		}

		std::list<IMnemoshemaView *> *views = mshViews[sensor];
		if (views->empty()) {
			return;
		}

		TRecordViewStringGridContainer *view = static_cast<TRecordViewStringGridContainer *>(views->front());
		StringGridSignals->Row = view->row;

	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		const TSensorBit *sensorBit = static_cast<const TSensorBit *>(record);
		const TSensor *sensor = sensorBit->sensor;

		if (mshViews.find(sensor) == mshViews.end()) {
			return;
		}

		std::list<IMnemoshemaView *> *views = mshViews[sensor];
		for (std::list<IMnemoshemaView *>::iterator itView = views->begin(); itView != views->end(); ++itView) {
			TRecordViewStringGridContainer *view = static_cast<TRecordViewStringGridContainer *>(*itView);
			if (view->GetRecord() == record) {
				StringGridSignals->Row = view->row;
			}
		}
	} else if (record->record_type == RECORD_TYPE_DEVICE) {
		const TDevice *device = static_cast<const TDevice *>(record);
		std::list<TSensor *> sensors = GetSensors(device);
		if (sensors.empty()) {
			return;
		}

		TSensor *frontSensor = sensors.front();
		TSensor *backSensor = sensors.back();

		TGridRect selection;
		selection.Left = 0;
		selection.Right = 2;

		if (mshViews.find(frontSensor) != mshViews.end() && mshViews.find(backSensor) != mshViews.end()) {
			std::list<IMnemoshemaView *> *frontViews = mshViews[frontSensor];
			if (frontViews->empty()) {
				return;
			}

			TRecordViewStringGridContainer *frontView = static_cast<TRecordViewStringGridContainer *>(frontViews->front());

			std::list<IMnemoshemaView *> *backViews = mshViews[backSensor];
			if (backViews->empty()) {
				return;
			}

			TRecordViewStringGridContainer *backView = static_cast<TRecordViewStringGridContainer *>(backViews->front());

			selection.Top = frontView->row;
			selection.Bottom = backView->row;

			StringGridSignals->Row = frontView->row;
		}

		StringGridSignals->Selection = selection;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormSignals::FormCloseQuery(TObject *Sender, bool &CanClose) {
	WindowState = wsMinimized;
	CanClose = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormSignals::FormDestroy(TObject *Sender) {
	SaveLastUserSettings();
}
//---------------------------------------------------------------------------

