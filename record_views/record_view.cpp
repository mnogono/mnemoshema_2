//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "record_view.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

String TRecordView::SELECT_SHAPE_PREFIX = "sel_shape_";

String TRecordView::LABEL_LEFT_PREFIX = "lb_left_";

String TRecordView::LABEL_RIGHT_PREFIX = "lb_right_";

String TRecordView::LABEL_UNIT_PREFIX = "lb_unit_";

int TRecordView::LABEL_UNIT_WIDTH = 30;


TRecordView::TRecordView(TWinControl *owner, TRecord *record) {
	viewControl = NULL;

	this->parent = owner;

	this->record = record;
}

//---------------------------------------------------------------------------
inline __fastcall TRecordView::~TRecordView() {
}

//---------------------------------------------------------------------------
const TRecord * TRecordView::GetRecord() {
	return record;
}

// ---------------------------------------------------------------------------
void TRecordView::CreateControls() {
	viewControl = sysVCL::FindControl(parent, record->uuid);

	viewControlSelectShape = (TShape *)sysVCL::FindControl(parent, SELECT_SHAPE_PREFIX + record->uuid);
	if (!viewControlSelectShape) {
		viewControlSelectShape = new TShape(parent);
		viewControlSelectShape->Name = SELECT_SHAPE_PREFIX + record->uuid;
		viewControlSelectShape->Parent = parent;
		viewControlSelectShape->Brush->Style = bsClear;
		viewControlSelectShape->Pen->Width = 3;
		viewControlSelectShape->Pen->Color = clBlack;
		viewControlSelectShape->Visible = false;
	}

	if (viewControl != NULL) {
		controls.insert(viewControl);
	}

	controls.insert(viewControlSelectShape);

	sysLogger::TRACE_A("Controls size: ");
	sysLogger::TRACE_W(IntToStr((int)controls.size()).c_str());
	sysLogger::TRACE_A("record->uuid: ");
	sysLogger::TRACE_W(record->uuid.c_str());
}

// ---------------------------------------------------------------------------
TLabel * TRecordView::CreateLeftLabel(TWinControl *parent, const TRecord *record) {
	TLabel *leftLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_LEFT_PREFIX + record->uuid);
	if (!leftLabel) {
		leftLabel = new TLabel(parent);
		leftLabel->Parent = parent;
		leftLabel->Name = LABEL_LEFT_PREFIX + record->uuid;
		leftLabel->Caption = record->GetName();
		leftLabel->AutoSize = true;
		leftLabel->Transparent = true;
		leftLabel->Alignment = taRightJustify;
		leftLabel->Left = viewControl->Left - leftLabel->Width - 5;
		leftLabel->Top = viewControl->Top + (viewControl->Height - leftLabel->Height) / 2;
	}

	return leftLabel;
}

// ---------------------------------------------------------------------------
TLabel * TRecordView::CreateRightLabel(TWinControl *parent, const TRecord *record) {
	TLabel *rightLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_RIGHT_PREFIX + record->uuid);
	if (!rightLabel) {
		rightLabel = new TLabel(parent);
		rightLabel->Parent = parent;
		rightLabel->Name = LABEL_RIGHT_PREFIX + record->uuid;
		rightLabel->Caption = "";
		rightLabel->AutoSize = true;
		rightLabel->Transparent = true;
		rightLabel->Alignment = taLeftJustify;
		rightLabel->Left = viewControl->Left + viewControl->Width + 5;
		rightLabel->Top = viewControl->Top + (viewControl->Height - rightLabel->Height) / 2;
	}

	return rightLabel;
}

// ---------------------------------------------------------------------------
TControl * TRecordView::GetControl() {
	return viewControl;
}

// ---------------------------------------------------------------------------
void TRecordView::UpdateValue() {
}
