//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "mouse_strategy_zoom.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMouseStrategyZoom::TMouseStrategyZoom(TPageControl *chartPageControl, TPopupMenu *popupMenu) : TMouseStrategy(chartPageControl, popupMenu) {}

//---------------------------------------------------------------------------
void TMouseStrategyZoom::Apply(TChart *chart) {
	chart->OriginalCursor = crCross;

	chart->Zoom->Allow = true;
	chart->Zoom->MouseButton = mbLeft;
	chart->Zoom->Pen->Color = clBlack;
	chart->Zoom->Pen->Style = psDash;
	chart->Zoom->Pen->Width = 2;
	chart->Zoom->UpLeftZooms = true;
	chart->Zoom->History = true;
	chart->Zoom->Animated = false;
	chart->Zoom->AnimatedSteps = 1;
	chart->Zoom->Direction = tzdHorizontal;
	chart->ScrollMouseButton = NULL;

	chart->OnMouseWheel   = OnChartMouseWheel;
	chart->OnEnter        = OnChartEnter;
	chart->OnExit         = OnChartExit;
	chart->OnZoom         = OnChartZoom;
	chart->OnDblClick     = OnChartDblClickZoom;
	chart->OnMouseDown    = NULL;
	chart->OnMouseUp      = NULL;
	chart->OnMouseMove    = NULL;
}

//---------------------------------------------------------------------------
void TMouseStrategyZoom::Deactivate(TChart *chart) {}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyZoom::OnChartZoom(TObject *Sender) {
	TChart *chart = static_cast<TChart *>(Sender);

	TSensorsTabSheet *activePage = static_cast<TSensorsTabSheet *>(chartPageControl->ActivePage);

	activePage->timeAxis->SetTimeAxisBounds(chart->BottomAxis->Minimum, chart->BottomAxis->Maximum, true);
}

//---------------------------------------------------------------------------
void __fastcall TMouseStrategyZoom::OnChartDblClickZoom(TObject *Sender) {

}
