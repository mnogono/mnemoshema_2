//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "record_view_string_grid_container.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TRecordViewStringGridContainer::TRecordViewStringGridContainer(TStringGrid *stringGrid, TRecord *record, int row) :
	TRecordView((TWinControl *)stringGrid, record) {

	this->row = row;
}

//---------------------------------------------------------------------------
__fastcall TRecordViewStringGridContainer::~TRecordViewStringGridContainer() {
}

//---------------------------------------------------------------------------
void TRecordViewStringGridContainer::MoveTo(int x, int y) {
}

//---------------------------------------------------------------------------
void TRecordViewStringGridContainer::CreateControls() {
}

//---------------------------------------------------------------------------
void TRecordViewStringGridContainer::DisplayData(std::list<TSensorData *> *data) {
	if (data == NULL || data->empty()) {
		DisplayData((TSensorData *)NULL);
	}

	DisplayData(data->front());
}

//---------------------------------------------------------------------------
void TRecordViewStringGridContainer::DisplayData(TSensorData *data) {
	TStringGrid *stringGrid = (TStringGrid *)parent;

	static TDateTime dtGMT;
	static double bias = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;

	if (data == NULL) {
		stringGrid->Cells[1][row] = "Í/Ä";

	} else if (record->record_type == RECORD_TYPE_SENSOR) {
		stringGrid->Cells[1][row] = SensorDataToString(static_cast<const TSensor *>(record), data, true);
		dtGMT.Val = sysTime::ConvertToDays(data->timeGMT * sysTime::MSEC2SEC) - bias;
		stringGrid->Cells[2][row] = dtGMT.FormatString("hh:nn:ss");

	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		stringGrid->Cells[1][row] = SensorBitDataToString(static_cast<const TSensorBit *>(record), data);
		dtGMT.Val = sysTime::ConvertToDays(data->timeGMT * sysTime::MSEC2SEC) - bias;
		stringGrid->Cells[2][row] = dtGMT.FormatString("hh:nn:ss");
	}
}

//---------------------------------------------------------------------------
void TRecordViewStringGridContainer::Highlight() {
}

//---------------------------------------------------------------------------
void TRecordViewStringGridContainer::UndoHighlight() {
}

//---------------------------------------------------------------------------
bool TRecordViewStringGridContainer::IsHighlighted() {
	return false;
}

//---------------------------------------------------------------------------
void TRecordViewStringGridContainer::SetPopupMenu(TPopupMenu *menu) {
    static_cast<TStringGrid *>(parent)->PopupMenu = menu;
}
