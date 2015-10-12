//---------------------------------------------------------------------------


#pragma hdrstop

#include "RecordSensorBitViewChart.h"

//---------------------------------------------------------------------------
TRecordSensorBitViewChart::TRecordSensorBitViewChart(TWinControl *owner, TRecord *record, TUser *user, void (*SetChartSeriesSetings)(TLineSeries *, TUser *, String)) : TRecordViewChart(owner, record, user, SetChartSeriesSetings) {
	Title->Text->Text = GetName();
	TSensorBit *sensorBit = static_cast<TSensorBit *>(record);

	if (sensorBit->view == "text") {
		sensorBitValues[1] = sensorBit->value1;
		sensorBitValues[-1] = sensorBit->value0;
	}

	if (series) {
		series->OnGetMarkText = SeriesGetMarkText;
	}

    //specify default Y scale for discrete sensor
    LeftAxis->SetMinMax(0, 1.1);

	count_of_period_before_brake = DataModuleMP->GetSettingInt("count_of_period_before_brake");

    //TODO I don't known how but we need to specify horizontal Grid line only for 0 and 1 value
    //LeftAxis->Grid->Visible = true;
    //LeftAxis->Grid->DrawEvery = 10;
}

//---------------------------------------------------------------------------
String __fastcall TRecordSensorBitViewChart::GetName(){
	//TSensorBit *sensorBit = static_cast<TSensorBit *>(record);
	//return sensor->name + " " + sensorBit->desc;
    return GetSensorBitTreeItemName((TSensorBit *)record);
}

//---------------------------------------------------------------------------
inline __fastcall TRecordSensorBitViewChart::~TRecordSensorBitViewChart(void){
};

//---------------------------------------------------------------------------
void TRecordSensorBitViewChart::DisplayMiddleDataAsString(std::vector<TData *> *data, bool bClearSeries){
	int iDataCount = data->size();
	if (!iDataCount) return;

	TData *data1, *data2;
	int iVal, iPrevVal;
	time_t time, prevTime;
	int iNullDataCount;
	long iRateMilliseconds = this->sensor->rate;
	double dRateSeconds = iRateMilliseconds / 1000.0;
	TSensorBit *sensorBit = dynamic_cast<TSensorBit *>(record);
	double dTimeBegin = sysTime::ConvertToDaysLocalTime(data->at(0)->msecGMT * sysTime::MSEC2SEC);
	double dTimeEnd;
	//series->Marks->Visible = true;
	//series->Marks->Style = smsLabel;
	series->XValues->DateTime = true;

	//Update Sensor bit value labels
	TLabelBitValue *labelBitValue;
	for (int ctrl = 0; ctrl < ControlCount; ctrl++) {
		labelBitValue = dynamic_cast<TLabelBitValue *>(Controls[ctrl]);
		if (labelBitValue) delete labelBitValue;
	}

	//check middle datas
	for (int iData = 0; iData < iDataCount - 1; ++iData) {
		data1 = data->at(iData);
		data2 = data->at(iData + 1);

		if (sensor->data_type == DATA_TYPE_CHAR) {
			std::bitset<8> bits(((TDataFile<char> *)data1)->data);
			iVal = (bits.test(sensorBit->bit))?1:0;

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
			std::bitset<8> bits(((TDataFile<unsigned char> *)data1)->data);
			iVal = (bits.test(sensorBit->bit))?1:0;

		} else if (sensor->data_type == DATA_TYPE_SHORT) {
			std::bitset<16> bits(((TDataFile<short> *)data1)->data);
			iVal = (bits.test(sensorBit->bit))?1:0;

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
			std::bitset<16> bits(((TDataFile<unsigned short> *)data1)->data);
			iVal = (bits.test(sensorBit->bit))?1:0;

		} else if (sensor->data_type == DATA_TYPE_INT) {
			std::bitset<32> bits(((TDataFile<int> *)data1)->data);
			iVal = (bits.test(sensorBit->bit))?1:0;

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
			std::bitset<32> bits(((TDataFile<unsigned int> *)data1)->data);
			iVal = (bits.test(sensorBit->bit))?1:0;

		} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
			throw EWrongDisplayType("Ќеверное отображение бита данных значени€ double");

		} else if (sensor->data_type == DATA_TYPE_FLOAT) {
			throw EWrongDisplayType("Ќеверное отображение бита данных значени€ float");

		} else if (sensor->data_type == DATA_TYPE_INT64) {
			std::bitset<64> bits(((TDataFile<__int64> *)data1)->data);
			iVal = (bits.test(sensorBit->bit))?1:0;

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT64) {
			std::bitset<64> bits(((TDataFile<unsigned __int64> *)data1)->data);
			iVal = (bits.test(sensorBit->bit))?1:0;

		} else if (sensor->data_type == DATA_TYPE_STRING) {
			throw EWrongDisplayType("Ќеверное отображение бита данных значени€ string");
		}

		time = data1->msecGMT * sysTime::MSEC2SEC;
		if (iData > 0) {
			String markTitle = "";
			if (iPrevVal != iVal) {
				markTitle = sensorBitValues[iVal];
			}
			//series->AddXY(ConvertToDaysLocalTime(time), iPrevVal, markTitle, clBlack);
			series->AddXY(sysTime::ConvertToDaysLocalTime(time), iPrevVal);
		}

		//series->AddXY(ConvertToDaysLocalTime(time), iVal, NULL, clBlack);
		series->AddXY(sysTime::ConvertToDaysLocalTime(time), iVal);

		iNullDataCount = (data2->msecGMT - data1->msecGMT) / (count_of_period_before_brake * iRateMilliseconds);
		if (iNullDataCount > 0) {
			time = (data1->msecGMT + iRateMilliseconds) * sysTime::MSEC2SEC;
			//series->AddXY(ConvertToDaysLocalTime(time), 0, "", clBlack);
			series->AddXY(sysTime::ConvertToDaysLocalTime(time), 0);

			time = (data2->msecGMT - iRateMilliseconds) * sysTime::MSEC2SEC;
			//series->AddXY(ConvertToDaysLocalTime(time), 0, "", clBlack);
			series->AddXY(sysTime::ConvertToDaysLocalTime(time), 0);
		}

		prevTime = time;
		iPrevVal = iVal;
	}
	/*
	if(this->BottomAxis->Minimum == this->BottomAxis->Maximum || this->followBySignal){
		RestoreChartZoom(dtGMT1, dtGMT2);
	}
	*/
}

void TRecordSensorBitViewChart::DisplayMiddleDataAs01(std::vector<TData *> *data, bool bClearSeries) {
	int iDataCount = data->size();
	TData *data1, *data2;
	double val, prevVal;
	time_t time, prevTime;
	int iNullDataCount;
	long iRateMilliseconds = this->sensor->rate;
	double dRateSeconds = iRateMilliseconds / 1000.0;
	TSensorBit *sensorBit = dynamic_cast<TSensorBit *>(record);

	//check middle datas
	for (int iData = 0; iData < iDataCount - 1; ++iData) {
		data1 = data->at(iData);
		data2 = data->at(iData + 1);

		if (sensor->data_type == DATA_TYPE_CHAR) {
			std::bitset<8> bits(((TDataFile<char> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = 1;
			else val = 0;

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
			std::bitset<8> bits(((TDataFile<unsigned char> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = 1;
			else val = 0;

		} else if (sensor->data_type == DATA_TYPE_SHORT) {
			std::bitset<16> bits(((TDataFile<short> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = 1;
			else val = 0;

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
			std::bitset<16> bits(((TDataFile<unsigned short> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = 1;
			else val = 0;

		} else if (sensor->data_type == DATA_TYPE_INT) {
			std::bitset<32> bits(((TDataFile<int> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = 1;
			else val = 0;

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
			std::bitset<32> bits(((TDataFile<unsigned int> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = 1;
			else val = 0;

		} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
			throw EWrongDisplayType("Ќеверное отображение бита данных значени€ double");

		} else if (sensor->data_type == DATA_TYPE_FLOAT) {
			throw EWrongDisplayType("Ќеверное отображение бита данных значени€ float");

		} else if (sensor->data_type == DATA_TYPE_INT64) {
			std::bitset<64> bits(((TDataFile<__int64> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = 1;
			else val = 0;

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT64) {
			std::bitset<64> bits(((TDataFile<unsigned __int64> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = 1;
			else val = 0;

		} else if (sensor->data_type == DATA_TYPE_STRING) {
			throw EWrongDisplayType("Ќеверное отображение бита данных значени€ string");
		}

		time = data1->msecGMT * sysTime::MSEC2SEC;
		if (iData > 0) {
			series->AddXY(sysTime::ConvertToDaysLocalTime(time), prevVal, "", clBlack);
		}
		series->AddXY(sysTime::ConvertToDaysLocalTime(time), val, "", clBlack);

		iNullDataCount = (data2->msecGMT - data1->msecGMT) / (count_of_period_before_brake * iRateMilliseconds);
		if (iNullDataCount > 0) {
			time = (data1->msecGMT + iRateMilliseconds) * sysTime::MSEC2SEC;
			series->AddXY(sysTime::ConvertToDaysLocalTime(time), 0, "", clBlack);

			time = (data2->msecGMT - iRateMilliseconds) * sysTime::MSEC2SEC;
			series->AddXY(sysTime::ConvertToDaysLocalTime(time), 0, "", clBlack);
		}

		prevTime = time;
		prevVal = val;
	}
}

//---------------------------------------------------------------------------
void TRecordSensorBitViewChart::DisplayMiddleDataAsNumeric(std::vector<TData *> *data, bool bClearSeries){
	int iDataCount = data->size();
	TData *data1, *data2;
	double val, prevVal;
	time_t time, prevTime;
	int iNullDataCount;
	long iRateMilliseconds = this->sensor->rate;
	double dRateSeconds = iRateMilliseconds / 1000.0;
	TSensorBit *sensorBit = dynamic_cast<TSensorBit *>(record);

	//check middle datas
	for (int iData = 0; iData < iDataCount - 1; ++iData) {
		data1 = data->at(iData);
		data2 = data->at(iData + 1);

		if (sensor->data_type == DATA_TYPE_CHAR) {
			std::bitset<8> bits(((TDataFile<char> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = _wtol(EmptyTo(sensorBit->value1, L"1").c_str()) ;
			else val = _wtol(EmptyTo(sensorBit->value0, L"0").c_str());

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
			std::bitset<8> bits(((TDataFile<unsigned char> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = _wtol(EmptyTo(sensorBit->value1, L"1").c_str());
			else val = _wtol(EmptyTo(sensorBit->value0, L"0").c_str());

		} else if (sensor->data_type == DATA_TYPE_SHORT) {
			std::bitset<16> bits(((TDataFile<short> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = _wtol(EmptyTo(sensorBit->value1, L"1").c_str());
			else val = _wtol(EmptyTo(sensorBit->value0, L"0").c_str());

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
			std::bitset<16> bits(((TDataFile<unsigned short> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = _wtol(EmptyTo(sensorBit->value1, L"1").c_str());
			else val = _wtol(EmptyTo(sensorBit->value0, L"0").c_str());

		} else if (sensor->data_type == DATA_TYPE_INT) {
			std::bitset<32> bits(((TDataFile<int> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = _wtol(EmptyTo(sensorBit->value1, L"1").c_str());
			else val = _wtol(EmptyTo(sensorBit->value0, L"0").c_str());

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
			std::bitset<32> bits(((TDataFile<unsigned int> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = _wtol(EmptyTo(sensorBit->value1, L"1").c_str());
			else val = _wtol(EmptyTo(sensorBit->value0, L"0").c_str());

		} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
			throw EWrongDisplayType("Ќеверное отображение бита данных значени€ double");

		} else if (sensor->data_type == DATA_TYPE_FLOAT) {
			throw EWrongDisplayType("Ќеверное отображение бита данных значени€ float");

		} else if (sensor->data_type == DATA_TYPE_INT64) {
			std::bitset<64> bits(((TDataFile<__int64> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = _wtol(EmptyTo(sensorBit->value1, L"1").c_str());
			else val = _wtol(EmptyTo(sensorBit->value0, L"0").c_str());

		} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT64) {
			std::bitset<64> bits(((TDataFile<unsigned __int64> *)data1)->data);
			if (bits.test(sensorBit->bit)) val = _wtol(EmptyTo(sensorBit->value1, L"1").c_str());
			else val = _wtol(EmptyTo(sensorBit->value0, L"0").c_str());
		} else if (sensor->data_type == DATA_TYPE_STRING) {
			throw EWrongDisplayType("Ќеверное отображение бита данных значени€ string");
		}

		time = data1->msecGMT * sysTime::MSEC2SEC;
		if (iData > 0) {
			series->AddXY(sysTime::ConvertToDaysLocalTime(time), prevVal, "", clBlack);
		}
		series->AddXY(sysTime::ConvertToDaysLocalTime(time), val, "", clBlack);

		iNullDataCount = (data2->msecGMT - data1->msecGMT) / (count_of_period_before_brake * iRateMilliseconds);
		if (iNullDataCount > 0) {
			time = (data1->msecGMT + iRateMilliseconds) * sysTime::MSEC2SEC;
			series->AddXY(sysTime::ConvertToDaysLocalTime(time), 0, "", clBlack);

			time = (data2->msecGMT - iRateMilliseconds) * sysTime::MSEC2SEC;
			series->AddXY(sysTime::ConvertToDaysLocalTime(time), 0, "", clBlack);
		}

		prevTime = time;
		prevVal = val;
	}
}

//---------------------------------------------------------------------------
void TRecordSensorBitViewChart::DisplayData(std::vector<TData *> *data, bool bClearSeries){
	try {
		TRecordViewChart::DisplayData(data, bClearSeries);

		TSensorBit *sensorBit = static_cast<TSensorBit *>(record);
		if (!sensorBit) return;

		vector<TData *>::iterator itData = data->begin();
		long milliseconds;
		WORD hour, min, sec, msec;
		char dataCaption[10];
		int iCurrentData = 0;
		int iDataCount = data->size();
		long msec1, msec2;
		static long iLocalTimeBiasSeconds = sysTime::GetLocalTimeBias();
		int iNullDataCount;
		struct tm *timeinfo;
		long iRateMilliseconds = this->sensor->rate;
		double dRateSeconds = iRateMilliseconds * sysTime::MSEC2SEC;// / 1000.0;
		time_t time;


		//if no data
		/*
		if (!iDataCount){
			iNullDataCount = (iSecondsGMT2 - iSecondsGMT1) / dRateSeconds;
			for (long iCnt = 0; iCnt < iNullDataCount; ++iCnt) {
				time = iCnt * dRateSeconds + iSecondsGMT1;
				series->AddXY(ConvertToDaysLocalTime(time), 0, "", clBlack);
			}
		}
		*/
		if (sensorBit->view == "numeric") {
			DisplayMiddleDataAsNumeric(data, bClearSeries);
		} else if (sensorBit->view == "text" || sensorBit->view == "edit") {
			DisplayMiddleDataAsString(data, bClearSeries);
		} else if (sensorBit->view == "image") {
			DisplayMiddleDataAs01(data, bClearSeries);
		}
		/*
		if(BottomAxis->Minimum == BottomAxis->Maximum || followBySignal){
			RestoreChartZoom(dtGMT1, dtGMT2);
		}
		*/

	} __finally {
		StopProgressBar();
	}

	this->minYScrollValue = this->LeftAxis->Minimum;

	this->maxYScrollValue = this->LeftAxis->Maximum;
}

void __fastcall TRecordSensorBitViewChart::SeriesGetMarkText(TChartSeries *Sender, int ValueIndex, String &MarkText){
	if (MarkText != sensorBitValues[1] && MarkText != sensorBitValues[-1]) {
		MarkText = "";
	}
}

#pragma package(smart_init)
