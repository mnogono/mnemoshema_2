//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "mouse_strategy_vertical_line.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

int TEventVerticalLineCreated::GetObserverableType() {
	return TObserverableTypes::NEW_VERTICAL_LINE;
}

//---------------------------------------------------------------------------
TMouseStrategyVerticalLine::TMouseStrategyVerticalLine(TPageControl *chartPageControl, TPopupMenu *popupMenu) : TMouseStrategy(chartPageControl, popupMenu) {
	this->popupMenu = popupMenu;
}

//---------------------------------------------------------------------------
void TMouseStrategyVerticalLine::Apply(TChart *chart) {
	chart->OriginalCursor = crHSplit;
	chart->Zoom->Allow = false;

	chart->OnScroll       = OnChartScroll;
	chart->OnMouseDown    = OnChartMouseDown;
	chart->OnMouseMove    = OnChartMouseMove;
	chart->OnMouseUp      = OnChartMouseUp;

	TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(chart);
	if (recordViewChart == NULL) {
		return;
	}

	recordViewChart->chartVerticalLine->SetVisible(true);

	chart->PopupMenu = popupMenu;
}

//---------------------------------------------------------------------------
void TMouseStrategyVerticalLine::Deactivate(TChart *chart) {
	TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(chart);
	if (recordViewChart == NULL) {
		return;
	}

	recordViewChart->chartVerticalLine->SetVisible(false);

	chart->PopupMenu = NULL;
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyVerticalLine::OnChartScroll(TObject *Sender) {
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyVerticalLine::OnChartMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TMouseStrategy::OnChartMouseDown(Sender, Button, Shift, X, Y);
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyVerticalLine::OnChartMouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	TMouseStrategy::OnChartMouseMove(Sender, Shift, X, Y);

	TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(Sender);
	if (recordViewChart == NULL) {
		return;
	}

	recordViewChart->chartVerticalLine->Move(X, Y);
	double localTime = recordViewChart->BottomAxis->CalcPosPoint(X);
	recordViewChart->chartVerticalLine->SetLocalTime(localTime);

	std::map<TChartSeries *, double> seriesValues = recordViewChart->GetSeriesValues(localTime);

	recordViewChart->chartVerticalLine->SetCaption("", localTime, seriesValues);
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyVerticalLine::CreateVerticalLine(TObject *Sender) {
	TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(Sender);
	if (recordViewChart == NULL) {
		return;
	}

	recordViewChart->AddTrackableElement(
		new TChartVerticalLine(*recordViewChart->chartVerticalLine)
	);

	TEventVerticalLineCreated event;
	NotifyObservers(&event);
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyVerticalLine::OnChartMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TMouseStrategy::OnChartMouseUp(Sender, Button, Shift, X, Y);

	if (Button != mbLeft) {
		return;
	}

	if (mouseInfo.isClick != true) {
		return;
	}

	TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(Sender);
	if (recordViewChart == NULL) {
		return;
	}

	if (recordViewChart->SeriesCount() <= 0) {
		return;
	}

	CreateVerticalLine(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyVerticalLine::ChartTextOnEnter(TObject *Sender) {
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyVerticalLine::ChartTextOnExit(TObject *Sender) {
}
