//---------------------------------------------------------------------------
#ifndef mouse_strategy_zoomH
#define mouse_strategy_zoomH

#include "mouse_strategy.h"
#include "sensors_tab_sheet.h"
//---------------------------------------------------------------------------
class TMouseStrategyZoom : public TMouseStrategy {
	public:
    	TMouseStrategyZoom(TPageControl *chartPageControl, TPopupMenu *popupMenu);

		virtual void Apply(TChart *chart);

		virtual void Deactivate(TChart *chart);

		virtual void __fastcall OnChartZoom(TObject *Sender);

		virtual void __fastcall OnChartDblClickZoom(TObject *Sender);
};

#endif
