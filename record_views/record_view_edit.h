//---------------------------------------------------------------------------
#ifndef record_view_editH
#define record_view_editH

#include "record_view.h"
//---------------------------------------------------------------------------

class TRecordViewEdit : public TRecordView {
	protected:
		TLabel *leftLabel;

		TLabel *rightLabel;

		TLabel *unitLabel;

	public:
		TRecordViewEdit(TWinControl *parent, TRecord *record);

		//data can be NULL if no data to display
		virtual void DisplayText(const String *data = NULL);

		virtual void MoveTo(int x, int y);

		virtual void Highlight();

		virtual void UndoHighlight();

		virtual bool IsHighlighted();

		virtual void CreateControls();

		virtual void SetPopupMenu(TPopupMenu *menu);

		virtual void UpdateValue();
};


#endif
