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
		/*
		viewControl->Left = 300;
		viewControl->Top = 300;
		*/
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
//		leftLabel->Left = viewControl->Left - leftLabel->Width - 5;
//		leftLabel->Top = viewControl->Top + (viewControl->Height - leftLabel->Height) / 2;
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
//		rightLabel->Left = viewControl->Left + LABEL_UNIT_WIDTH + viewControl->Width + 5;
//		rightLabel->Top = viewControl->Top + (viewControl->Height - rightLabel->Height) / 2;
	}

	unitLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_UNIT_PREFIX + record->uuid);
	if (!unitLabel) {
		unitLabel = new TLabel(parent);
		unitLabel->Parent = parent;
		unitLabel->Name = LABEL_UNIT_PREFIX + record->uuid;
		unitLabel->Caption = "";
		unitLabel->AutoSize = true;
		unitLabel->Transparent = true;
//		unitLabel->Left = viewControl->Left + viewControl->Width + 5;
//		unitLabel->Top = viewControl->Top + (viewControl->Height - unitLabel->Height) / 2;
		unitLabel->Width = LABEL_UNIT_WIDTH;
	}

	controls.insert(viewControl);

	/*
	if (std::find(controls.begin(), controls.end(), leftLabel) == controls.end()) {
		controls.push_back(leftLabel);
	}
	*/
	controls.insert(leftLabel);

	/*
	if (std::find(controls.begin(), controls.end(), rightLabel) == controls.end()) {
		controls.push_back(rightLabel);
	}
	*/
	controls.insert(rightLabel);

	/*
	if (std::find(controls.begin(), controls.end(), unitLabel) == controls.end()) {
		controls.push_back(unitLabel);
	}
	*/
	controls.insert(unitLabel);
}

//---------------------------------------------------------------------------
void TRecordViewEdit::SetPopupMenu(TPopupMenu *menu) {
	static_cast<TEdit *>(viewControl)->PopupMenu = menu;
}

//---------------------------------------------------------------------------
void TRecordViewEdit::UpdateValue() {
}
