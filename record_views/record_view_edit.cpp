//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "record_view_edit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TRecordViewEdit::TRecordViewEdit(TWinControl *parent, TRecord *record) : TRecordView(parent, record) {
}

//---------------------------------------------------------------------------
void TRecordViewEdit::MoveTo(int x, int y) {
}

//---------------------------------------------------------------------------
void TRecordViewEdit::Highlight() {
}

//---------------------------------------------------------------------------
void TRecordViewEdit::UndoHighlight() {
}

bool TRecordViewEdit::IsHighlighted() {
	return false;
}

//---------------------------------------------------------------------------
void TRecordViewEdit::DisplayText(const String *data) {
	if (!viewControl) {
		return;
	}

	if (data == NULL) {
		static_cast<TEdit *>(viewControl)->Text = "Í/Ä";
	} else {
		static_cast<TEdit *>(viewControl)->Text = *data;
	}
}

//---------------------------------------------------------------------------
void TRecordViewEdit::CreateControls() {
	TRecordView::CreateControls();

	if (viewControl == NULL) {
		viewControl = new TEdit(parent);
		viewControl->Name = record->uuid;
		viewControl->Parent = parent;
	}

	leftLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_LEFT_PREFIX + record->uuid);
	if (!leftLabel) {
		leftLabel = new TLabel(parent);
		leftLabel->Parent = parent;
		leftLabel->Name = LABEL_LEFT_PREFIX + record->uuid;
		leftLabel->Caption = record->GetName();
		leftLabel->AutoSize = true;
		leftLabel->Transparent = true;
		leftLabel->Alignment = taRightJustify;
	}

	rightLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_RIGHT_PREFIX + record->uuid);
	if (!rightLabel) {
		rightLabel = new TLabel(parent);
		rightLabel->Parent = parent;
		rightLabel->Name = LABEL_RIGHT_PREFIX + record->uuid;
		rightLabel->Caption = "";
		rightLabel->AutoSize = true;
		rightLabel->Transparent = true;
		rightLabel->Alignment = taLeftJustify;
	}

	unitLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_UNIT_PREFIX + record->uuid);
	if (!unitLabel) {
		unitLabel = new TLabel(parent);
		unitLabel->Parent = parent;
		unitLabel->Name = LABEL_UNIT_PREFIX + record->uuid;
		unitLabel->Caption = "";
		unitLabel->AutoSize = true;
		unitLabel->Transparent = true;
		unitLabel->Width = LABEL_UNIT_WIDTH;
	}

	controls.insert(viewControl);

	controls.insert(leftLabel);

	controls.insert(rightLabel);

	controls.insert(unitLabel);
}

//---------------------------------------------------------------------------
void TRecordViewEdit::SetPopupMenu(TPopupMenu *menu) {
	static_cast<TEdit *>(viewControl)->PopupMenu = menu;
}

//---------------------------------------------------------------------------
void TRecordViewEdit::UpdateValue() {
}
