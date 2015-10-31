//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "mouse_strategy.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//TMouseInfo mouseInfo;

//---------------------------------------------------------------------------
TMouseStrategy::TMouseStrategy(TPageControl *chartPageControl, TPopupMenu *popupMenu) {
	this->popupMenu = popupMenu;

	this->chartPageControl = chartPageControl;

	mouseInfo.isMouseDown = false;
	mouseInfo.isDblClick = false;
	mouseInfo.isClick = false;
	mouseInfo.lastDblClickTime = 0;
	mouseInfo.lastMouseMoveTime = 0;
	mouseInfo.lastMouseDownTime = 0;
	mouseInfo.lastMouseUpTime = 0;
}

//---------------------------------------------------------------------------
TMouseStrategy::~TMouseStrategy() {
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategy::OnChartMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled) {
	TChart *chart = dynamic_cast<TChart *>(Sender);
	if (!chart){
		return;
	}

	TScrollBox *scrollBox = dynamic_cast<TScrollBox *>(chart->Parent);
	if (!scrollBox){
		return;
	}

	scrollBox->VertScrollBar->Position -= Sign(WheelDelta) * 50;

	Handled = true;
}

// ---------------------------------------------------------------------------
/*
void __fastcall TMouseStrategy::OnChartClick(TObject *Sender) {
	TWinControl *winControl = dynamic_cast<TWinControl *>(Sender);
	if (!winControl){
		return;
	}

	winControl->SetFocus();
}
*/

//---------------------------------------------------------------------------
void __fastcall TMouseStrategy::OnChartEnter(TObject *Sender) {
	TRecordViewChart *recordViewChart = (TRecordViewChart *)Sender;
	recordViewChart->Highlight();
	/*
	TChart *chart = dynamic_cast<TChart *>(Sender);
	if (chart == NULL) {
		return;
	}

	chart->Border->Color = clBlack;
	*/
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategy::OnChartExit(TObject *Sender) {
	TRecordViewChart *recordViewChart = (TRecordViewChart *)Sender;
	recordViewChart->UndoHighlight();
	/*
	TChart *chart = dynamic_cast<TChart *>(Sender);
	if (chart == NULL) {
		return;
	}

	chart->Border->Color = chart->BackColor;
	*/
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategy::OnChartTrackerMove(TObject *Sender, int x, int y) {
	TRecordViewChart *recordViewChart = static_cast<TRecordViewChart *>(Sender);
	if (recordViewChart->chartTracker->IsVisible() == false) {
		return;
	}

	recordViewChart->chartTracker->Move(x, y);
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategy::OnChartMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	mouseInfo.button = Button;
	mouseInfo.x = X;
	mouseInfo.y = Y;
	mouseInfo.mx = X;
	mouseInfo.my = Y;
	mouseInfo.isMouseDown = true;
	mouseInfo.lastMouseDownTime = Now().Val;

	mouseInfo.trackable = FindTrackable(Sender, Shift, X, Y);
}

// ---------------------------------------------------------------------------
TChartTrackable * __fastcall TMouseStrategy::FindTrackable(TObject *Sender, TShiftState Shift, int X, int Y) {
	TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(Sender);
	if (recordViewChart == NULL) {
		return NULL;
	}

	double localTime1 = recordViewChart->GetTimeAxisMinValue();
	double localTime2 = recordViewChart->GetTimeAxisMaxValue();
	double onePercentRange = (localTime2 - localTime1) * 0.01;

	double localTime;
	double value;
	recordViewChart->Series[0]->GetCursorValues(localTime, value);
	double minDistance = localTime1;
	double distance;
	TChartTrackable *minTrackableDistance = NULL;

	std::list<TChartTrackable *> *trackableElements = recordViewChart->GetTrackableElements();
	for (std::list<TChartTrackable *>::iterator i = trackableElements->begin(), iEnd = trackableElements->end(); i != iEnd; ++i) {
		TChartTrackable *trackable = *i;
		if (trackable->IsVisible() == false) {
			continue;
		}

		distance = fabs(localTime - trackable->GetLocalTime());
		if (distance < minDistance && distance < onePercentRange) {
			distance = minDistance;

			minTrackableDistance = trackable;
		}
	}

	return minTrackableDistance;
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategy::OnChartMouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	//check is marker under mouse position
	//change the cursor




}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategy::OnChartMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	mouseInfo.isMouseDown = false;

	if ((Now().Val - mouseInfo.lastMouseUpTime) < 0.1 * sysTime::SEC2DAY) {
		mouseInfo.isDblClick = true;
	} else {
		mouseInfo.isDblClick = false;
	}

	mouseInfo.lastMouseUpTime = Now().Val;

	//check if event ChartMouseUpHand invoke for dbl click event
	//prevent dbl click event
	if ((Now().Val - mouseInfo.lastDblClickTime) < sysTime::SEC2DAY) {
		return;
	}

	if (abs(mouseInfo.x - X) < 3 && abs(mouseInfo.y - Y) < 3) {
		mouseInfo.isClick = true;
	} else {
		mouseInfo.isClick = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategy::OnMoveTrackableElement(TObject *Sender, TShiftState Shift, int X, int Y) {

}

// ---------------------------------------------------------------------------
int TMouseStrategy::GetObserverableType() {
	return 0;
}
