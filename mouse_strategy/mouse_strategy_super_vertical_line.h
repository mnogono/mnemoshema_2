//---------------------------------------------------------------------------
#ifndef mouse_strategy_super_vertical_lineH
#define mouse_strategy_super_vertical_lineH

#include "mouse_strategy_vertical_line.h"

//---------------------------------------------------------------------------
class TEventSuperVerticalLineCreated : public sysObserverable::TObserverable {
	public:
		virtual int GetObserverableType();
};

//---------------------------------------------------------------------------
class TMouseStrategySuperVerticalLine : public TMouseStrategyVerticalLine {
	public:
		TMouseStrategySuperVerticalLine(TPageControl *chartPageControl, TPopupMenu *popupMenu) : TMouseStrategyVerticalLine (chartPageControl, popupMenu) {};

		virtual void TMouseStrategySuperVerticalLine::Apply(TChart *chart);

		virtual void __fastcall OnChartMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

		virtual void __fastcall OnChartMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		virtual void __fastcall CreateVerticalLine(TObject *Sender);
};

#endif
