//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "mouse_strategy_text.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

int TEventTextMarkerCreated::GetObserverableType() {
	return TObserverableTypes::NEW_TEXT_MARKER;
}

//---------------------------------------------------------------------------
TMouseStrategyText::TMouseStrategyText(TPageControl *chartPageControl, TPopupMenu *popupMenu) : TMouseStrategy(chartPageControl, popupMenu) {
	this->popupMenu = popupMenu;
}

//---------------------------------------------------------------------------
void TMouseStrategyText::Apply(TChart *chart) {
	chart->OriginalCursor = crIBeam;
	chart->Zoom->Allow = false;

	chart->OnScroll       = OnChartScroll;
	chart->OnMouseDown    = OnChartMouseDown;
	chart->OnMouseMove    = OnChartMouseMove;
	chart->OnMouseUp      = OnChartMouseUp;
}

// ---------------------------------------------------------------------------
void TMouseStrategyText::Deactivate(TChart *chart) {}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyText::OnChartScroll(TObject *Sender) {
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyText::OnChartMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TMouseStrategy::OnChartMouseDown(Sender, Button, Shift, X, Y);
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyText::OnChartMouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	TSensorsTabSheet *activePage = static_cast<TSensorsTabSheet *>(chartPageControl->ActivePage);

	TChart *chart = static_cast<TChart *>(Sender);
	activePage->timeAxis->SetTimeAxisBounds(chart->BottomAxis->Minimum, chart->BottomAxis->Maximum);
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyText::OnChartMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TMouseStrategy::OnChartMouseUp(Sender, Button, Shift, X, Y);

	if (mouseInfo.isDblClick == true) {
		return;
	}

	if (mouseInfo.isClick == false) {
		return;
	}

	TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(Sender);
	if (recordViewChart == NULL) {
		return;
	}

	if (recordViewChart->SeriesCount() <= 0) {
		return;
	}

	double localTime;
	double yVal;
	recordViewChart->Series[0]->GetCursorValues(localTime, yVal);

	//create new trackable element to the chart
	TChartText *chartText = new TChartText(recordViewChart);
	chartText->SetVisible(true);
	chartText->Move(X, Y);
	chartText->SetLocalTime(localTime);
	chartText->SetValue(yVal);

	/*
	chartText->OnEnter = ChartTextOnEnter;
	chartText->OnExit = ChartTextOnExit;
	chartText->PopupMenu = popupMenu;
	*/

	recordViewChart->AddTrackableElement(chartText);

	TEventTextMarkerCreated event;
	NotifyObservers(&event);
}

void __fastcall TMouseStrategyText::ChartTextOnEnter(TObject *Sender) {
	/*
	TChartText *chartText = dynamic_cast<TChartText *>(Sender);
	if (chartText != NULL) {
		chartText->Highlight(true);
	}
	*/
}

void __fastcall TMouseStrategyText::ChartTextOnExit(TObject *Sender) {
	/*
	TChartText *chartText = dynamic_cast<TChartText *>(Sender);
	if (chartText != NULL) {
		chartText->Highlight(false);
	}
	*/
}


