//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "chart_text.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TChartText::TChartText(TChart *chart) {
	this->chart = chart;

	memo = new TMemo(chart);
	memo->Parent = chart;
	memo->Height = 22;
	memo->Width = 150;

	box = new TShape(chart);
	box->Parent = chart;
	box->Shape = stRectangle;
	box->Pen->Color = clBlack;
}

//---------------------------------------------------------------------------
inline __fastcall TChartText::~TChartText() {}

void TChartText::Highlight(bool highlight) {
    box->Visible = highlight;
}

//---------------------------------------------------------------------------
void TChartText::UpdateBox() {
	box->Width = memo->Width + 2;
	box->Height = memo->Height + 2;
	box->Left = memo->Left - 1;
	box->Top = memo->Top - 1;
}

//---------------------------------------------------------------------------
void TChartText::Move(int x, int y) {
	memo->Left = x;
	memo->Top = y;

	UpdateBox();
}

//---------------------------------------------------------------------------
void TChartText::SetVisible(bool visible) {
	memo->Visible = visible;
}

//---------------------------------------------------------------------------
bool TChartText::IsVisible() const {
	return memo->Visible;
}

//---------------------------------------------------------------------------
double TChartText::GetLocalTime() const {
	return trackLocalTime;
}

//---------------------------------------------------------------------------
void TChartText::SetLocalTime(double localTime) {
	trackLocalTime = localTime;
}

//---------------------------------------------------------------------------
void TChartText::SetValue(double value) {
	trackValue = value;
}

//---------------------------------------------------------------------------
bool TChartText::TrackValue() {
	return true;
}

//---------------------------------------------------------------------------
double TChartText::GetValue() const {
	return trackValue;
}

//---------------------------------------------------------------------------
bool TChartText::IsEqual(TChartTrackable *chartTrackable) {
	return chartTrackable->GetType() == GetType() && chartTrackable->GetLocalTime() == GetLocalTime();
}

//---------------------------------------------------------------------------
int TChartText::GetType() {
	return trackable::TTrackableType::TEXT;
}
