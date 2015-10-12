//---------------------------------------------------------------------------
#ifndef mouse_strategy_textH
#define mouse_strategy_textH

#include "mouse_strategy.h"
#include "chart_text.h"
#include "mouse_strategy_hand.h"

//---------------------------------------------------------------------------
class TEventTextMarkerCreated : public sysObserverable::TObserverable {
	public:
		virtual int GetObserverableType();
};

//---------------------------------------------------------------------------
class TMouseStrategyText : public TMouseStrategy {
	public:
		TMouseStrategyText(TPageControl *chartPageControl, TPopupMenu *popupMenu);

		virtual void Apply(TChart *chart);

		virtual void Deactivate(TChart *chart);

		virtual void __fastcall OnChartScroll(TObject *Sender);

		virtual void __fastcall OnChartMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		virtual void __fastcall OnChartMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

		virtual void __fastcall OnChartMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		void __fastcall ChartTextOnEnter(TObject *Sender);

		void __fastcall ChartTextOnExit(TObject *Sender);
};

#endif
