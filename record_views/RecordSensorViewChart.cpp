//---------------------------------------------------------------------------


#pragma hdrstop

#include "RecordSensorViewChart.h"

//---------------------------------------------------------------------------
TRecordSensorViewChart::TRecordSensorViewChart(TWinControl *owner, TRecord *record, TUser *user, void (*SetChartSeriesSetings)(TLineSeries *, TUser *, String)) : TRecordViewChart(owner, record, user, SetChartSeriesSetings) {
	Title->Text->Text = GetName();

    count_of_period_before_brake = DataModuleMP->GetSettingInt("count_of_period_before_brake");
}

//---------------------------------------------------------------------------
String __fastcall TRecordSensorViewChart::GetName(){
	return GetSensorTreeItemName((TSensor *)record);
	//return sensor->name + " " + GetPowerPrefix(sensor->power) + GetUnitName(sensor->unit);
}

//---------------------------------------------------------------------------
inline __fastcall TRecordSensorViewChart::~TRecordSensorViewChart(void){
};

/*
double __get(void *data, int i) {
	std::vector<TData *> *vecData = (std::vector<TData *> *)data;
	return (*vecData)[i]->toDouble();
}

void __set(void *data, int i, double value) {
	std::vector<TData *> *vecData = (std::vector<TData *> *)data;
	(*vecData)[i]->data = value;
}
*/

//---------------------------------------------------------------------------
void TRecordSensorViewChart::DisplayData(std::vector<TData *> *data, bool bClearSeries){
	try {

		TRecordViewChart::DisplayData(data, bClearSeries);

		vector<TData *>::iterator itData = data->begin();
		//long milliseconds;
		//WORD hour, min, sec, msec;
		//char dataCaption[10];
		long iRateMilliseconds = this->sensor->rate;
		double fRateDays = this->sensor->rate * sysTime::MSEC2DAY;
		double val;
		//int iCurrentData = 0;
		int iDataCount = data->size();

		#ifdef ENABLE_TRACE_LOG
		if (TLogger::logger->CheckLogLevel(LOG_LEVEL_TRACE)) {
			TLogger::logger->Log("DataCount to display: " + IntToStr(iDataCount), LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);
		}
		#endif

		//long msec1, msec2;

		int iNullDataCount;
		TData *data1, *data2;

		//struct tm *timeinfo;
		time_t time;
		double dTime;

		this->minYScrollValue = std::numeric_limits<double>::max();
		this->maxYScrollValue = std::numeric_limits<double>::min();
		/*
		double lastSeriesDataTime = 0;
		if (series->XValues->Count) {
			lastSeriesDataTime = series->XValues->Value[series->XValues->Count - 1];
			//double d = series->XValues[0].;
			//lastSeriesDataTime = series->XValues[series->XValues->Count];
		}
		*/

		//TODO perform some Gauss filter here
		double gaussValue = 0;
		if (sensor) {
			String sensorGaussName = sensor->uuid + String(L"_gaussWidth");
			double gaussWidth = DataModuleMP->GetSettingFloat(sensorGaussName);

			if (gaussWidth && (iDataCount > gaussWidth)) {
				double *d;
				size_t size;
				if (iDataCount > gaussWidth) {
					//convert vector of TData into double array
					size = iDataCount;
					d = new double[size];
					for (int iData = 0; iData < iDataCount; ++iData) {
						d[iData] = (*data)[iData]->toDouble() * sensor->powerValue;
					}
				}/* else {
					//TODO
					size = iDataCount;
					d = new double[iDataCount];
					for (int iData = 0; iData < iDataCount; ++iData) {
						d[iData] = (*data)[iData]->toDouble() * sensor->powerValue;
					}
				}*/

				double *gaussKernal = sysMath::GaussKernal(gaussWidth, 5.0);
				sysMath::ApplyGaussFilter(gaussKernal, gaussWidth, d, size);
				delete []gaussKernal;

				for (int iData = 0; iData < iDataCount; ++iData) {
					data1 = (*data)[iData];
					time = data1->msecGMT * sysTime::MSEC2SEC;
					dTime = sysTime::ConvertToDaysLocalTime(time);

					series->AddXY(dTime, d[iData], "", clBlack);
				}

				delete []d;
			} else {
				double lastSeriesDataTime = 0;
				if (series->XValues->Count) {
					lastSeriesDataTime = series->XValues->Value[series->XValues->Count - 1];
				}

				for (int iData = 0; iData < iDataCount; ++iData) {
					data1 = (*data)[iData];
					val = data1->toDouble() * sensor->powerValue;
					time = data1->msecGMT * sysTime::MSEC2SEC;
					dTime = sysTime::ConvertToDaysLocalTime(time);
					series->AddXY(dTime, val, "", clBlack);

					iNullDataCount = (dTime - lastSeriesDataTime) / (count_of_period_before_brake * fRateDays);
					if (iNullDataCount) {
						time = (data1->msecGMT - iRateMilliseconds) * sysTime::MSEC2SEC;
						series->AddNullXY(sysTime::ConvertToDaysLocalTime(time), val, "");

						time = (data1->msecGMT + iRateMilliseconds) * sysTime::MSEC2SEC;
						series->AddNullXY(sysTime::ConvertToDaysLocalTime(time), val, "");
					}

					lastSeriesDataTime = series->XValues->Value[series->XValues->Count - 1];
				}
			}

			/*

			//prepare test data for cheking gauss filter

			double *d = new double[iDataCount];
			for (int i = 0; i < iDataCount; ++i) {
				d[i] = 100 * sin(i * M_PI / 180.0) + rand() % 10;
			}


			for (int iData = 0; iData < iDataCount; ++iData) {
				data1 = (*data)[iData];
				time = data1->msecGMT * sysTime::MSEC2SEC;
				dTime = sysTime::ConvertToDaysLocalTime(time);

				series->AddXY(dTime, d[iData], "", clBlack);
			}

			delete []d;
			*/
		}

		/*
		double lastSeriesDataTime = 0;
		if (series->XValues->Count) {
			lastSeriesDataTime = series->XValues->Value[series->XValues->Count - 1];
		}

		for (int iData = 0; iData < iDataCount; ++iData) {
			data1 = (*data)[iData];
			val = data1->toDouble() * sensor->powerValue;
			time = data1->msecGMT * sysTime::MSEC2SEC;
			dTime = sysTime::ConvertToDaysLocalTime(time);
			series->AddXY(dTime, val, "", clBlack);

			iNullDataCount = (dTime - lastSeriesDataTime) / (count_of_period_before_brake * fRateDays);
			if (iNullDataCount) {
				time = (data1->msecGMT - iRateMilliseconds) * sysTime::MSEC2SEC;
				series->AddNullXY(sysTime::ConvertToDaysLocalTime(time), val, "");

				time = (data1->msecGMT + iRateMilliseconds) * sysTime::MSEC2SEC;
				series->AddNullXY(sysTime::ConvertToDaysLocalTime(time), val, "");
			}

			lastSeriesDataTime = series->XValues->Value[series->XValues->Count - 1];
		}
		*/
		/*
		TDateTime dt;
		for (int i=0; i < series->XValues->Count; ++i) {
			dt.Val = series->XValues->Value[i];
		}
        */
		//check middle datas
		/*
		for (int iData = 0; iData < iDataCount - 1; ++iData) {
			data1 = data->at(iData);
			data2 = data->at(iData + 1);

			val = data1->toDouble() * sensor->powerValue;
			if (val > this->maxYScrollValue) this->maxYScrollValue = val;
			if (val < this->minYScrollValue) this->minYScrollValue = val;

			time = data1->msecGMT * sysTime::MSEC2SEC;
			series->AddXY(sysTime::ConvertToDaysLocalTime(time), val, "", clBlack);

			iNullDataCount = (data2->msecGMT - data1->msecGMT) / (count_of_period_before_brake * iRateMilliseconds);
			if (iNullDataCount > 0) {
				//Установим разрыв, и запишем в качестве значений последнее найденное значение
				time = (data1->msecGMT - iRateMilliseconds) * sysTime::MSEC2SEC;
				series->AddNullXY(sysTime::ConvertToDaysLocalTime(time), val, "");

				time = (data1->msecGMT + iRateMilliseconds) * sysTime::MSEC2SEC;
				series->AddNullXY(sysTime::ConvertToDaysLocalTime(time), val, "");
			}
		}
		*/

		//add the last data point
		/*
		TData *data = data[iDataCount - 1];
		time = data->msecGMT * sysTime::MSEC2SEC;
		double timeLastDataPoint = series->XValues[series->XValues->Count];
		series->AddXY(sysTime::ConvertToDaysLocalTime(time), val, "", clBlack);

		iNullDataCount = (data2->msecGMT - data1->msecGMT) / (count_of_period_before_brake * iRateMilliseconds);
		if (iNullDataCount > 0) {
					time = (data1->msecGMT - iRateMilliseconds) * sysTime::MSEC2SEC;
			series->AddNullXY(sysTime::ConvertToDaysLocalTime(time), val, "");

			time = (data1->msecGMT + iRateMilliseconds) * sysTime::MSEC2SEC;
			series->AddNullXY(sysTime::ConvertToDaysLocalTime(time), val, "");
		}
		*/

	} __finally {
		StopProgressBar();
	}
}

#pragma package(smart_init)
