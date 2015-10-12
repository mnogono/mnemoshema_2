//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "record_view_chart.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TRecordViewChart::TRecordViewChart(
	TWinControl *owner,
	const TRecord *record,
	TChartTimeAxis *timeAxis) : TChart(owner) {

	this->owner = owner;

	this->record = record;

	timeAxis->AddObserver(this);

	autoTrackTimeAxis = true;

	autoTrackValueAxis = true;

	chartTracker = CreateChartTracker(this, record);

	chartVerticalLine = new TChartVerticalLine(this, record);

	CreateControls();

	autoAdjust = true;
}

//---------------------------------------------------------------------------
TChartTracker * TRecordViewChart::CreateChartTracker(TChart *chart, const TRecord *record) {
	return new TChartTracker(chart, record);
}

//---------------------------------------------------------------------------
__fastcall TRecordViewChart::~TRecordViewChart() {
}

//---------------------------------------------------------------------------
void TRecordViewChart::MoveTo(int x, int y) {
}

//---------------------------------------------------------------------------
void TRecordViewChart::CreateControls() {
	TChart *chart = this;

	chart->DoubleBuffered = true;
	chart->Parent = owner;

	chart->View3D = false;
	chart->Legend->Visible = false;

	chart->Title->Alignment = taLeftJustify;
	chart->Title->Margins->Bottom = 5;
	chart->Title->Font->Color = clBlack;

	chart->MarginUnits = TTeeUnits::muPixels;
	chart->MarginLeft = 0;
	chart->MarginRight = 5;
	chart->MarginTop = 0;
	chart->MarginBottom = -22;

	chart->BevelInner = TBevelCut::bvNone;
	chart->BevelOuter = TBevelCut::bvNone;

	chart->Border->Color = clGray;
	chart->Border->Visible = true;

	chart->LeftAxis->LabelsSize = 50;
	chart->LeftAxis->Axis->Color = clBlack;
	chart->LeftAxis->EndPosition = 97;

	chart->Walls->Visible = true;
	chart->Walls->Size = 3;

	chart->Align = alTop;

	chart->BottomAxis->DateTimeFormat = "hh:nn:ss";
	chart->BottomAxis->LabelsMultiLine = true;
	chart->BottomAxis->Visible = true;
	chart->BottomAxis->Grid->Visible = true;
	chart->BottomAxis->Axis->Color = clWhite;

	SetDefaultTitle();
}

//---------------------------------------------------------------------------
void TRecordViewChart::SetDefaultTitle() {
	Title->Text->Text = record->GetName();
}

//---------------------------------------------------------------------------
void TRecordViewChart::Highlight() {
	Border->Color = clBlack;
}

//---------------------------------------------------------------------------
void TRecordViewChart::UndoHighlight() {
	Border->Color = BackColor;
}

//---------------------------------------------------------------------------
bool TRecordViewChart::IsHighlighted() {
	return Border->Color == clBlack;
}

//---------------------------------------------------------------------------
void TRecordViewChart::OnTimeAxisChanged(double dt1LocalTime, double dt2LocalTime) {
	BottomAxis->SetMinMax(dt1LocalTime, dt2LocalTime);

	//update visible state of all chart trackable elements
	double localTime;
	double value;
	for (std::list<TChartTrackable *>::iterator i = chartTrackableElements.begin(), iEnd = chartTrackableElements.end(); i != iEnd; ++i) {
		localTime = (*i)->GetLocalTime();


		(*i)->SetVisible(dt1LocalTime <= localTime && localTime <= dt2LocalTime);
		if ((*i)->TrackValue()) {
			value = (*i)->GetValue();
			(*i)->SetVisible(LeftAxis->Minimum <= value && value <= LeftAxis->Maximum);
		}

		if ((*i)->IsVisible() == true) {
			//move trackable element position
			int left = LeftAxis->PosAxis + (localTime - dt1LocalTime) * (Width - LeftAxis->PosAxis - MarginRight) / (dt2LocalTime - dt1LocalTime);
			int top = 0;
			if (LeftAxis->Maximum - LeftAxis->Minimum != 0) {
				top = Height - Height / (LeftAxis->Maximum - LeftAxis->Minimum) * (value - LeftAxis->Minimum);
			}

			(*i)->Move(left, top);
		}
	}
}

//---------------------------------------------------------------------------
const TRecord * TRecordViewChart::GetRecord() {
	return record;
}

//---------------------------------------------------------------------------
void TRecordViewChart::UpdateData(sysObserverable::IObserverable *data) {
	if (data == NULL) {
		return;
	}

	if (data->GetObserverableType() == TObserverableTypes::HISTORY_DATE_TIME) {
		TObserverableHistoryDateTime *historyDateTime = static_cast<TObserverableHistoryDateTime *>(data);

		OnTimeAxisChanged(historyDateTime->dt1LocalTime, historyDateTime->dt2LocalTime);
	}
}

//---------------------------------------------------------------------------
void TRecordViewChart::SetPopupMenu(TPopupMenu *menu) {
	PopupMenu = menu;
}

//---------------------------------------------------------------------------
void TRecordViewChart::SetTimeAxisTracking(bool enable) {
	autoTrackTimeAxis = enable;
}

//---------------------------------------------------------------------------
void TRecordViewChart::SetValueAxisTracking(bool enable) {
	autoTrackValueAxis = enable;
}

//---------------------------------------------------------------------------
void TRecordViewChart::TrackTimeAxis() {
	if (autoTrackTimeAxis == true) {
		BottomAxis->SetMinMax(GetTimeAxisMinValue(), GetTimeAxisMaxValue());
	}
}

//---------------------------------------------------------------------------
void TRecordViewChart::AdjustValueAxis() {
	if (autoTrackValueAxis == false) {
		return;
	}

	double range = GetValueAxisMaxValue() - GetValueAxisMinValue();
	if (range == 0) {
		range = 10;
	}
	double offset = 0.3 * range;

	LeftAxis->SetMinMax(GetValueAxisMinValue() - offset, GetValueAxisMaxValue() + offset);
}

//---------------------------------------------------------------------------
void TRecordViewChart::TrackValueAxis() {
	if (autoTrackValueAxis == true) {
        AdjustValueAxis();
	}
}

//---------------------------------------------------------------------------
void TRecordViewChart::AddTrackableElement(TChartTrackable *trackable) {
	chartTrackableElements.push_back(trackable);
}

//---------------------------------------------------------------------------
void TRecordViewChart::RemoveTrackableElement(TChartTrackable *trackable) {
	if (trackable == NULL) {
		return;
	}

	chartTrackableElements.remove(trackable);
}

//---------------------------------------------------------------------------
TControl * TRecordViewChart::GetControl() {
	return this;
}

//---------------------------------------------------------------------------
bool TRecordViewChart::IsExistTrackableElement(TChartTrackable *trackable) {
	for (std::list<TChartTrackable *>::iterator i = chartTrackableElements.begin(), iEnd = chartTrackableElements.end(); i != iEnd; ++i) {
		if ((*i)->IsEqual(trackable)) {
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
std::list<TChartTrackable *> * TRecordViewChart::GetTrackableElements() {
	return &chartTrackableElements;
}

//---------------------------------------------------------------------------
void TRecordViewChart::UpdateValue() {}
/*
void TRecordViewChart::SetAutoAdjust(bool autoAdjust) {
	this->autoAdjust = autoAdjust;
}
*/
