//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "sensor_bit_view_text_chart.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TSensorBitViewTextChart::TSensorBitViewTextChart(TWinControl *owner, const TSensorBit *sensorBit, TChartTimeAxis *timeAxis) : TRecordViewChart(owner, sensorBit, timeAxis) {
	this->sensorBit = sensorBit;
}

//---------------------------------------------------------------------------
void TSensorBitViewTextChart::DisplayData(TSensorData *data) {
	if (data != NULL) {
		String text = SensorBitDataToString(sensorBit, data);
		double y;
		double x = sysTime::ConvertToDaysLocalTime(data->timeGMT * sysTime::MSEC2SEC);
		if (sensorBit->value0 == text) {
			y = 0;
		} else if (sensorBit->value1 == text) {
			y = 1;
		}

		signal->AddXY(x, y, text, clBlack);
	}

	SetDefaultTitle();
}

//---------------------------------------------------------------------------
void TSensorBitViewTextChart::DisplayData(std::list<TSensorData *> *data) {
	signal->Clear();

	if (data != NULL && data->size() != 0) {
		for (std::list<TSensorData *>::iterator i = data->begin(), iEnd = data->end(); i != iEnd; ++i) {
			String text = SensorBitDataToString(sensorBit, *i);
			double y;
			double x = sysTime::ConvertToDaysLocalTime((*i)->timeGMT * sysTime::MSEC2SEC);
			if (sensorBit->value0 == text) {
				y = 0;
			} else if (sensorBit->value1 == text) {
				y = 1;
			}

			signal->AddXY(x, y, text, clBlack);
		}

		LeftAxis->SetMinMax(signal->YValues->MinValue, signal->YValues->MaxValue);
		BottomAxis->SetMinMax(signal->XValues->MinValue, signal->XValues->MaxValue);
	}

	SetDefaultTitle();
}

//---------------------------------------------------------------------------
void TSensorBitViewTextChart::ChangeMinMaxLeftAxis(double coefficient) {
	//do not change left axis scale for discrete signals
}

//---------------------------------------------------------------------------
double TSensorBitViewTextChart::GetTimeAxisMinValue() {
	return BottomAxis->Minimum;
	//return signal->XValues->MinValue;
}

//---------------------------------------------------------------------------
double TSensorBitViewTextChart::GetTimeAxisMaxValue() {
	return BottomAxis->Maximum;
	//return signal->XValues->MaxValue;
}

//---------------------------------------------------------------------------
double TSensorBitViewTextChart::GetValueAxisMaxValue() {
	return signal->YValues->MaxValue;
}

//---------------------------------------------------------------------------
double TSensorBitViewTextChart::GetValueAxisMinValue() {
	return signal->YValues->MinValue;
}

//---------------------------------------------------------------------------
void TSensorBitViewTextChart::SetValueAxisMinMaxValue(double min, double max) {
	//do not change left axis scale for discrete signals
}

//---------------------------------------------------------------------------
std::map<TChartSeries *, double> TSensorBitViewTextChart::GetSeriesValues(double localTime) {
    return std::map<TChartSeries *, double>();
}
