//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "mouse_strategy_hand.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMouseStrategyHand::TMouseStrategyHand(TPageControl *chartPageControl, TPopupMenu *popupMenu) : TMouseStrategy(chartPageControl, popupMenu) {
}

//---------------------------------------------------------------------------
void TMouseStrategyHand::Apply(TChart *chart) {
	chart->OriginalCursor = crArrow;

	chart->BottomAxis->Automatic = false;
	chart->BottomAxis->AutomaticMinimum = false;
	chart->BottomAxis->AutomaticMaximum = false;

	chart->LeftAxis->Automatic = false;
	chart->LeftAxis->AutomaticMinimum = false;
	chart->LeftAxis->AutomaticMaximum = false;

	chart->ScrollMouseButton = mbLeft;
	chart->Zoom->Allow = false;
	chart->Zoom->Direction = tzdBoth;

	chart->OnScroll       = OnChartScroll;
	chart->OnDblClick     = OnChartDblClick;
	chart->OnMouseDown    = OnChartMouseDown;
	chart->OnMouseMove    = OnChartMouseMove;
	chart->OnMouseUp      = OnChartMouseUp;
	//chart->OnContextPopup = OnChartContextPopup;
	chart->OnMouseWheel   = OnChartMouseWheel;
	chart->OnEnter        = OnChartEnter;
	chart->OnExit         = OnChartExit;

	chart->PopupMenu = popupMenu;
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategyHand::SetPanningMode(TChart *chart) {
	TRecordViewChart *recordViewChart;
	if ((recordViewChart = dynamic_cast<TRecordViewChart *>(chart)) != NULL) {
		if (recordViewChart->GetRecord()->record_type == RECORD_TYPE_SENSOR_BIT) {
			chart->AllowPanning = pmHorizontal;
		} else {
			chart->AllowPanning = pmBoth;
		}
	}
}

// ---------------------------------------------------------------------------
void TMouseStrategyHand::Deactivate(TChart *chart) {
	chart->PopupMenu = NULL;
}

// ---------------------------------------------------------------------------
// TODO here problem for discrete data display 1 and 0...
void __fastcall TMouseStrategyHand::OnChartScroll(TObject *Sender) {
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategyHand::OnChartDblClick(TObject *Sender) {
	//last dbl click time introduce for prevent dbl click
	mouseInfo.lastDblClickTime = Now().Val;

	TSensorsTabSheet *activePage = static_cast<TSensorsTabSheet *>(chartPageControl->ActivePage);

	TSensorViewChart *chart = dynamic_cast<TSensorViewChart *>(Sender);
	if (chart != NULL) {
		double dt1LocalTime, dt2LocalTime;
		activePage->GetTimeAxisLocalTime(dt1LocalTime, dt2LocalTime);
		activePage->timeAxis->SetTimeAxisBounds(dt1LocalTime, dt2LocalTime);
		//activePage->timeAxis->SetTimeAxisBounds(chart->GetTimeAxisMinValue(), chart->GetTimeAxisMaxValue());
	}

	//enable traking for all changes of active tab
	activePage->SetAutoTracking(true);
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategyHand::OnChartMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TMouseStrategy::OnChartMouseDown(Sender, Button, Shift, X, Y);

	if (mouseInfo.trackable != NULL) {
		static_cast<TChart *>(Sender)->AllowPanning = TPanningMode::pmNone;
	} else {
    	SetPanningMode(static_cast<TChart *>(Sender));
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategyHand::OnChartTimeAxisMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	TSensorsTabSheet *activePage = static_cast<TSensorsTabSheet *>(chartPageControl->ActivePage);

	if (!mouseInfo.isMouseDown) {
		return;
	}

	if (mouseInfo.button != mbLeft) {
		return;
	}

	if (mouseInfo.x == X && mouseInfo.y == Y) {
		return;
	}

	if (mouseInfo.mx == X && mouseInfo.my == Y) {
		return;
	}

	mouseInfo.mx = X;
	mouseInfo.my = Y;

	//trick to resolve the problem when user try to drag not a first chart in scroll box, for some reason it very slow.
	if ((Now().Val - mouseInfo.lastMouseMoveTime) < 0.1 * sysTime::SEC2DAY) {
		return;
	}
	mouseInfo.lastMouseMoveTime = Now().Val;

	TChart *chart = static_cast<TChart *>(Sender);
	activePage->timeAxis->SetTimeAxisBounds(chart->BottomAxis->Minimum, chart->BottomAxis->Maximum);

	//disable traking for all changes of active tab
	activePage->SetAutoTracking(false);
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategyHand::OnMoveTracker(TObject *Sender, TShiftState Shift, int X, int Y) {
	TSensorsTabSheet *activePage = static_cast<TSensorsTabSheet *>(chartPageControl->ActivePage);

	//move tracker for sensor msh viewer
	for (std::map<const TSensor *, IMnemoshemaView *>::iterator it = activePage->mshViews.begin(); it != activePage->mshViews.end(); ++it) {
		IMnemoshemaView *mshView = it->second;
		TRecordViewChart *viewChart;
		if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
			OnChartTrackerMove(viewChart, X, Y);
		}
	}

	//move tracker for sensor bit msh viewer
	for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator it = activePage->mshSensorBitViews.begin(); it != activePage->mshSensorBitViews.end(); ++it) {
		IMnemoshemaView *mshView = it->second;
		TRecordViewChart *viewChart;
		if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
			OnChartTrackerMove(viewChart, X, Y);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategyHand::OnChartMouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	TChartTrackable *trackable = FindTrackable(Sender, Shift, X, Y);

	TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(Sender);
	if (recordViewChart == NULL) {
		return;
	}

	if (recordViewChart->chartTracker->IsVisible() == true) {
		OnMoveTracker(Sender, Shift, X, Y);

		Screen->Cursor = crArrow;

	} else if (trackable != NULL && mouseInfo.isMouseDown == false) {
		Screen->Cursor = crHSplit;

	} else if (mouseInfo.isMouseDown == true && mouseInfo.button == mbLeft) {
		if (mouseInfo.trackable == NULL) {
			OnChartTimeAxisMove(Sender, Shift, X, Y);

			Screen->Cursor = crSizeAll;
		} else {
			OnMoveTrackableElement(Sender, Shift, X, Y);

			Screen->Cursor = crHSplit;
        }
	} else {
        Screen->Cursor = crArrow;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategyHand::OnChartMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TMouseStrategy::OnChartMouseUp(Sender, Button, Shift, X, Y);
	if (mouseInfo.isDblClick == true) {
		return;
	}

	TChart *chart = static_cast<TChart *>(Sender);

	if (mouseInfo.trackable != NULL) {
		if (mouseInfo.button == mbLeft) {
			double x;

			TChart *chart = static_cast<TChart *>(Sender);
			double localTime = chart->BottomAxis->CalcPosPoint(X);

			mouseInfo.trackable->SetLocalTime(localTime);
			mouseInfo.trackable->SetValue(0);
		}
	} else {
		TSensorsTabSheet *activePage = static_cast<TSensorsTabSheet *>(chartPageControl->ActivePage);
		activePage->timeAxis->SetTimeAxisBounds(chart->BottomAxis->Minimum, chart->BottomAxis->Maximum, true);
	}

	mouseInfo.button = mbMiddle;
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategyHand::OnChartContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled) {
	if (mouseInfo.button == mbRight){
		/*
		if (mouseInfo.pointClick.PtInCircle(MousePos, 2)){
			Handled = false;
			return;
		}
		*/
	}
	Handled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TMouseStrategyHand::OnMoveTrackableElement(TObject *Sender, TShiftState Shift, int X, int Y) {
	if (mouseInfo.isMouseDown == false) {
		return;
	}

	if (mouseInfo.button != mbLeft) {
		return;
	}

	if (mouseInfo.trackable == NULL) {
		return;
	}

	mouseInfo.trackable->Move(X, Y);

	if (mouseInfo.trackable->GetType() == trackable::TTrackableType::VERTICAL_LINE) {
		TChartVerticalLine *currentVerticalLine = static_cast<TChartVerticalLine *>(mouseInfo.trackable);

		TRecordViewChart *recordViewChart = static_cast<TRecordViewChart *>(Sender);
		double localTime = recordViewChart->BottomAxis->CalcPosPoint(X);

		if (currentVerticalLine->IsSuperline()) {
			//find other vertical lines and move it also

			TSensorsTabSheet *tabSheet;
			if ((tabSheet = dynamic_cast<TSensorsTabSheet *>(chartPageControl->ActivePage)) != NULL) {
				TChartVerticalLine *verticalLine;
				for (std::map<const TSensor *, IMnemoshemaView *>::iterator it = tabSheet->mshViews.begin(); it != tabSheet->mshViews.end(); ++it) {
					IMnemoshemaView *mshView = it->second;
					TRecordViewChart *viewChart;
					if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
						//find vertical line with the same time like '
						std::list<TChartTrackable *> *trackableElements = viewChart->GetTrackableElements();
						for (std::list<TChartTrackable *>::iterator t = trackableElements->begin(), tEnd = trackableElements->end(); t != tEnd; ++t) {
							if ((*t)->GetType() != trackable::TTrackableType::VERTICAL_LINE) {
								continue;
							}

							verticalLine = static_cast<TChartVerticalLine *>(*t);
							if (verticalLine == currentVerticalLine) {
								continue;
							}

							if (fabs(verticalLine->GetLocalTime() - currentVerticalLine->GetLocalTime()) <= sysTime::SEC2DAY) {
								verticalLine->Move(X, Y);
								verticalLine->SetLocalTime(localTime);
								verticalLine->SetCaption("", localTime, viewChart->GetSeriesValues(localTime));
							}
						}
					}
				}

				//move msh sensor bit view
				for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator it = tabSheet->mshSensorBitViews.begin(); it != tabSheet->mshSensorBitViews.end(); ++it) {
					IMnemoshemaView *mshView = it->second;
					TRecordViewChart *viewChart;
					if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
						//find vertical line with the same time like '
						std::list<TChartTrackable *> *trackableElements = viewChart->GetTrackableElements();
						for (std::list<TChartTrackable *>::iterator t = trackableElements->begin(), tEnd = trackableElements->end(); t != tEnd; ++t) {
							if ((*t)->GetType() != trackable::TTrackableType::VERTICAL_LINE) {
								continue;
							}

							verticalLine = static_cast<TChartVerticalLine *>(*t);
							if (verticalLine == currentVerticalLine) {
								continue;
							}

							if (fabs(verticalLine->GetLocalTime() - currentVerticalLine->GetLocalTime()) <= sysTime::SEC2DAY) {
								verticalLine->Move(X, Y);
								verticalLine->SetLocalTime(localTime);
								verticalLine->SetCaption("", localTime, viewChart->GetSeriesValues(localTime));
							}
						}
					}
				}
			}
		}

		currentVerticalLine->SetLocalTime(localTime);

		currentVerticalLine->SetCaption("", localTime, recordViewChart->GetSeriesValues(localTime));
	}
}
