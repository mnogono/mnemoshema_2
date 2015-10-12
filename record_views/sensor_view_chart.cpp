//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "sensor_view_chart.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TSensorViewChart::TSensorViewChart(TWinControl *parent, const TSensor *sensor, TChartTimeAxis *timeAxis) : TRecordViewChart(parent, sensor, timeAxis) {
	this->sensor = sensor;

	CreateControls();

	signal->Title = sensor->GetName();

	int count_of_period_before_brake = DataModuleMP->GetSettingInt("count_of_period_before_brake");
	count_of_period_before_brake = (count_of_period_before_brake == 0)?1:count_of_period_before_brake;

	double rate = (0 < sensor->rate_msec && sensor->rate_msec <= 1000)?sensor->rate_msec:1000;
	sensorPeriodDay = count_of_period_before_brake * rate * sysTime::MSEC2DAY;

	acceptableDispersionPercentage = 20;

	chartData.SetInitialSize(500);
	/*
	dataSize = 0;

	size_t initialSize = 10;

	dataX.resize(initialSize, 0.0);

	dataY.resize(initialSize, 0.0);
	*/
}

//---------------------------------------------------------------------------
__fastcall TSensorViewChart::~TSensorViewChart() {

}

//---------------------------------------------------------------------------
void TSensorViewChart::CreateSignalSeries() {
	signal = new TLineSeries(this);
	AddSeries(signal);
}

//---------------------------------------------------------------------------
void TSensorViewChart::CreateControls() {
	TRecordViewChart::CreateControls();

	CreateSignalSeries();
}

//---------------------------------------------------------------------------
void TSensorViewChart::DisplayData(TSensorData *data) {

}

//---------------------------------------------------------------------------
void TSensorViewChart::DisplayData(std::list<TSensorData *> *data) {

}

//---------------------------------------------------------------------------
TLineSeries * TSensorViewChart::GetSignal() {
	return signal;
}

//---------------------------------------------------------------------------
void TSensorViewChart::ChangeMinMaxLeftAxis(double coefficient) {
	double newSignalRange = coefficient * (LeftAxis->Maximum - LeftAxis->Minimum);
	double middleSignal = 0.5 * (LeftAxis->Maximum + LeftAxis->Minimum);
	LeftAxis->SetMinMax(middleSignal - 0.5 * newSignalRange, middleSignal + 0.5 * newSignalRange);
}

//---------------------------------------------------------------------------
double TSensorViewChart::GetTimeAxisMinValue() {
	return BottomAxis->Minimum;
}

//---------------------------------------------------------------------------
double TSensorViewChart::GetTimeAxisMaxValue() {
	return BottomAxis->Maximum;
}

//---------------------------------------------------------------------------
double TSensorViewChart::GetValueAxisMaxValue() {
	return signal->YValues->MaxValue;
}

//---------------------------------------------------------------------------
double TSensorViewChart::GetValueAxisMinValue() {
	return signal->YValues->MinValue;
}

//---------------------------------------------------------------------------
void TSensorViewChart::SetValueAxisMinMaxValue(double min, double max) {
	LeftAxis->SetMinMax(min, max);
}

//---------------------------------------------------------------------------
std::map<TChartSeries *, double> TSensorViewChart::GetSeriesValues(double localTime) {
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
