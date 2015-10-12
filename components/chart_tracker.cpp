//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "chart_tracker.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void GetSerieValueUnderCursor(TLineSeries *serie, double &x, double &y);

TChartTracker::TChartTracker(TChart *chart, const TRecord *record) : TChartTrackable() {
	this->chart = chart;

	this->record = record;

	//create tracker background;
	trackerBackground = new TShape(chart);
	trackerBackground->Parent = chart;
	trackerBackground->Visible = false;

	trackerBackground->OnMouseMove = OnMouseMove;

	//create tracker vertical line
	tracker = new TShape(chart);
	tracker->Parent = chart;
	tracker->Width = 1;
	tracker->Visible = false;
	tracker->Height = chart->Height;

	tracker->OnMouseDown = OnMouseDown;
	tracker->OnMouseMove = OnMouseMove;
	tracker->OnMouseUp = OnMouseUp;

	//create tracker label value
	trackerLabel = new TLabel(chart);
	trackerLabel->Parent = chart;
	trackerLabel->Caption = "";
	trackerLabel->Transparent = true;
	trackerLabel->Visible = false;

	trackerLabel->OnMouseMove = OnMouseMove;
}

void TChartTracker::SetVisible(bool visible) {
	tracker->Visible = visible;
	trackerLabel->Visible = visible;
	trackerBackground->Visible = visible;
	//label->Visible = visible;

	tracker->BringToFront();
	trackerBackground->BringToFront();
	trackerLabel->BringToFront();
}

bool TChartTracker::IsVisible() const {
	return tracker->Visible;
}

void TChartTracker::Move(int x, int y) {
	int seriesCount = chart->SeriesCount();
	if (seriesCount <= 0) {
		trackerLabel->Caption = "";
		return;
	}

	wchar_t buffer[1024];
	int bufferIndex = 0;
	int trackerLineCount = 1;

	double xVal, yVal;
	int trackerCaptionWidth = 0;
	int trackerCaptionHeight = 0;

	chart->Series[0]->GetCursorValues(xVal, yVal);

	TDateTime dt(xVal);
	String trackTime = dt.FormatString("время: HH:nn:ss");
	String trackDate = dt.FormatString("дата: yyyy.mm.dd");
	bufferIndex += swprintf(&buffer[bufferIndex], L"%s\n", trackTime.c_str());
	bufferIndex += swprintf(&buffer[bufferIndex], L"%s\n", trackDate.c_str());

	int width = chart->Canvas->TextWidth(trackTime);
	if (width > trackerCaptionWidth) {
		trackerCaptionWidth = width;
	}
	trackerCaptionHeight += chart->Canvas->TextHeight(trackTime);

	width = chart->Canvas->TextWidth(trackDate);
	if (width > trackerCaptionWidth) {
		trackerCaptionWidth = width;
	}
	trackerCaptionHeight += chart->Canvas->TextHeight(trackDate);

	for (int i = 0; i < seriesCount; i++) {
		chart->Series[i]->GetCursorValues(xVal, yVal);

		int index = BinSearchValueList(
			chart->Series[i]->XValues,
			xVal,
			0,
			chart->Series[i]->XValues->Count - 1,
			5 * sysTime::SEC2DAY);

		if (index == -1) {
			continue;
		}

		int n;
		if (record->record_type == RECORD_TYPE_SENSOR) {
			n = swprintf(&buffer[bufferIndex],
				L"%s : %s %s\n",
				chart->Series[i]->Title.c_str(),
				SensorDoubleDataToString(static_cast<const TSensor *>(record), chart->Series[i]->YValues->Value[index], true).c_str(),
				unit.c_str());
		} else {
			//TODO for record type sensor bit make different chart tracker
			n = swprintf(&buffer[bufferIndex],
				L"%s : %.0f\n",
				chart->Series[i]->Title.c_str(),
                chart->Series[i]->YValues->Value[index]);
		}

		//calculate the text width
		width = chart->Canvas->TextWidth(String(&buffer[bufferIndex], n));
		if (width > trackerCaptionWidth) {
			trackerCaptionWidth = width;
		}

		//update tracker caption height
		trackerCaptionHeight += chart->Canvas->TextHeight(String(&buffer[bufferIndex], n));

		bufferIndex += n;

		++trackerLineCount;
	}

	buffer[bufferIndex] = 0;

	//Text Width return less the it has
	trackerCaptionWidth += 10;

	if (chart->Width < (x + 5 + trackerCaptionWidth)) {
		tracker->Left = x - 1;

		trackerLabel->Left = x - trackerCaptionWidth + 5;

		trackerBackground->Left = x - trackerCaptionWidth;

	} else {
		trackerLabel->Left = x + 5;

		tracker->Left = x + 1;

		trackerBackground->Left = x + 1;
	}

	trackerLabel->Caption = buffer;

	trackerBackground->Height = trackerCaptionHeight;
	trackerBackground->Brush->Color = clWhite;
	trackerBackground->Width = trackerCaptionWidth;
}

bool TChartTracker::IsEnabled() {
	return enabled;
}

void TChartTracker::SetEnabled(bool enabled) {
	this->enabled = enabled;
}

//---------------------------------------------------------------------------
void GetSerieValueUnderCursor(TLineSeries *serie, double &x, double &y) {
	serie->GetCursorValues(x, y);
	int pos = BinSearchValueList(serie->XValues, x, 0, serie->XValues->Count - 1, sysTime::SEC2DAY);
	if (pos == -1) {
		x = 0; y = 0;
	} else {
		y = serie->YValues->Value[pos];
	}
}

//---------------------------------------------------------------------------
double TChartTracker::GetLocalTime() const {
	return trackLocalTime;
}

//---------------------------------------------------------------------------
void TChartTracker::SetLocalTime(double localTime) {
	trackLocalTime = localTime;
}

//---------------------------------------------------------------------------
void TChartTracker::SetValue(double value) {
	trackValue = value;
}

bool TChartTracker::TrackValue() {
	return false;
}

//---------------------------------------------------------------------------
double TChartTracker::GetValue() const {
	return trackValue;
}

//---------------------------------------------------------------------------
void __fastcall TChartTracker::OnMouseMove(TObject* Sender, TShiftState Shift, int X, int Y) {
	TWinControl *winControl = static_cast<TWinControl *>(Sender);
	chart->OnMouseMove(chart, Shift, winControl->Left + X, winControl->Top + Y);
}

//---------------------------------------------------------------------------
void __fastcall TChartTracker::OnMouseDown(TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TWinControl *winControl = static_cast<TWinControl *>(Sender);
	chart->OnMouseDown(chart, Button, Shift, winControl->Left + X, winControl->Top + Y);
}

//---------------------------------------------------------------------------
void __fastcall TChartTracker::OnMouseUp(TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	TWinControl *winControl = static_cast<TWinControl *>(Sender);
	chart->OnMouseUp(chart, Button, Shift, winControl->Left + X, winControl->Top + Y);
}

//---------------------------------------------------------------------------
bool TChartTracker::IsEqual(TChartTrackable *chartTrackable) {
	return false;
}

//---------------------------------------------------------------------------
int TChartTracker::GetType() {
	return trackable::TTrackableType::TRACKER;
}
