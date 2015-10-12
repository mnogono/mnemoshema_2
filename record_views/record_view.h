//---------------------------------------------------------------------------
#ifndef record_viewH
#define record_viewH

#include "mnemoshema_view.h"
//---------------------------------------------------------------------------

//disable warning about all interface should be inherted from IUnknown
#pragma option push -w-8130

class TRecordView :
	public IMnemoshemaView {
	public:
		static String SELECT_SHAPE_PREFIX;

		static String LABEL_LEFT_PREFIX;

		static String LABEL_RIGHT_PREFIX;

		static String LABEL_UNIT_PREFIX;

		static int LABEL_UNIT_WIDTH;

	protected:
		const TRecord * record;

		//vcl parent control
		TWinControl *parent;

		//record view control
		TControl *viewControl;

		//all vcl controls correspond with TSensorView (additional controls like labels)
		//std::list<TControl *> controls;
		std::set<TControl *> controls;

		//shape control to select (mark as active) main control vcl element
		TShape *viewControlSelectShape;

	public:
		TRecordView(TWinControl *parent, TRecord *record);

		inline __fastcall virtual ~TRecordView();

		virtual const TRecord * GetRecord();

		void CreateControls();

		TLabel * CreateLeftLabel(TWinControl *parent, const TRecord *record);

		TLabel * CreateRightLabel(TWinControl *parent, const TRecord *record);

		virtual TControl * GetControl();

		virtual void UpdateValue();
};

#pragma option pop

#endif
