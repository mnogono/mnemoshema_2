//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "mouse_strategy_super_vertical_line.h"
#include "sensors_tab_sheet.h"
#include "record_view_chart.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
int TEventSuperVerticalLineCreated::GetObserverableType() {
	return TObserverableTypes::NEW_SUPER_VERTICAL_LINE;
}

//---------------------------------------------------------------------------
void TMouseStrategySuperVerticalLine::Apply(TChart *chart) {
	//iterate over all charts in page control and enable vertical line
	TSensorsTabSheet *tabSheet;
	if ((tabSheet = dynamic_cast<TSensorsTabSheet *>(chartPageControl->ActivePage)) == NULL) {
		return;
	}

	for (std::map<const TSensor *, IMnemoshemaView *>::iterator it = tabSheet->mshViews.begin(); it != tabSheet->mshViews.end(); ++it) {
		IMnemoshemaView *mshView = it->second;
		TRecordViewChart *viewChart;
		if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
			TMouseStrategyVerticalLine::Apply(viewChart);
        }
	}

	for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator it = tabSheet->mshSensorBitViews.begin(); it != tabSheet->mshSensorBitViews.end(); ++it) {
		IMnemoshemaView *mshView = it->second;
		TRecordViewChart *viewChart;
		if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
			TMouseStrategyVerticalLine::Apply(viewChart);
        }
	}
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategySuperVerticalLine::OnChartMouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	TSensorsTabSheet *tabSheet;
	if ((tabSheet = dynamic_cast<TSensorsTabSheet *>(chartPageControl->ActivePage)) == NULL) {
		return;
	}

	//move sensor view msh
	for (std::map<const TSensor *, IMnemoshemaView *>::iterator it = tabSheet->mshViews.begin(); it != tabSheet->mshViews.end(); ++it) {
		IMnemoshemaView *mshView = it->second;
		TRecordViewChart *viewChart;
		if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
			TMouseStrategyVerticalLine::OnChartMouseMove(viewChart, Shift, X, Y);
        }
	}

	//move sensor bit vide msh
	for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator it = tabSheet->mshSensorBitViews.begin(); it != tabSheet->mshSensorBitViews.end(); ++it) {
		IMnemoshemaView *mshView = it->second;
		TRecordViewChart *viewChart;
		if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
			TMouseStrategyVerticalLine::OnChartMouseMove(viewChart, Shift, X, Y);
        }
	}
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategySuperVerticalLine::OnChartMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TSensorsTabSheet *tabSheet;
	if ((tabSheet = dynamic_cast<TSensorsTabSheet *>(chartPageControl->ActivePage)) == NULL) {
		return;
	}

	for (std::map<const TSensor *, IMnemoshemaView *>::iterator it = tabSheet->mshViews.begin(); it != tabSheet->mshViews.end(); ++it) {
		IMnemoshemaView *mshView = it->second;
		TRecordViewChart *viewChart;
		if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
			TMouseStrategyVerticalLine::OnChartMouseUp(viewChart, Button, Shift, X, Y);
        }
	}

	for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator it = tabSheet->mshSensorBitViews.begin(); it != tabSheet->mshSensorBitViews.end(); ++it) {
		IMnemoshemaView *mshView = it->second;
		TRecordViewChart *viewChart;
		if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
			TMouseStrategyVerticalLine::OnChartMouseUp(viewChart, Button, Shift, X, Y);
        }
	}

	TEventSuperVerticalLineCreated event;
	NotifyObservers(&event);
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategySuperVerticalLine::CreateVerticalLine(TObject *Sender) {
	TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(Sender);
	if (recordViewChart == NULL) {
		return;
	}

	TChartVerticalLine *verticalLine = new TChartVerticalLine(*recordViewChart->chartVerticalLine);
	verticalLine->MarkAsSuperline(true);

	recordViewChart->AddTrackableElement(
		verticalLine
	);
}
