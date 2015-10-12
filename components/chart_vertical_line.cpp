//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "chart_vertical_line.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TChartVerticalLine::TChartVerticalLine(TChart *chart, const TRecord *record) {
	Init(chart, record);
}

TChartVerticalLine::TChartVerticalLine(const TChartVerticalLine &chartVerticalLine) {
	Init(chartVerticalLine.chart, chartVerticalLine.record);

	SetLocalTime(chartVerticalLine.GetLocalTime());
	SetValue(chartVerticalLine.GetValue());
	SetVisible(chartVerticalLine.IsVisible());
	Move(chartVerticalLine.line->Left, 0);

	SetCaption(chartVerticalLine.label->Caption);
}

void TChartVerticalLine::Init(TChart *chart, const TRecord *record) {
	this->record = record;

	this->chart = chart;

	line = new TShape(chart);
	line->Parent = chart;
	line->Visible = false;
	line->Width = 1;
	line->Height = chart->Height;
	line->OnMouseDown = MouseDown;
	line->OnMouseUp = MouseUp;
	line->OnMouseMove = MouseMove;

	box = new TShape(chart);
	box->Parent = chart;
	box->Shape = stRectangle;
	box->Pen->Color = clBlack;
	box->Visible = false;
	box->Width = 100;
	box->Height = 20;

	label = new TLabel(chart);
	label->Parent = chart;
	label->Caption = "";
	label->Visible = false;
	label->Margins->SetBounds(10, 10, 10, 10);
	label->Canvas->Brush->Color = clWhite;
	label->Transparent = true;

	isSuperline = false;
}

__fastcall TChartVerticalLine::~TChartVerticalLine() {
	delete box;
	delete label;
	delete line;
}

double TChartVerticalLine::GetLocalTime() const {
	return trackLocalTime;
}

void TChartVerticalLine::SetLocalTime(double localTime) {
	trackLocalTime = localTime;
}

void TChartVerticalLine::SetValue(double value) {
	trackValue = value;
}

bool TChartVerticalLine::TrackValue() {
	return false;
}

double TChartVerticalLine::GetValue() const {
	return trackValue;
}

void TChartVerticalLine::Highlight(bool highlight) {}

void TChartVerticalLine::Move(int x, int y) {
	line->Left = x;
	label->Left = x + 4;
	box->Left = x;
}

void TChartVerticalLine::SetVisible(bool visible) {
	line->Visible = visible;
	label->Visible = visible;
	box->Visible = visible;
}

bool TChartVerticalLine::IsVisible() const {
	return line->Visible;
}

void TChartVerticalLine::UpdateCaption() {

}

void TChartVerticalLine::SetCaption(String caption, double localTime, const std::map<TChartSeries *, double> &seriesValues) {
	String text = "";
	if (!caption.IsEmpty()) {
	    text = caption + "\n";
	}

	for (std::map<TChartSeries *, double>::const_iterator i = seriesValues.begin(), iEnd = seriesValues.end(); i != iEnd; ++i) {

		if (record != NULL) {
			String sValue = "";
			if (record->record_type == RECORD_TYPE_SENSOR) {
				sValue = SensorDoubleDataToString(
					static_cast<const TSensor *>(record),
					i->second,
					true);
			} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
				sValue = FloatToStr(i->second);
			}

			String subCaption = i->first->Title + ": " + sValue + "\n";
			text += subCaption;
		}
	}


	TDateTime dt(localTime);
	text += dt.FormatString("yyyy.mm.dd HH:nn:ss");

	label->Caption = text;

	UpdateCaptionBox();
}

void TChartVerticalLine::SetCaption(String caption) {
	label->Caption = caption;

	UpdateCaptionBox();
}

void TChartVerticalLine::UpdateCaptionBox() {
	std::vector<std::wstring> strings;
	sysStr::Split(label->Caption.c_str(), L"\n", strings);
	int height = 0;
	int width, maxWidth = 0;
	String chunk;
	for (std::vector<std::wstring>::iterator i = strings.begin(), iEnd = strings.end(); i != iEnd; ++i) {
		chunk = String((*i).c_str());
		height += chart->Canvas->TextHeight(chunk);
		width = chart->Canvas->TextWidth(chunk);
		if (width > maxWidth) {
			maxWidth = width;
		}
	}

	box->Width = maxWidth + 10;
	box->Height = height + 3;
}

void __fastcall TChartVerticalLine::MouseDown(System::TObject* Sender, System::Uitypes::TMouseButton Button, System::Classes::TShiftState Shift, int X, int Y) {
	chart->OnMouseDown(chart, Button, Shift, line->Left + X, Y);
}

void __fastcall TChartVerticalLine::MouseUp(System::TObject* Sender, System::Uitypes::TMouseButton Button, System::Classes::TShiftState Shift, int X, int Y) {
	chart->OnMouseUp(chart, Button, Shift, line->Left + X, Y);
}

void __fastcall TChartVerticalLine::MouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	chart->OnMouseMove(chart, Shift, line->Left + X, Y);
}

bool TChartVerticalLine::IsEqual(TChartTrackable *chartTrackable) {
	return chartTrackable->GetType() == GetType() &&  chartTrackable->GetLocalTime() == GetLocalTime();
}

int TChartVerticalLine::GetType() {
	return trackable::TTrackableType::VERTICAL_LINE;
}

void TChartVerticalLine::MarkAsSuperline(bool isSuperline) {
	this->isSuperline = isSuperline;
}

bool TChartVerticalLine::IsSuperline() {
	return isSuperline;
}
