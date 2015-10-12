//---------------------------------------------------------------------------
#ifndef record_view_string_grid_containerH
#define record_view_string_grid_containerH

#include "record_view.h"
//---------------------------------------------------------------------------

/*
string grid container for display different record for different string grid row
string grid with 2 columns, (record title) and (sensor data value)
*/
class TRecordViewStringGridContainer : public TRecordView {
	public:
		int row;
	public:
		TRecordViewStringGridContainer(TStringGrid *stringGrid, TRecord *record, int row);

		virtual __fastcall ~TRecordViewStringGridContainer();

		virtual void MoveTo(int x, int y);

		virtual void CreateControls();

		virtual void DisplayData(TSensorData *data = NULL);

		virtual void DisplayData(std::list<TSensorData *> *data = NULL);

		virtual void Highlight();

		virtual void UndoHighlight();

		virtual bool IsHighlighted();

		virtual void SetPopupMenu(TPopupMenu *menu);
};

#endif
