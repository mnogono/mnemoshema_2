//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "chart_tracker_sensor_bits_view.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TChartTrackerSensorBitsView::TChartTrackerSensorBitsView(TChart *chart) : TChartTracker(chart, NULL) {
}

void TChartTrackerSensorBitsView::Move(int x, int y) {
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

	int yBitVal;
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

		yBitVal = ((int)chart->Series[i]->YValues->Value[index]) % 2;

		int n = swprintf(&buffer[bufferIndex],
			L"%s : %d %s\n",
			chart->Series[i]->Title.c_str(),
			yBitVal,
			unit.c_str());

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
