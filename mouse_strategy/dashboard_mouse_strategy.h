//---------------------------------------------------------------------------
#ifndef dashboard_mouse_strategyH
#define dashboard_mouse_strategyH
//---------------------------------------------------------------------------

//TODO maybe more precy change the name to TImageMouseStrategy
class TDashboardMouseStrategy {
	public:
	private:
		// mouse button -1 default no button, else TMouseButton
		int button;

		//mouse coordinate x
		int mouseDownX;

		//mouse coordinate y
		int mouseDownY;

		//horizontal scroll position of parent element
		int horizontalScrollPosition;

		//vertical scroll position of parent element
		int verticalScrollPosition;

		//parent scroll box component
		TScrollBox *scrollBox;
	public:

		TDashboardMouseStrategy();

		void __fastcall OnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		void __fastcall OnMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

		void __fastcall OnMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
};

#endif
