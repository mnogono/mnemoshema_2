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

	UpdateControlsPosition();

	/*
	if (std::find(controls.begin(), controls.end(), viewControl) == controls.end()) {
		controls.push_back(viewControl);
	}
	*/
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
	/*
	if (this->status == status) return;

	String preff = "msch";
	if (status == SENSOR_DISPLAY_EVENT) preff = "msch_event";

	TEdit *edit = (TEdit *)viewControl;

	//TLogger::logger->Log("status: " + IntToStr(status), LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);

	edit->Font->Name = EmptyTo(DataModuleMP->GetUserSetting(user->id, preff+"_font_name"), edit->Font->Name);

	//TLogger::logger->Log("edit->Font->Name: " + edit->Font->Name, LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);

	edit->Font->Size = StrToInt(EmptyTo(DataModuleMP->GetUserSetting(user->id, preff+"_font_size"), IntToStr(edit->Font->Size)));

	//TLogger::logger->Log("edit->Font->Size: " + edit->Font->Name, LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);

	edit->Font->Color = (TColor)StrToInt(EmptyTo(DataModuleMP->GetUserSetting(user->id, preff+"_font_color"), IntToStr(edit->Font->Color)));

    //TLogger::logger->Log("edit->Font->Color: " + IntToStr(edit->Font->Color), LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);

	String fontStyleBold = EmptyTo(DataModuleMP->GetUserSetting(user->id, preff+"_font_style_bold"), "no");

    //TLogger::logger->Log("fontStyleBold: " + fontStyleBold, LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);

	String fontStyleItalic = EmptyTo(DataModuleMP->GetUserSetting(user->id, preff+"_font_style_italic"), "no");

	//TLogger::logger->Log("fontStyleItalic: " + fontStyleItalic, LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);

	String fontStyleUnderline = EmptyTo(DataModuleMP->GetUserSetting(user->id, preff+"_font_style_underline"), "no");

    //TLogger::logger->Log("fontStyleUnderline: " + fontStyleUnderline, LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);

	String fontStyleStrikeout = EmptyTo(DataModuleMP->GetUserSetting(user->id, preff+"_font_style_strikeout"), "no");

    //TLogger::logger->Log("fontStyleStrikeout: " + fontStyleStrikeout, LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);

	if (fontStyleBold == "yes") edit->Font->Style = edit->Font->Style << fsBold;
	else edit->Font->Style = edit->Font->Style >> fsBold;

	if (fontStyleItalic == "yes") edit->Font->Style = edit->Font->Style << fsItalic;
	else edit->Font->Style = edit->Font->Style >> fsItalic;

	if (fontStyleUnderline == "yes") edit->Font->Style = edit->Font->Style << fsUnderline;
	else edit->Font->Style = edit->Font->Style >> fsUnderline;

	if (fontStyleStrikeout == "yes") edit->Font->Style = edit->Font->Style << fsStrikeOut;
	else edit->Font->Style = edit->Font->Style >> fsStrikeOut;

	this->status = status;
	*/
}

//---------------------------------------------------------------------------
/*
void TSensorViewEdit::DisplayAlarm(std::vector<TData *> *alarms){
	static String hintBitAlarmMsg = "; Превышение уставки";
	static bool isEventTypeBitEnable;
	static char firstEventType, secondEventType;
	static int posBitAlarmMsg;
    static int COMPARE_TRUE = 1;
    static int COMPARE_FALSE = 0;

	//TSensor *sensor = dynamic_cast<TSensor *>(record);
	TSensor *sensor = (TSensor *)record;
	TEdit *edit = (TEdit *)viewControl;

	isEventTypeBitEnable = false;
	posBitAlarmMsg = edit->Hint.Pos(hintBitAlarmMsg);

    if (alarms) {
        for (std::vector<TData *>::iterator itAlarm = alarms->begin(), itEnd = alarms->end(); itAlarm != itEnd; ++itAlarm) {
            TDataBaseAlarm *baseAlarm = static_cast<TDataBaseAlarm *>(*itAlarm);
            if (!baseAlarm) continue;

            GetEventTypeBits(baseAlarm->eventType, &firstEventType, &secondEventType);
            if (secondEventType == EVENT_TYPE_BIT) {
                if (baseAlarm->compareResult == COMPARE_TRUE) {
                    isEventTypeBitEnable = true;
                    break;
                }
            }
        }
    }

	if (isEventTypeBitEnable) {
		if (!posBitAlarmMsg) {
			edit->Hint += hintBitAlarmMsg;
		}
		SetUserSetting(USER, SENSOR_DISPLAY_EVENT);
	} else {
		edit->Hint.SubString(posBitAlarmMsg, hintBitAlarmMsg.Length());
		SetUserSetting(USER, SENSOR_DISPLAY_NO_EVENT);
	}
}
*/

//---------------------------------------------------------------------------
void TSensorViewEdit::Highlight(){
	/*
	TSensorView::Highlight();
	((TEdit *)viewControl)->Color = clHighlight;
	*/
}

//---------------------------------------------------------------------------
bool TSensorViewEdit::IsHighlighted(){
	return false;
}

//---------------------------------------------------------------------------
void TSensorViewEdit::UndoHighlight(){
	/*
	TSensorView::UndoHighlight();
	((TEdit *)viewControl)->Color = clNormal;
	*/
}

//---------------------------------------------------------------------------
TLabel * TSensorViewEdit::GetLabelLeft() {
	return NULL;
	//return (TLabel *)GetControl(LABEL_LEFT_PREFIX + sensor->uuid);
}

//---------------------------------------------------------------------------
TLabel * TSensorViewEdit::GetLabelRight() {
	return NULL;
	//return (TLabel *)GetControl(LABEL_RIGHT_PREFIX + sensor->uuid);
}

//---------------------------------------------------------------------------
TLabel * TSensorViewEdit::GetLabelUnit() {
	return NULL;
//	return (TLabel *)GetControl(LABEL_UNIT_PREFIX + sensor->uuid);
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
	/*
	TSensorView::Resize(width, height);
	UpdateControlsPosition();
	*/
}
