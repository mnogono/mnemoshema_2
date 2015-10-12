//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "dashboard_mouse_strategy.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TDashboardMouseStrategy::TDashboardMouseStrategy() : button(-1) {
}

//---------------------------------------------------------------------------
void __fastcall TDashboardMouseStrategy::OnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	scrollBox = NULL;

	TImage *image = dynamic_cast<TImage *>(Sender);
	if (image == NULL) {
		return;
	}

	scrollBox = dynamic_cast<TScrollBox *>(image->Parent);
	if (scrollBox == NULL) {
		return;
	}

	button = Button;
	mouseDownX = Mouse->CursorPos.X;
	mouseDownY = Mouse->CursorPos.Y;

	horizontalScrollPosition = scrollBox->HorzScrollBar->Position;
	verticalScrollPosition = scrollBox->VertScrollBar->ScrollPos;
}

//---------------------------------------------------------------------------
void __fastcall TDashboardMouseStrategy::OnMouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	if (scrollBox == NULL) {
		return;
	}

	if (button == TMouseButton::mbLeft) {
		scrollBox->HorzScrollBar->Position = horizontalScrollPosition + 2 * (- Mouse->CursorPos.X + mouseDownX);
		scrollBox->VertScrollBar->Position = verticalScrollPosition + 2 * (- Mouse->CursorPos.Y + mouseDownY);
		scrollBox->Refresh();
	}
}

//---------------------------------------------------------------------------
void __fastcall TDashboardMouseStrategy::OnMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	button = -1;
}

