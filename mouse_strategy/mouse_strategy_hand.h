//---------------------------------------------------------------------------
#ifndef mouse_strategy_handH
#define mouse_strategy_handH

#include "mouse_strategy.h"
#include "sensors_tab_sheet.h"
#include "record_view_chart.h"
//---------------------------------------------------------------------------
class TMouseStrategyHand : public TMouseStrategy {
	public:
    	TMouseStrategyHand(TPageControl *chartPageControl, TPopupMenu *popupMenu);

		virtual void Apply(TChart *chart);

		virtual void Deactivate(TChart *chart);

		void __fastcall OnChartScroll(TObject *Sender);

		void __fastcall OnChartDblClick(TObject *Sender);

		void __fastcall OnChartMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		void __fastcall OnChartMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

		void __fastcall OnChartMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		void __fastcall OnChartContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled);

		void __fastcall OnMoveTracker(TObject *Sender, TShiftState Shift, int X, int Y);

		void __fastcall OnChartTimeAxisMove(TObject *Sender, TShiftState Shift, int X, int Y);

		void __fastcall SetPanningMode(TChart *chart);

		virtual void __fastcall OnMoveTrackableElement(TObject *Sender, TShiftState Shift, int X, int Y);
};

#endif
