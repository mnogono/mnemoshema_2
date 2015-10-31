//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "SensorViewEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TSensorViewEdit::TSensorViewEdit(TWinControl *parent, TSensor *sensor) : TRecordViewEdit(parent, sensor) {
	sysLogger::TRACE_A("Creating the sensor view edit sensor uuid:");
	sysLogger::TRACE_W(sensor->uuid.c_str());

	clHighlight = clMoneyGreen;

	clNormal = clWhite;

	this->sensor = sensor;
}

//---------------------------------------------------------------------------
void TSensorViewEdit::CreateControls() {
	TRecordViewEdit::CreateControls();

	unitLabel->Caption = sensor->powerPrefix + sensor->unitName;

	//update hint for record view edit
	const TDevice *device = GetDevice(sensor->device_id);
	if (device != NULL) {
		viewControl->Hint = "Устройство: " + device->GetName() + "\nДатчик: " + sensor->GetName();
	} else {
	    viewControl->Hint = "Датчик: " + sensor->GetName();
	}

	UpdateControlsPosition();
}

//---------------------------------------------------------------------------
void TSensorViewEdit::DisplayData(std::list<TSensorData *> *data){
	if (data != NULL && data->size() != 0) {
		DisplayText(&SensorDataToString(sensor, data->back(), false));
	} else {
		DisplayText(NULL);
	}
}

//---------------------------------------------------------------------------
void TSensorViewEdit::DisplayData(TSensorData *data){
	if (data != NULL) {
		DisplayText(&SensorDataToString(sensor, data, false));
	} else {
		DisplayText(NULL);
	}
};

//---------------------------------------------------------------------------
void TSensorViewEdit::SetUserSetting(TUser *user, int status){
}


//---------------------------------------------------------------------------
void TSensorViewEdit::Highlight(){
}

//---------------------------------------------------------------------------
bool TSensorViewEdit::IsHighlighted(){
	return false;
}

//---------------------------------------------------------------------------
void TSensorViewEdit::UndoHighlight(){
}

//---------------------------------------------------------------------------
TLabel * TSensorViewEdit::GetLabelLeft() {
	return NULL;
}

//---------------------------------------------------------------------------
TLabel * TSensorViewEdit::GetLabelRight() {
	return NULL;
}

//---------------------------------------------------------------------------
TLabel * TSensorViewEdit::GetLabelUnit() {
	return NULL;
}

//---------------------------------------------------------------------------
void TSensorViewEdit::UpdateControlsPosition() {
	TLabel *leftLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_LEFT_PREFIX + sensor->uuid);
	if (leftLabel) {
		leftLabel->Left = viewControl->Left - leftLabel->Width - 5;
		leftLabel->Top = viewControl->Top + (viewControl->Height - leftLabel->Height) / 2;
	}

	TLabel *rightLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_RIGHT_PREFIX + sensor->uuid);
	if (rightLabel) {
		rightLabel->Left = viewControl->Left + LABEL_UNIT_WIDTH + viewControl->Width + 5;
		rightLabel->Top = viewControl->Top + (viewControl->Height - rightLabel->Height) / 2;
	}

	TLabel *unitLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_UNIT_PREFIX + sensor->uuid);
	if (unitLabel) {
		unitLabel->Left = viewControl->Left + viewControl->Width + 5;
		unitLabel->Top = viewControl->Top + (viewControl->Height - unitLabel->Height) / 2;
	}
}

//---------------------------------------------------------------------------
void TSensorViewEdit::Resize(int width, int height) {
}
