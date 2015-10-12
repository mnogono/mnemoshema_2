//---------------------------------------------------------------------------
#ifndef mouse_strategy_vertical_lineH
#define mouse_strategy_vertical_lineH

#include "chart_vertical_line.h"
#include "mouse_strategy.h"

//---------------------------------------------------------------------------
class TEventVerticalLineCreated : public sysObserverable::TObserverable {
	public:
		virtual int GetObserverableType();
};


//---------------------------------------------------------------------------
class TMouseStrategyVerticalLine : public TMouseStrategy {
	private:
		TPopupMenu *popupMenu;
	public:
		TMouseStrategyVerticalLine(TPageControl *chartPageControl, TPopupMenu *popupMenu);

		virtual void Apply(TChart *chart);

		virtual void Deactivate(TChart *chart);

		virtual void __fastcall OnChartScroll(TObject *Sender);

		virtual void __fastcall OnChartMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		virtual void __fastcall OnChartMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

		virtual void __fastcall OnChartMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		void __fastcall ChartTextOnEnter(TObject *Sender);

		void __fastcall ChartTextOnExit(TObject *Sender);

		virtual void __fastcall CreateVerticalLine(TObject *Sender);
};

#endif
