//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "sensor_bits_view_numeric_chart.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TSensorBitsViewNumericChart::TSensorBitsViewNumericChart(TWinControl *owner, const TSensor *sensor, TChartTimeAxis *timeAxis) : TRecordViewChart(owner, sensor, timeAxis){
	this->sensor = sensor;

	CreateSignalSeries();

	Legend->Visible = true;
	Legend->Alignment = laTop;
	Legend->CheckBoxes = true;
	Legend->CheckBoxesStyle = cbsCheck;

	MarginTop = 0;

	int count_of_period_before_brake = DataModuleMP->GetSettingInt("count_of_period_before_brake");
	count_of_period_before_brake = (count_of_period_before_brake == 0)?1:count_of_period_before_brake;

	double rate = (0 < sensor->rate_msec && sensor->rate_msec <= 1000)?sensor->rate_msec:1000;
	sensorPeriodDay = count_of_period_before_brake * rate * sysTime::MSEC2DAY;
}

//---------------------------------------------------------------------------
TChartTracker * TSensorBitsViewNumericChart::CreateChartTracker(TChart *chart, const TRecord *record) {
	return new TChartTrackerSensorBitsView(chart);
}

//---------------------------------------------------------------------------
void TSensorBitsViewNumericChart::DisplayData(TSensorData *data) {
	if (data != NULL) {
		double y,
			   x = sysTime::ConvertToDaysLocalTime(data->timeGMT * sysTime::MSEC2SEC);

		int bit = 0;

		__int64 value;

		for (std::vector<Vcltee::Series::TLineSeries *>::iterator iBitSerie = bitSignals.begin(), iBitSerieEnd = bitSignals.end(); iBitSerie != iBitSerieEnd; ++iBitSerie) {
			Vcltee::Series::TLineSeries *signal = *iBitSerie;

			value = SensorDataToInt64(sensor, data);
			y = 2 * bit + ((value >> bit) & 1);

			if (signal->XValues->Count > 0) {
				double xPrev = signal->XValues->Value[signal->XValues->Count - 1];
				int nullCount = (x - xPrev) / sensorPeriodDay;
				if (nullCount) {
					signal->AddNullXY(xPrev, y, "");
				}

				signal->AddXY(x, y, "", clBlack);
			} else {
				signal->AddXY(x, y, "", clBlack);
			}

			++bit;
		}
	}

	//make default min max y value for discrete chart signal
	//LeftAxis->SetMinMax(-1.5, bitSignals.capacity() * 2 + 1.5);

	SetDefaultTitle();

	TrackTimeAxis();

	TrackValueAxis();
}

//---------------------------------------------------------------------------
void TSensorBitsViewNumericChart::DisplayData(std::list<TSensorData *> *data) {
	for (std::vector<Vcltee::Series::TLineSeries *>::iterator iBitSerie = bitSignals.begin(), iBitSerieEnd = bitSignals.end(); iBitSerie != iBitSerieEnd; ++iBitSerie) {
		Vcltee::Series::TLineSeries *signal = *iBitSerie;
		signal->Clear();
	}

	if (data != NULL && data->size() != 0) {
		double x,
			   y,
			   xPrev = sysTime::ConvertToDaysLocalTime((*data->begin())->timeGMT * sysTime::MSEC2SEC);
		int nullCount,
			bit;
		__int64 value;

		for (std::list<TSensorData *>::iterator i = data->begin(), iEnd = data->end(); i != iEnd; ++i) {
			x = sysTime::ConvertToDaysLocalTime((*i)->timeGMT * sysTime::MSEC2SEC);
			bit = 0;
			for (std::vector<Vcltee::Series::TLineSeries *>::iterator iBitSerie = bitSignals.begin(), iBitSerieEnd = bitSignals.end(); iBitSerie != iBitSerieEnd; ++iBitSerie) {
				Vcltee::Series::TLineSeries *signal = *iBitSerie;
				value = SensorDataToInt64(sensor, (*i));

				//separate discrete signals to several lines: 0-1, 2-3, 4-5, ...
				y = 2 * bit + ((value >> bit) & 1);

				nullCount = (x - xPrev) / sensorPeriodDay;
				if (nullCount) {
					signal->AddNullXY(xPrev, y, "");
				} else {
					signal->AddXY(x, y, "", clBlack);
				}

				++bit;
			}

			xPrev = x;
		}

		Vcltee::Series::TLineSeries *signal = *(bitSignals.begin());
		BottomAxis->SetMinMax(signal->XValues->MinValue, signal->XValues->MaxValue);
	}

	//make default min max y value for discrete chart signal
	LeftAxis->SetMinMax(-1.5, bitSignals.capacity() * 2 + 1.5);

	SetDefaultTitle();
}

//---------------------------------------------------------------------------
void TSensorBitsViewNumericChart::CreateSignalSeries() {
	size_t sensorDataSize = TSensor::GetDataSize(sensor->data_type, 0) - sizeof(long);

	bitSignals.reserve(sensorDataSize * 8);

	//clear all y axis tick titles, and display only 0 and 1 title tick
	LeftAxis->Items->Clear();

	for (int i = 0, len = bitSignals.capacity(); i < len; ++i) {
		TLineSeries *serie = new TLineSeries(this);
		serie->Stairs = true;

		AddSeries(serie);
		bitSignals.push_back(serie);

		TRecord *record = ::GetRecord(sensor->uuid + IntToStr(i));
		serie->LegendTitle = static_cast<TSensorBit *>(record)->desc;
		serie->Title = serie->LegendTitle;

		serie->Marks->DrawEvery = 100;
		serie->Marks->Visible = false;
		serie->Marks->Style = smsSeriesTitle;

		//prepare items captions fo discrete bit value of different lines
		LeftAxis->Items->Add(2 * i,     "0");
		LeftAxis->Items->Add(2 * i + 1, "1");
	}
}

//---------------------------------------------------------------------------
void TSensorBitsViewNumericChart::ChangeMinMaxLeftAxis(double coefficient) {
	//do not change left axis scale for discrete signals
	double newSignalRange = coefficient * (LeftAxis->Maximum - LeftAxis->Minimum);
	double middleSignal = 0.5 * (LeftAxis->Maximum + LeftAxis->Minimum);
	LeftAxis->SetMinMax(middleSignal - 0.5 * newSignalRange, middleSignal + 0.5 * newSignalRange);
}

//---------------------------------------------------------------------------
double TSensorBitsViewNumericChart::GetTimeAxisMinValue() {
	return BottomAxis->Minimum;
}

//---------------------------------------------------------------------------
double TSensorBitsViewNumericChart::GetTimeAxisMaxValue() {
	return BottomAxis->Maximum;
}

//---------------------------------------------------------------------------
double TSensorBitsViewNumericChart::GetValueAxisMaxValue() {
	return bitSignals.capacity() * 2 + 1.5;
}

//---------------------------------------------------------------------------
double TSensorBitsViewNumericChart::GetValueAxisMinValue() {
	return -1.5;
}

//---------------------------------------------------------------------------
void TSensorBitsViewNumericChart::SetValueAxisMinMaxValue(double min, double max) {
	LeftAxis->SetMinMax(min, max);
}

//---------------------------------------------------------------------------
std::map<TChartSeries *, double> TSensorBitsViewNumericChart::GetSeriesValues(double localTime) {
	return std::map<TChartSeries *, double>();
}
