//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "sensor_bit_view_numeric_chart.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TSensorBitViewNumericChart::TSensorBitViewNumericChart(TWinControl *owner, const TSensorBit *sensorBit, TChartTimeAxis *timeAxis) : TRecordViewChart(owner, sensorBit, timeAxis) {
	this->sensorBit = sensorBit;

	signal->Title = sensorBit->GetName();
}

//---------------------------------------------------------------------------
void TSensorBitViewNumericChart::DisplayData(TSensorData *data) {
	if (data != NULL) {
		double y = SensorBitDataToByte(sensorBit, data);
		double x = sysTime::ConvertToDaysLocalTime(data->timeGMT * sysTime::MSEC2SEC);

		//make discrete steps
		if (signal->YValues->Count) {
			signal->AddXY(x, (*signal->YValues)[signal->YValues->Count - 1], "", clBlack);
		}
		signal->AddXY(x, y, "", clBlack);
	}

	AdjustValueAxis();

	SetDefaultTitle();
}

//---------------------------------------------------------------------------
void TSensorBitViewNumericChart::DisplayData(std::list<TSensorData *> *data) {
	signal->Clear();

	if (data != NULL && data->size() != 0) {
		for (std::list<TSensorData *>::iterator i = data->begin(), iEnd = data->end(); i != iEnd; ++i) {
			double y = SensorBitDataToByte(sensorBit, *i);
			double x = sysTime::ConvertToDaysLocalTime((*i)->timeGMT * sysTime::MSEC2SEC);

			//make discrete steps
			if (signal->YValues->Count) {
				signal->AddXY(x, (*signal->YValues)[signal->YValues->Count - 1], "", clBlack);
			}
			signal->AddXY(x, y, "", clBlack);
		}
		//BottomAxis->SetMinMax(signal->XValues->MinValue, signal->XValues->MaxValue);
	}

	AdjustValueAxis();

	SetDefaultTitle();
}

//---------------------------------------------------------------------------
void TSensorBitViewNumericChart::CreateSignalSeries() {
	signal = new TLineSeries(this);
	AddSeries(signal);

	//clear all y axis tick titles, and display only 0 and 1 title tick
	LeftAxis->Items->Clear();
	LeftAxis->Items->Add(0, "0");
	LeftAxis->Items->Add(1, "1");

	//make default min max y value for discrete chart signal
	LeftAxis->SetMinMax(-0.5, 1.5);
}

//---------------------------------------------------------------------------
void TSensorBitViewNumericChart::CreateControls() {
	TRecordViewChart::CreateControls();

	CreateSignalSeries();
}

//---------------------------------------------------------------------------
void TSensorBitViewNumericChart::ChangeMinMaxLeftAxis(double coefficient) {
	//do not change left axis scale for discrete signals
}

//---------------------------------------------------------------------------
double TSensorBitViewNumericChart::GetTimeAxisMinValue() {
	return BottomAxis->Minimum;
}

//---------------------------------------------------------------------------
double TSensorBitViewNumericChart::GetTimeAxisMaxValue() {
	return BottomAxis->Maximum;
}

//---------------------------------------------------------------------------
double TSensorBitViewNumericChart::GetValueAxisMaxValue() {
	return 1.5;
}

//---------------------------------------------------------------------------
double TSensorBitViewNumericChart::GetValueAxisMinValue() {
	return -0.5;
}

//---------------------------------------------------------------------------
void TSensorBitViewNumericChart::SetValueAxisMinMaxValue(double min, double max) {
	LeftAxis->SetMinMax(min, max);
}

//---------------------------------------------------------------------------
std::map<TChartSeries *, double> TSensorBitViewNumericChart::GetSeriesValues(double localTime) {
	//return std::map<TChartSeries *, double>();

	std::map<TChartSeries *, double> seriesValues;

	double xVal, yVal;

	int index = BinSearchValueList(
		signal->XValues,
		localTime,
		0,
		signal->XValues->Count - 1,
		5 * sysTime::SEC2DAY);

	if (index != -1) {
		seriesValues[signal] = signal->YValues->Value[index];
	}

	return seriesValues;
}
