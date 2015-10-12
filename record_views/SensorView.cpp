//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "SensorView.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
TSensorView::TSensorView(TWinControl *parent, TSensor *sensor) : TRecordView(parent, sensor) {
	sysLogger::TRACE_A("Creation new sensor view sensor uuid: ");
	sysLogger::TRACE_W(sensor->uuid.c_str());

	this->sensor = sensor;

	status = -1;

	isHighligthed = false;

	offsetThresholdActive = false;

	CreateControls();
}

// ---------------------------------------------------------------------------
void TSensorView::CreateControls() {
	viewControl = sysVCL::FindControl(parent, sensor->uuid);

	static String SELECT_SHAPE_PREFIX = "sel_shape_";

	viewControlSelectShape = (TShape *)sysVCL::FindControl(parent, SELECT_SHAPE_PREFIX + sensor->uuid);
	if (!viewControlSelectShape) {
		viewControlSelectShape = new TShape(parent);
		viewControlSelectShape->Name = SELECT_SHAPE_PREFIX + sensor->uuid;
		viewControlSelectShape->Parent = parent;
		viewControlSelectShape->Brush->Style = bsClear;
		viewControlSelectShape->Pen->Width = 3;
		viewControlSelectShape->Pen->Color = clBlack;
		viewControlSelectShape->Visible = false;
	}

	if (viewControl) {
		if (std::find(controls.begin(), controls.end(), viewControl) == controls.end()) {
			controls.push_back(viewControl);
		}
	}

	if (std::find(controls.begin(), controls.end(), viewControlSelectShape) == controls.end()) {
		controls.push_back(viewControlSelectShape);
	}

	sysLogger::TRACE_A("Controls size: ");
	sysLogger::TRACE_W(IntToStr((int)controls.size()).c_str());
	sysLogger::TRACE_A("sensor->uuid: ");
	sysLogger::TRACE_W(sensor->uuid.c_str());
}

// ---------------------------------------------------------------------------
__fastcall TSensorView::~TSensorView() {
	int controlsCount = controls.size();
	for (int itCtrl = controlsCount - 1; itCtrl >= 0; --itCtrl) {
    	if (controls[itCtrl]) {
            controls[itCtrl]->Parent = NULL;
            delete controls[itCtrl];
        }
	}

	controls.clear();
}

// ---------------------------------------------------------------------------
bool TSensorView::IsHighlighted(){
	return isHighligthed;
}

// ---------------------------------------------------------------------------
void TSensorView::Highlight(){
	if (isHighligthed) {
		return;
	}
	isHighligthed = true;
}

// ---------------------------------------------------------------------------
void TSensorView::UndoHighlight(){
	if (!isHighligthed) {
		return;
	}
	isHighligthed = false;
}

// ---------------------------------------------------------------------------
void TSensorView::MoveTo(int x, int y) {
	/* check parent border */
	if (x < 0) return;
	else if ((x + viewControl->Width) > parent->Width) return;

	if (y < 0) return;
	else if ((y + viewControl->Height) > parent->Height) return;


	int controlsCount = controls.size();
	int *offsetX = new int[controlsCount];
	int *offsetY = new int[controlsCount];
	int itCtrl;

	for (itCtrl = 0; itCtrl < controlsCount; ++itCtrl) {
		offsetX[itCtrl] = viewControl->Left - controls[itCtrl]->Left;
		offsetY[itCtrl] = viewControl->Top - controls[itCtrl]->Top;
	}

	viewControl->Left = x;
	viewControl->Top = y;

	for (itCtrl = 0; itCtrl < controlsCount; ++itCtrl) {
		controls[itCtrl]->Left = viewControl->Left - offsetX[itCtrl];
		controls[itCtrl]->Top  = viewControl->Top - offsetY[itCtrl];
	}

	delete []offsetX;
	delete []offsetY;
}

// ---------------------------------------------------------------------------
void TSensorView::MouseOffset(int x, int y, int offsetThreshold) {
	if (!offsetThresholdActive && abs(mouseDownX - x) < offsetThreshold && abs(mouseDownY - y) < offsetThreshold) {
		return;
	}

	offsetThresholdActive = true;

	MoveTo(x, y);
}

// ---------------------------------------------------------------------------
void TSensorView::Offset(int offsetX, int offsetY) {
	MoveTo(viewControl->Left + offsetX, viewControl->Top + offsetY);
}

// ---------------------------------------------------------------------------
void TSensorView::Unselect() {
	viewControlSelectShape->Visible = false;
}

// ---------------------------------------------------------------------------
void TSensorView::Select() {

	viewControlSelectShape->Left = viewControl->Left - 3;
	viewControlSelectShape->Top = viewControl->Top - 3;
	viewControlSelectShape->Width = viewControl->Width + 6;
	viewControlSelectShape->Height = viewControl->Height + 6;

	viewControlSelectShape->Visible = true;
}

// ---------------------------------------------------------------------------
void TSensorView::OnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	mouseDownX = X;
	mouseDownY = Y;
	offsetThresholdActive = false;
}

// ---------------------------------------------------------------------------
TControl * TSensorView::GetControl(const String &controlName) {
	int controlsCount = controls.size();
	for (int itCtrl = 0; itCtrl < controlsCount; ++itCtrl) {
		if (controls[itCtrl]->Name == controlName) {
			return controls[itCtrl];
		}
	}

	return NULL;
}

// ---------------------------------------------------------------------------
TControl * TSensorView::GetControl() {
	return TRecordView::GetControl();
}

// ---------------------------------------------------------------------------
void TSensorView::Resize(int width, int height) {
	viewControl->Width = width;
	viewControl->Height = height;

	if (viewControlSelectShape->Visible) {
		/* update selection rectangle */
		Select();
	}
}
