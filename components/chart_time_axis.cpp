//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "chart_time_axis.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TChartTimeAxis::~TChartTimeAxis() {

}

TChartTimeAxis::TChartTimeAxis(TWinControl *owner, TMnemoshemaDataManager &mnemoshemaDataManager) {
	this->mnemoshemaDataManager = &mnemoshemaDataManager;

	timeAxis = new TChart(owner);

	timeAxis->Parent = owner;
	timeAxis->View3D = false;
	timeAxis->Legend->Visible = false;
	timeAxis->LeftAxis->Visible = false;
	timeAxis->Align = alBottom;
	timeAxis->Height = 45;
	timeAxis->AllowZoom = false;
	//disable scroll by mouse button
	timeAxis->ScrollMouseButton = (TMouseButton)-1;

	timeAxis->LeftAxis->LabelsSize = 50;

	timeAxis->RightAxis->Visible = false;
	timeAxis->TopAxis->Visible = false;
	timeAxis->Border->Visible = false;

	timeAxis->BottomAxis->DateTimeFormat = "hh:nn:ss";
	timeAxis->BottomAxis->LabelsMultiLine = true;

	timeAxis->Title->Visible = false;

	timeAxis->MarginUnits = TTeeUnits::muPixels;
	timeAxis->MarginLeft = 56;
	timeAxis->MarginRight = 5;
	timeAxis->MarginTop = 0;
	timeAxis->MarginBottom = 15;

	timeAxis->BevelInner = TBevelCut::bvNone;
	timeAxis->BevelOuter = TBevelCut::bvNone;

	timeSeries = new TLineSeries(timeAxis);
	timeSeries->XValues->DateTime = true;
	timeAxis->AddSeries(timeSeries);
}

//---------------------------------------------------------------------------
int TChartTimeAxis::GetHeight() {
	return timeAxis->Height;
}

//---------------------------------------------------------------------------
int TChartTimeAxis::GetWidth() {
	return timeAxis->Width;
}

//---------------------------------------------------------------------------
void TChartTimeAxis::SetTimeAxisBounds(double dt1LocalTime, double dt2LocalTime, bool requestData) {
	timeAxis->BottomAxis->SetMinMax(dt1LocalTime, dt2LocalTime);

	NotifyObservers(&TObserverableHistoryDateTime(dt1LocalTime, dt2LocalTime));

	if (requestData) {
		mnemoshemaDataManager->NotifyObservers(&TObserverableChangeTimeAxisByUser(dt1LocalTime, dt2LocalTime));
	}

}

//---------------------------------------------------------------------------
void TChartTimeAxis::GetTimeAxisBounds(double &dt1LocalTime, double &dt2LocalTime) {
	dt1LocalTime = timeAxis->BottomAxis->Minimum;
	dt2LocalTime = timeAxis->BottomAxis->Maximum;
}

//---------------------------------------------------------------------------
void __fastcall TChartTimeAxis::PaintTo(HDC DC, int X, int Y) {
	timeAxis->PaintTo(DC, X, Y);
}
