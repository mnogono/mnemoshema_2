//---------------------------------------------------------------------------
#ifndef mouse_strategyH
#define mouse_strategyH

#include "record_view_chart.h"
//---------------------------------------------------------------------------

struct TMouseInfo {
	bool isMouseDown;
	TMouseButton button;

	//mouse down position
	int x;
	int y;

	//mouse move position
	int mx;
	int my;

	//prevent invoke mouse up event after the dbl click event
	double lastDblClickTime;

	//prevent invoke move event too fast
	double lastMouseMoveTime;

	double lastMouseDownTime;

	double lastMouseUpTime;

	//flag describe is double click invoked
	bool isDblClick;

	//flag is was left mouse button click
	bool isClick;

	//current trackable element
	TChartTrackable *trackable;
};

//TODO rename to TChartMouseStrategy
class TMouseStrategy : public sysObserverable::TObserverable {
	public:
		TMouseInfo mouseInfo;
	protected:
		//chart page control for getting current page
		TPageControl *chartPageControl;

		TPopupMenu *popupMenu;
	public:
		TMouseStrategy(TPageControl *chartPageControl, TPopupMenu *popupMenu);

		virtual ~TMouseStrategy();

		//apply mouse strategy for the chart
		virtual void Apply(TChart *chart) = 0;

		//disactivate mouse strategy
		virtual void Deactivate(TChart *chart) = 0;

		//scroll vertical scroll bar (charts container)
		virtual void __fastcall OnChartMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled);

		//virtual void __fastcall OnChartClick(TObject *Sender);
		//chart get focus
		virtual void __fastcall OnChartEnter(TObject *Sender);

		//chart lose focus
		virtual void __fastcall OnChartExit(TObject *Sender);

		virtual void __fastcall OnChartTrackerMove(TObject *Sender, int x, int y);

		virtual void __fastcall OnChartMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		virtual void __fastcall OnChartMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

		virtual void __fastcall OnChartMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		//find trackable element under X chart position or return NULL
		virtual TChartTrackable * __fastcall FindTrackable(TObject *Sender, TShiftState Shift, int X, int Y);

		//move trackable element when user moused doen on it and move
		virtual void __fastcall OnMoveTrackableElement(TObject *Sender, TShiftState Shift, int X, int Y);

		virtual int GetObserverableType();
};
#endif
