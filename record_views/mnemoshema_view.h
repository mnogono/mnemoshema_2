//---------------------------------------------------------------------------
#ifndef mnemoshema_viewH
#define mnemoshema_viewH
//---------------------------------------------------------------------------

class IMnemoshemaView {
	public:
		virtual void MoveTo(int x, int y) = 0;

		virtual void CreateControls() = 0;

		//display sensor data (update online data)
		virtual void DisplayData(TSensorData *data = NULL) = 0;

		//display sensor data (update offline data)
		virtual void DisplayData(std::list<TSensorData *> *data = NULL) = 0;

		virtual void Highlight() = 0;

		virtual void UndoHighlight() = 0;

		virtual bool IsHighlighted() = 0;

		virtual const TRecord * GetRecord() = 0;

		//set popup menu for mnemoshema control
		virtual void SetPopupMenu(TPopupMenu *menu) = 0;

		//return view control
		virtual TControl *GetControl() = 0;

		//update mnemosheva view value (update chart, update text editor, update img and so on... using internal data container)
		virtual void UpdateValue() = 0;

		//virtual void SetAutoAdjust(bool autoAdjust) = 0;

		//change left axis min max using scale coefficient
		//virtual void ChangeMinMaxLeftAxis(double coefficient) = 0;
};

#endif
