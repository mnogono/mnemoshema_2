//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "recordInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormRecordInfo *FormRecordInfo;

//---------------------------------------------------------------------------
__fastcall TFormRecordInfo::TFormRecordInfo(TComponent* Owner) : TForm(Owner) {
	record = NULL;
    currentPanel = NULL;
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::SetRecord(TRecord *record){
	this->record = record;

	if (!record) {
		return;
    }

    if (currentPanel) {
		currentPanel->Visible = false;
    }

    if (record->record_type == RECORD_TYPE_SENSOR) {
    	ShowSensor();
    } else if (record->record_type == RECORD_TYPE_DEVICE) {
    	ShowDevice();
	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
    	SetRecord(static_cast<TSensorBit *>(record)->sensor);
	}
}

//---------------------------------------------------------------------------
void TFormRecordInfo::OnTreeViewNodeChange(TTreeNode *node) {
	if (node == NULL){
		return;
	}

	TRecord *record = static_cast<TRecord *>(node->Data);
	if (record == NULL){
		return;
	}

	SetRecord(record);
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::FormShow(TObject *Sender){
	/*
	if (!record) {
		return;
    }

    if (currentPanel) {
		currentPanel->Visible = false;
    }

    if (record->record_type == RECORD_TYPE_SENSOR) {
    	ShowSensor();
    } else if (record->record_type == RECORD_TYPE_DEVICE) {
    	ShowDevice();
	}
	*/
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::ShowDevice(){
	if (!record) {
		return;
    }

    if (record->record_type != RECORD_TYPE_DEVICE) {
		return;
    }

    TDevice *device = (TDevice *)record;

    EditDeviceUUID->Text = device->uuid;

    int dataPointIndex = sysVCL::FindComboBoxItemByObject(ComboBoxDeviceCollectionPoint, IntToStr(device->collection_point_id));
    ComboBoxDeviceCollectionPoint->ItemIndex = dataPointIndex;

    EditDeviceName->Text = device->name;

    SetInvisiblePanels();

    PanelDeviceSettingAdditional->Visible = true;
    PanelDeviceSettingAdditional->Parent = FormRecordInfo;

    PanelDevice->Visible = true;
    PanelDevice->Parent = FormRecordInfo;
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::ShowSensor(){
	if (!record) {
		return;
    }

	if (record->record_type != RECORD_TYPE_SENSOR) {
		return;
    }

	TSensor *sensor = (TSensor *)record;

	TDevice *device = GetDevice(sensor->device_id);

	if (device) {
		EditSensorDevice->Text = device->name;

		TCollectionPoint *collectionPoint = GetCollectionPoint(device->collection_point_id);
		if (collectionPoint != NULL) {
			EditSensorRemotePoint->Text = collectionPoint->title;
        }
    }

    EditSensorName->Text = sensor->name;

    EditSensorDescription->Text = sensor->desc;

    int unitIndex = sysVCL::FindComboBoxItemByObject(ComboBoxSensorUnit, IntToStr(sensor->unit));
    ComboBoxSensorUnit->ItemIndex = unitIndex;

    int powerIndex = sysVCL::FindComboBoxItemByObject(ComboBoxSensorPower, IntToStr(sensor->power));
    ComboBoxSensorPower->ItemIndex = powerIndex;

    int precisionIndex = sysVCL::FindComboBoxItemByObject(ComboBoxSensorPower, IntToStr(sensor->precision));
	ComboBoxSensorPrecision->ItemIndex = precisionIndex;

	//display gauss filter
	String sensorGaussFilterName = sensor->uuid + String("_gaussWidth");
	EditGaussFilter->Text = DataModuleMP->GetSetting(sensorGaussFilterName);

	EditSensorMinValue->Text = FloatToStr(sensor->min_value);

	EditSensorMaxValue->Text = FloatToStr(sensor->max_value);

    EditSensorUUID->Text = sensor->uuid;

    SetInvisiblePanels();

	PanelSensor->Visible = true;
    PanelSensor->Parent = FormRecordInfo;

	currentPanel = PanelSensor;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::InitComboBoxSensorUnit(){
	ComboBoxSensorUnit->AddItem("", (TObject *)new String("0"));
	ComboBoxSensorUnit->AddItem("В", (TObject *)new String("1"));
	ComboBoxSensorUnit->AddItem("А", (TObject *)new String("2"));
	ComboBoxSensorUnit->AddItem("Вт", (TObject *)new String("3"));
	ComboBoxSensorUnit->AddItem("ВАр", (TObject *)new String("4"));
	ComboBoxSensorUnit->AddItem("Гц", (TObject *)new String("5"));
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::DeleteComboBoxStringObjectElements(TComboBox *comboBox){
	for (int i = comboBox->Items->Count - 1; i >= 0; --i) {
    	String *str = (String *)comboBox->Items->Objects[i];
    	delete str;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::InitComboBoxSensorPower(){
	ComboBoxSensorPower->AddItem("", (TObject *)new String("0"));
	ComboBoxSensorPower->AddItem("к (кило)", (TObject *)new String("1"));
	ComboBoxSensorPower->AddItem("М (Мега)", (TObject *)new String("2"));
	ComboBoxSensorPower->AddItem("Г (Гига)", (TObject *)new String("3"));
	ComboBoxSensorPower->AddItem("Т (Тера)", (TObject *)new String("4"));
	ComboBoxSensorPower->AddItem("м (мили)", (TObject *)new String("-1"));
	ComboBoxSensorPower->AddItem("мк (микро)", (TObject *)new String("-2"));
	ComboBoxSensorPower->AddItem("н (нано)", (TObject *)new String("-3"));
	ComboBoxSensorPower->AddItem("п (пика)", (TObject *)new String("-4"));
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::InitComboBoxSensorPrecision(){
	ComboBoxSensorPrecision->AddItem("Без округления", (TObject *)new String("8"));
	ComboBoxSensorPrecision->AddItem("y", (TObject *)new String("0"));
	ComboBoxSensorPrecision->AddItem("y.x", (TObject *)new String("1"));
	ComboBoxSensorPrecision->AddItem("y.xx", (TObject *)new String("2"));
	ComboBoxSensorPrecision->AddItem("y.xxx", (TObject *)new String("3"));
	ComboBoxSensorPrecision->AddItem("y.xxxx", (TObject *)new String("4"));
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::InitComboBoxDeviceCollectionPoint(){
	for (std::list<TCollectionPoint *>::iterator i = COLLECTION_POINTS.begin(), iEnd = COLLECTION_POINTS.end(); i != iEnd; ++i) {
		ComboBoxDeviceCollectionPoint->AddItem((*i)->title, (TObject *)new String(IntToStr((*i)->id)));
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::UserLogin(TUser *User){
    InitComboBoxSensorUnit();

    InitComboBoxSensorPower();

    InitComboBoxSensorPrecision();

    InitComboBoxDeviceCollectionPoint();
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::FormDestroy(TObject *Sender) {
	DeleteComboBoxStringObjectElements(ComboBoxSensorUnit);
    DeleteComboBoxStringObjectElements(ComboBoxSensorPower);
	DeleteComboBoxStringObjectElements(ComboBoxSensorPrecision);
	DeleteComboBoxStringObjectElements(ComboBoxDeviceCollectionPoint);

	SaveLastUserSettings();
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::SetInvisiblePanels() {
    PanelDeviceSettingAdditional->Visible = false;
    PanelDevice->Visible = false;
	PanelSensor->Visible = false;
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::EditGaussFilterChange(TObject *Sender) {
	/*
	if (record->record_type != RECORD_TYPE_SENSOR) {
		return;
	}

	TSensor *sensor = (TSensor *)record;
	String sensorGaussFilterName = sensor->uuid + String("_gaussWidth");
	double value = StrToFloatDef(EditGaussFilter->Text, 0);
	DataModuleMP->SetSettingFloat(sensorGaussFilterName, value, L"");
	*/
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::RestoreLastUserSettings() {
	sysApp::RestoreFormSettings(this);
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::SaveLastUserSettings() {
	sysApp::SaveFormSettings(this);
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::FormCreate(TObject *Sender) {
	RestoreLastUserSettings();
}

//---------------------------------------------------------------------------
void __fastcall TFormRecordInfo::FormCloseQuery(TObject *Sender, bool &CanClose) {
	WindowState = wsMinimized;
	CanClose = false;
}
//---------------------------------------------------------------------------

