//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "sensor_view_numeric_chart.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TSensorViewNumericChart::TSensorViewNumericChart(TWinControl *parent, const TSensor *sensor, TChartTimeAxis *timeAxis) : TSensorViewChart(parent, sensor, timeAxis) {
	gaussKernal = sysMath::GaussKernal(10, 1);
}

//---------------------------------------------------------------------------
__fastcall TSensorViewNumericChart::~TSensorViewNumericChart() {
    delete []gaussKernal;
}

//---------------------------------------------------------------------------
void TSensorViewNumericChart::SetDefaultTitle() {
	String title = "";

	//TODO why here need to check the record_type, it is always TYPE_SENSOR ?
	if (record->record_type == RECORD_TYPE_SENSOR) {
		const TSensor *sensor = static_cast<const TSensor *>(record);

		TDevice *device = GetDevice(sensor->device_id);
		if (device != NULL) {
			title = device->GetName() + " ";
		}

		title += record->GetName();
	}

	if (signal && signal->Count() == 0) {
		Title->Text->Text = title + " (данных нет)";
	} else {
		Title->Text->Text = title;
	}
}
//---------------------------------------------------------------------------
void TSensorViewNumericChart::UpdateChart() {
	signal->Clear();

	if (chartData.dataSize == 0) {
		return;
	}

	TChartData *d;

	if (GAUSSIAN_FILTER_ENABLED == true ||
		MEDIAN_FILTER_ENABLED == true) {

		d = new TChartData(chartData);
	} else {
		d = &chartData;
	}

	//make copy of sensor data, then modify it before display
	//TChartData cpyChartData(chartData);

	if (MEDIAN_FILTER_ENABLED == true) {
		MedianFilter3(sensor, *d);
	}

	if (GAUSSIAN_FILTER_ENABLED == true) {
		sysMath::ApplyGaussFilter(gaussKernal, 10, &(d->dataY)[0], d->dataSize);
	}

	double x,
		   y,
		   xPrev = d->dataX[0],
		   yPrev = d->dataY[0] - 1; //-1 just for make true sentence y != yPrev

	double resolution = 1.1/* for improve */ * (GetTimeAxisMaxValue() - GetTimeAxisMinValue()) / (double) (Width);
	double maxAllowTimeRange = std::max(resolution, sensorPeriodDay);

	for (size_t i = 0; i < d->dataSize; ++i) {
		y = d->dataY[i] * sensor->powerValue;
		x = d->dataX[i];

		if ((x - xPrev) > maxAllowTimeRange /* chart data filtered by resolution */ ) {
			signal->AddNullXY(xPrev, y, "");
		} else {
			signal->AddXY(x, y, "", clBlack);
		}

		xPrev = x;
		yPrev = y;
	}

	//delete only if d was memory allocate above
	if (d != &chartData) {
	    delete d;
	}


	AdjustValueAxis();

	SetDefaultTitle();
}

//---------------------------------------------------------------------------
void TSensorViewNumericChart::DisplayData(TSensorData *data) {
	if (data == NULL) {
		return;
	}

	chartData.AppendSensorData(sensor, data);

	UpdateChart();
}

//---------------------------------------------------------------------------
void TSensorViewNumericChart::DisplayData(std::list<TSensorData *> *data) {
	chartData.SetSensorData(sensor, data);

	UpdateChart();
}

//---------------------------------------------------------------------------
/*
void TSensorViewNumericChart::DisplayData(std::list<TSensorData *> *data) {
	signal->Clear();

	if (data == NULL || data->size() == 0) {
		return;
	}

	//prepare data

	//1. remove zero spikes
	ZeroSpikeRemove(sensor, data);

	//2. remove all other spikes
	SpikeRemove(sensor, data, sensorPeriodDay);

	double x,
		   y,
		   xPrev = sysTime::ConvertToDaysLocalTime((*data->begin())->timeGMT * sysTime::MSEC2SEC);
	int nullCount;

	std::list<TSensorData *>::iterator iBegin = data->begin(), iEnd = data->end();

	for (std::list<TSensorData *>::iterator i = iBegin; i != iEnd; ++i) {
		y = SensorDataToDouble(sensor, *i) * sensor->powerValue;
		x = sysTime::ConvertToDaysLocalTime((*i)->timeGMT * sysTime::MSEC2SEC);

		if ((x - xPrev) > sensorPeriodDay) {
			signal->AddNullXY(xPrev, y, "");
		} else {
			signal->AddXY(x, y, "", clBlack);
		}

		xPrev = x;
	}

	AdjustValueAxis();

	SetDefaultTitle();
}
*/

/*
//---------------------------------------------------------------------------
void TSensorViewNumericChart::DisplayData(std::list<TSensorData *> *data) {
	//sysLogger::LOG_A("...", sysLogger::LOG_LEVEL_PROFILE);

	signal->Clear();

	if (data != NULL && data->size() != 0) {
		double x,
			   y,
			   xPrev = sysTime::ConvertToDaysLocalTime((*data->begin())->timeGMT * sysTime::MSEC2SEC);
		int nullCount;

		//calculate series average
		double average = 0.0;
		int countWithoutZero = 0;
		double acceptableDispersion = 0.0;
		double xOutOfAcceptable = 0;

		std::list<TSensorData *>::iterator i = data->begin(), iEnd = data->end();
		for (; i != iEnd; ++i) {
			y = SensorDataToDouble(sensor, *i);
			if (y != 0) {
				++countWithoutZero;
				average += y;
			}
		}
		if (countWithoutZero != 0) {
			average = average * sensor->powerValue / countWithoutZero;
			acceptableDispersion = average * acceptableDispersionPercentage / 100.0;
		}

		//plot series
		for (i = data->begin(); i != iEnd; ++i) {
			y = SensorDataToDouble(sensor, *i) * sensor->powerValue;
			x = sysTime::ConvertToDaysLocalTime((*i)->timeGMT * sysTime::MSEC2SEC);

			if ((y < average - acceptableDispersion) || (y > average + acceptableDispersion)) {
				if (xOutOfAcceptable == 0) {
					xOutOfAcceptable = x;
				}
				if ((x - xOutOfAcceptable) < sensorPeriodDay) {
					continue;
				}
			}

			if ((x - xPrev) > sensorPeriodDay) {
				signal->AddNullXY(xPrev, y, "");
			} else {
				signal->AddXY(x, y, "", clBlack);
			}

			xPrev = x;
			xOutOfAcceptable = 0;
		}

		AdjustValueAxis();
	}

	SetDefaultTitle();

	//AnsiString profileMessage = AnsiString("display data of sensor") + AnsiString(sensor->GetName());
	//sysLogger::LOG_A(profileMessage.c_str(), sysLogger::LOG_LEVEL_PROFILE);
}
*/

//---------------------------------------------------------------------------
void TSensorViewNumericChart::UpdateValue() {
	UpdateChart();
}
