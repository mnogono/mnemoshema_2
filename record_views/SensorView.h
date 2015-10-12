//---------------------------------------------------------------------------
#ifndef SensorViewH
#define SensorViewH
//---------------------------------------------------------------------------
#include "record_view.h"
//---------------------------------------------------------------------------
/*
const int SENSOR_DISPLAY_NO_EVENT = 0;
const int SENSOR_DISPLAY_EVENT = 1;
*/
/* can be not only sensor view, maybe can be also device view */
class TSensorView : public TRecordView{
	protected:
		//TODO what is the status?
		int status;

		//background highlighted
		bool isHighligthed;

		//all vcl controls correspond with TSensorView (additional controls like labels)
		std::vector<TControl *> controls;

		//coordinate of mouse positions when user perform mouse down
		int mouseDownX, mouseDownY;

		//offset threshold
		bool offsetThresholdActive;
	public:
		//shape control to select (mark as active) main control vcl element
		TShape *viewControlSelectShape;

		static String SELECT_SHAPE_PREFIX;

		TSensor *sensor;

		//TSensorView(TControl *viewControl, TSensor *sensor);

		/*
		create new instance of TSensorView,
		parent parent element.

		try to find component under parent with name == sensor->uuid,
		if this element not found, create new TEdit, and several additional elements (like labels)
		*/
		TSensorView(TWinControl *parent, TSensor *sensor);

		inline __fastcall virtual ~TSensorView();

		virtual void SetUserSetting(TUser *user, int status) = 0;

		//virtual void DisplayAlarm(std::vector<TData *> *alarm) = 0;

		virtual void Highlight();

		virtual void UndoHighlight();

		//check is control highlighted
		virtual bool IsHighlighted();

		/*
		move component to coordinate
		(do not allow move coordinate out of parent border)
		*/
		virtual void MoveTo(int x, int y);

		/* offset component to coordinate, using start moving mouse coordinate */
		virtual void MouseOffset(int x, int y, int offsetThreshold = 0);

		/* shift component by specify offset coordanate */
		virtual void Offset(int offsetX, int offsetY);

		/* select all controls (display selected rectangle over all sb controls) */
		virtual void Select();

		/* unselect all controls (hide selected rectangle) */
		virtual void Unselect();

		//virtual void OnEnter(TObject *Sender);
		virtual void OnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		/*
		find first control by controlName in controls container
		return NULL, if no control find
		*/
		virtual TControl * GetControl(const String &controlName);

		virtual TControl * GetControl();

		/* create all controls, and push control element into controls container */
		virtual void CreateControls();

		virtual void Resize(int width, int height);
};

#endif
