//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)

sysPatterns::TPool<TSensorData> SENSOR_DATA_POOL;

sysPatterns::TPool<TSensorEventData> SENSOR_EVENT_DATA_POOL;

TUser USER;

std::list<TCollectionPoint *> COLLECTION_POINTS;

std::list<TDevice *> DEVICES;

std::list<TSensor *> SENSORS;

std::list<TSensorBit *> SENSORS_BITS;

std::list<TDeviceTreeGroup *> DEVICE_TREE_GROUPS;

std::list<TMnemoshema *> MNEMOSHEMS;

std::map<String, TRecord *> RECORDS;

sysThread::TThreadPoolBorland<TThreadHTTPDataUISynchronize> *THREAD_POOL;

//sysPatterns::TPool<TTaskRequestFileData> *FILE_REQUEST_POOL;

sysPatterns::TPool<TTaskRequestHTTPData> *HTTP_REQUEST_POOL;

sysPatterns::TPool<TTaskRequestDeviceDataFileFormat> *DEVICE_DATA_FILE_FORMAT_REQUEST_POOL;

sysPatterns::TPool<TTaskRequestDeviceEventDataFileFormat> *DEVICE_EVENT_DATA_FILE_FORMAT_REQUEST_POOL;

sysPatterns::TPool<TTaskRequestMnemoshemaDataHistory> *MNEMOSHEMA_DATA_HISTORY_REQUEST_POOL;

//ptr function for request time range data using old file format (separate sensor data file for each sensor)
//or (seperate device data file for each divece)
RequestTimeRangeData *REQUEST_TIME_RANGE_DATA;

RequestMnemoshemaData *REQUEST_MNEMOSHEMA_DATA;

bool GAUSSIAN_FILTER_ENABLED;

bool MEDIAN_FILTER_ENABLED;


//---------------------------------------------------------------------------
std::list<TSensor *> GetSensors(const TDevice *device) {
	std::list<TSensor *> sensors;
	for (std::list<TSensor *>::iterator i = SENSORS.begin(), iEnd = SENSORS.end(); i != iEnd; ++i) {
		if ((*i)->device_id == device->id) {
			sensors.push_back(*i);
		}
	}

	return sensors;
}

//---------------------------------------------------------------------------
TSensor * GetSensor(const String &uuid) {
	TRecord *record = GetRecord(uuid);
	if (record == NULL) {
		return NULL;
	}
	if (record->record_type == TRecordType::RECORD_TYPE_SENSOR) {
		return (TSensor *)record;
	}

	return NULL;
}

//---------------------------------------------------------------------------
TSensor * GetSensor(int id){
	for (std::list<TSensor *>::iterator si = SENSORS.begin(); si != SENSORS.end(); si++) {
		if ((*si)->id == id) {
			return *si;
		}
	}

	return NULL;
}

// ---------------------------------------------------------------------------
TRecord * GetRecord(const String &uuid) {
	//increase speed of find record by uuid, when application is loading
	//mnemoshema try to find record by all mimic panel elements names for instance is TLabel.Name and other
	//form elements, thats why mnemoshema should search all records and do not find anything

	if (RECORDS.find(uuid) == RECORDS.end()) {
		return NULL;
	}

	return RECORDS[uuid];
}

// ---------------------------------------------------------------------------
TDevice * GetDevice(int id) {
	for (std::list<TDevice *>::iterator di = DEVICES.begin(); di != DEVICES.end(); di++) {
		if ((*di)->id == id) {
			return *di;
		}
	}

	return NULL;
}

// ---------------------------------------------------------------------------
TCollectionPoint * GetCollectionPoint(int id) {
	for (std::list<TCollectionPoint *>::iterator cpi = COLLECTION_POINTS.begin(); cpi != COLLECTION_POINTS.end(); cpi++) {
		if ((*cpi)->id == id) {
			return *cpi;
		}
	}

	return NULL;
}

// ---------------------------------------------------------------------------
double SensorDataToDouble(const TSensor *sensor, const TSensorData *sensorData) {
	if (sensor->data_type == DATA_TYPE_CHAR) {
		return (double)sensorData->cValue;
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
		return (double)sensorData->ucValue;
	} else if (sensor->data_type == DATA_TYPE_SHORT) {
		return (double)sensorData->sValue;
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
		return (double)sensorData->usValue;
	} else if (sensor->data_type == DATA_TYPE_INT) {
		return (double)sensorData->iValue;
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
		return (double)sensorData->uiValue;
	} else if (sensor->data_type == DATA_TYPE_LONG) {
		return (double)sensorData->lValue;
	} else if (sensor->data_type == DATA_TYPE_FLOAT) {
		return (double)sensorData->fValue;
	} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
		return sensorData->dValue;
	} else if (sensor->data_type == DATA_TYPE_INT64) {
		return sensorData->i64Value;
	}

	return 0;
}

// ---------------------------------------------------------------------------
__int64 SensorDataToInt64(const TSensor *sensor, const TSensorData *sensorData) {
	if (sensor->data_type == DATA_TYPE_CHAR) {
		return (__int64)sensorData->cValue;
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
		return (__int64)sensorData->ucValue;
	} else if (sensor->data_type == DATA_TYPE_SHORT) {
		return (__int64)sensorData->sValue;
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
		return (__int64)sensorData->usValue;
	} else if (sensor->data_type == DATA_TYPE_INT) {
		return (__int64)sensorData->iValue;
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
		return (__int64)sensorData->uiValue;
	} else if (sensor->data_type == DATA_TYPE_LONG) {
		return (__int64)sensorData->lValue;
	} else if (sensor->data_type == DATA_TYPE_FLOAT) {
		return 0;
	} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
		return 0;
	} else if (sensor->data_type == DATA_TYPE_INT64) {
		return sensorData->i64Value;
	}

	return 0;
}

// ---------------------------------------------------------------------------
String SensorDoubleDataToString(const TSensor *sensor, double data, bool displayUnit) {
	static wchar_t buff[255];
	static wchar_t format[10];

	if (displayUnit) {
		swprintf(format, L"%%.%df %%s%%s", sensor->precision);
		swprintf(buff, format, data, sensor->powerPrefix.c_str(), sensor->unitName.c_str());
	} else {
		swprintf(format, L"%%.%df", sensor->precision);
		swprintf(buff, format, data);
	}
	return buff;
}

// ---------------------------------------------------------------------------
String SensorDataToString(const TSensor *sensor, const TSensorData *sensorData, bool displayUnit) {
	if (sensorData != NULL) {
		if (sensor->data_type == DATA_TYPE_STRING) {
			return sensorData->strValue;
		} else {
			double number = SensorDataToDouble(sensor, sensorData) * sensor->powerValue;

			return SensorDoubleDataToString(sensor, number, displayUnit);
		}
	}

	return "";
}

// ------------------------------------------------------------------------------
String SensorBitDataToString(const TSensorBit *sensorBit, const TSensorData *sensorData) {
	if (sensorData == NULL) {
		return "";
	}

	const TSensor *sensor = sensorBit->sensor;
	if (sensor == NULL) {
		return "";
	}

	bool isText = sensorBit->view == "text";
	unsigned char value;

	__int64 i64 = SensorDataToInt64(sensor, sensorData);
	value = (i64 >> sensorBit->bit) & 1;

	if (isText) {
		return (value == 1) ? sensorBit->value1 : sensorBit->value0;
	} else {
		return IntToStr(value);
	}


	return "";
}

// ------------------------------------------------------------------------------
char SensorBitDataToByte(const TSensorBit *sensorBit, const TSensorData *sensorData) {
	if (sensorData == NULL) {
		return -1;
	}

	const TSensor *sensor = sensorBit->sensor;
	if (sensor == NULL) {
		return -1;
	}

	return (SensorDataToInt64(sensor, sensorData) >> sensorBit->bit) & 1;
}

// ------------------------------------------------------------------------------
/*
String GetSensorFormattedName(const TSensor *sensor) {
	int unit = sensor->unit;
	TBrizAnalogMode mode = sensor->unit_mode;
	if (mode == TBrizAnalogMode::amdNone){
		return sensor->name;
	}
	if (mode == TBrizAnalogMode::amdDirect){
		return GetUnitNameEn(unit) + " - " + sensor->name;
	} else if (mode == TBrizAnalogMode::amdPhaseA || mode == TBrizAnalogMode::amdPhaseB || mode == TBrizAnalogMode::amdPhaseC){
		if (unit == UNIT_HZ){
			return GetUnitNameEn(unit) + "* - " + sensor->name;
		}
		return GetUnitNameEn(unit) + GetUnitModeName(mode) + " - " + sensor->name;
	} else {
		if (unit == UNIT_VT || unit == UNIT_VAR || unit == UNIT_HZ){
			return GetUnitNameEn(unit) + "* - " + sensor->name;
		}
		if (mode == TBrizAnalogMode::amdSequence0 && (unit == UNIT_V || unit == UNIT_A)){
			return "3" + GetUnitNameEn(unit) + GetUnitModeName(mode) + " - " + sensor->name;
		}
		return GetUnitNameEn(unit) + GetUnitModeName(mode) + " - " + sensor->name;
	}
}
*/

//---------------------------------------------------------------------------
/*
extract first file name from folder
@param folder
@param fileNameLength criteria of folder scan procedure
@param wFile output parameter, memmory must be allocated before execute function
@return true if file was found
*/
bool GetFileByFileNameLength(const wchar_t *folder, size_t fileNameLength, wchar_t *wDataFile) {
	WIN32_FIND_DATAW ffd;

	HANDLE hFind = INVALID_HANDLE_VALUE;

	wchar_t wBuffer[MAX_PATH];
	wmemset(&wBuffer[0], 0, MAX_PATH * sizeof(wchar_t));

	swprintf(wBuffer, MAX_PATH, L"%s\\*", folder);

	hFind = FindFirstFileW(wBuffer, &ffd);
	if (hFind == INVALID_HANDLE_VALUE) return false;

	bool res = false;
	bool isFolder;
	do {
		isFolder = ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
		if (isFolder) {
			continue;
		}

		if (wcslen(ffd.cFileName) != fileNameLength) {
			continue;
		}

		wcscpy(wDataFile, ffd.cFileName);
		res = true;

		break;
	} while (FindNextFileW(hFind, &ffd));

	FindClose(hFind);

	return res;
}

//---------------------------------------------------------------------------
//find first data value, where millisecond more or eq ms
int FindPositionByteOffsetInDataFile(FILE *f, size_t fileSize, size_t dataSize, long ms) {
	size_t iDataChunkCount = fileSize / dataSize;
	size_t left = 0;
	size_t right = iDataChunkCount;

	size_t position;
	long milliseconds;

	while ((right - left) > 1) {
		position = (right + left) / 2;
		fseek(f, position * dataSize, SEEK_SET);
		fread(&milliseconds, sizeof(milliseconds), 1, f);

		if (milliseconds < ms) {
			left = position;
		} else if (milliseconds > ms) {
			right = position;
		} else {
			return position * dataSize;
		}
	}

	if (position > 0) {
		//check is left border can be eq ms
		fseek(f, (position - 1) * dataSize, SEEK_SET);
		fread(&milliseconds, sizeof(milliseconds), 1, f);
		if (milliseconds == ms) {
			return (position - 1) * dataSize;
		}
	}

	if (position < (iDataChunkCount - 1)) {
		//check is right border can be eq ms
		fseek(f, (position + 1) * dataSize, SEEK_SET);
		fread(&milliseconds, sizeof(milliseconds), 1, f);
		if (milliseconds == ms) {
			return (position - 1) * dataSize;
		}
	}

	return position * dataSize;
}

//---------------------------------------------------------------------------
void SetSensorData(const TSensor *sensor, TSensorData *sensorData, void *data) {
	if (sensor->data_type == DATA_TYPE_CHAR) {
    	sensorData->cValue = *(char *)(data);
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
		sensorData->ucValue = *(unsigned char *)(data);
	} else if (sensor->data_type == DATA_TYPE_SHORT) {
		sensorData->sValue = *(short *)(data);
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
		sensorData->usValue = *(unsigned short *)(data);
	} else if (sensor->data_type == DATA_TYPE_INT) {
		sensorData->iValue = *(int *)(data);
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
		sensorData->uiValue = *(unsigned int *)(data);
	} else if (sensor->data_type == DATA_TYPE_LONG) {
		sensorData->lValue = *(long *)(data);
	} else if (sensor->data_type == DATA_TYPE_FLOAT) {
		sensorData->fValue = *(float *)(data);
	} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
		sensorData->dValue = *(double *)(data);
	} else if (sensor->data_type == DATA_TYPE_INT64) {
		sensorData->i64Value = *(__int64 *)(data);
	}
}

//---------------------------------------------------------------------------
TSensorData * CreateSensorDataFromBytes(const TSensor *sensor, BYTE *bytes, __int64 time) {
	TSensorData *sensorData = *(SENSOR_DATA_POOL.Get<TSensorData>());

	long milliseconds;
	memcpy(&milliseconds, &bytes[0], sizeof(milliseconds));
	sensorData->timeGMT = time + (__int64)milliseconds;

	if (sensor->data_type == DATA_TYPE_CHAR) {
		memcpy(&(sensorData->cValue), &bytes[4], sizeof(char));
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
		memcpy(&(sensorData->ucValue), &bytes[4], sizeof(unsigned char));
	} else if (sensor->data_type == DATA_TYPE_SHORT) {
		memcpy(&(sensorData->sValue), &bytes[4], sizeof(short));
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
		memcpy(&(sensorData->usValue), &bytes[4], sizeof(unsigned short));
	} else if (sensor->data_type == DATA_TYPE_INT) {
		memcpy(&(sensorData->iValue), &bytes[4], sizeof(int));
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
		memcpy(&(sensorData->uiValue), &bytes[4], sizeof(unsigned int));
	} else if (sensor->data_type == DATA_TYPE_LONG) {
		memcpy(&(sensorData->lValue), &bytes[4], sizeof(long));
	} else if (sensor->data_type == DATA_TYPE_FLOAT) {
		memcpy(&(sensorData->fValue), &bytes[4], sizeof(float));
	} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
		memcpy(&(sensorData->dValue), &bytes[4], sizeof(double));
	} else if (sensor->data_type == DATA_TYPE_INT64) {
		memcpy(&(sensorData->i64Value), &bytes[4], sizeof(__int64));
	} else if (sensor->data_type == DATA_TYPE_STRING) {
		sensorData->strValue = new char[sensor->size + 1];
		memcpy(&(sensorData->strValue[0]), &bytes[4], sensor->size);
		sensorData->strValue[sensor->size] = 0;
	}

	return sensorData;
}

//---------------------------------------------------------------------------
TSensorEventData * CreateSensorEventDataFromBytes(BYTE *bytes, __int64 time, size_t bytesSize, size_t dataSize) {
    unsigned char eventType;
	memcpy(&eventType,     &bytes[8],  sizeof(eventType));
	unsigned char eventType1 = eventType & 0xF;

	if (eventType1 != event::TSensorEventType::RECORD_START) {
		return NULL;
	}

	TSensorEventData *sensorEventData = *(SENSOR_EVENT_DATA_POOL.Get<TSensorEventData>());

	long sensorIndex;
	long milliseconds;

	unsigned char compareType;
	unsigned char compareResult;
	unsigned char dataType;

	memcpy(&sensorIndex,   &bytes[0],  sizeof(sensorIndex));
	memcpy(&milliseconds,  &bytes[4],  sizeof(milliseconds));

	unsigned char eventType0 = eventType >> 4;

	if (eventType1 == event::TSensorEventType::SETPOINT) {
		memcpy(&compareType,   &bytes[9],  sizeof(compareType));
		memcpy(&compareResult, &bytes[10], sizeof(compareResult));
		memcpy(&dataType,      &bytes[11], sizeof(dataType));

		if (dataType/*sensor->alarm_type*/ == ALARM_DATA_TYPE_INT64) {
			memcpy(&sensorEventData->i64Value, &bytes[12], sizeof(__int64));

		} else if (dataType/*sensor->alarm_type*/ == ALARM_DATA_TYPE_DOUBLE) {
			memcpy(&sensorEventData->dValue, &bytes[12], sizeof(double));

		} else if (dataType/*sensor->alarm_type*/ == ALARM_DATA_TYPE_STRING) {
			memcpy(&sensorEventData->strValue, &bytes[12], 8 * sizeof(char));
		}

		sensorEventData->compareType = (event::TSensorEventCompareType)compareType;
		sensorEventData->compareResult = (event::TSensorEventCompareResult)compareResult;
		sensorEventData->dataType = (event::TSensorEventDataType)dataType;

	} else if (eventType1 == event::TSensorEventType::EXTERNAL) {

	} else if (eventType1 == event::TSensorEventType::RECORD_START) {

		//1. read count of bytes
		memcpy(&(sensorEventData->bufferSize), &bytes[12], sizeof(__int64));
		sensorEventData->buffer = new char[11 * ceil(sensorEventData->bufferSize / 11.0)];

		size_t readedBytes = 0;
		size_t index = 1;

		while (readedBytes < sensorEventData->bufferSize) {
			if ((index * dataSize + 9) >= bytesSize) {
				assert(0);
			}

			//next should by RECORD_CONTINUE
			memcpy(&sensorEventData->buffer[readedBytes], &bytes[index * dataSize + 9], 11 * sizeof(char));
			readedBytes += 11;
			index++;
		}
	}

	sensorEventData->sensorIndex = sensorIndex;
	sensorEventData->timeGMT = time + (__int64)milliseconds;
	sensorEventData->eventType0 = (event::TSensorEventType)eventType0;
	sensorEventData->eventType1 = (event::TSensorEventType)eventType1;

	return sensorEventData;
}

/*
void ResizeValarray(std::valarray<double> &x, std::valarray<double> &y, size_t newDataSize) {
	if (x.size() <= newDataSize) {
		std::valarray<double> cpy = x[std::slice(0, x.size(), 1)];

		x.resize(newDataSize * 2);
		for (size_t i = 0, len = cpy.size(); i < len; ++i) {
			x[i] = cpy[i];
		}

		cpy = y[std::slice(0, y.size(), 1)];

		y.resize(newDataSize * 2);
		for (size_t i = 0, len = cpy.size(); i < len; ++i) {
			y[i] = cpy[i];
		}
	}
}
*/

/*
//---------------------------------------------------------------------------
void ConvertSensorDataToValarray(const TSensor *sensor, std::list<TSensorData *> *data, std::valarray<double> &x, std::valarray<double> &y, size_t &dataSize) {
	dataSize = 0;

	if (data == NULL) {
		return;
	}

	size_t size = data->size();
	if (size == 0) {
		return;
	}

    ResizeValarray(x, y, size);

	dataSize = size;

	size_t i = 0;
	if (sensor->data_type == DATA_TYPE_CHAR) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			x[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			y[i] = (*it)->cValue;
		}
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			x[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			y[i] = (*it)->ucValue;
		}
	} else if (sensor->data_type == DATA_TYPE_SHORT) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			x[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			y[i] = (*it)->sValue;
		}
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			x[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			y[i] = (*it)->usValue;
		}
	} else if (sensor->data_type == DATA_TYPE_INT) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			x[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			y[i] = (*it)->iValue;
		}
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			x[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			y[i] = (*it)->uiValue;
		}
	} else if (sensor->data_type == DATA_TYPE_LONG) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			x[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			y[i] = (*it)->lValue;
		}
	} else if (sensor->data_type == DATA_TYPE_FLOAT) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			x[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			y[i] = (*it)->fValue;
		}
	} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			x[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			y[i] = (*it)->dValue;
		}
	} else if (sensor->data_type == DATA_TYPE_INT64) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			x[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			y[i] = (*it)->i64Value;
		}
	}
}
*/

/*
//---------------------------------------------------------------------------
void AppendSensorDataToValarray(const TSensor *sensor, TSensorData *data, std::valarray<double> &x, std::valarray<double> &y, size_t &dataSize) {
	if (data == NULL) {
		return;
	}

	ResizeValarray(x, y, dataSize + 1);

	x[dataSize] = sysTime::ConvertToDaysLocalTime(data->timeGMT * sysTime::MSEC2SEC);

	y[dataSize] = SensorDataToDouble(sensor, data);

	++dataSize;
}
*/

//---------------------------------------------------------------------------
int BinSearchValueList(TChartValueList *A,  double key, int low, int high, double exp) {
	if ( low > high ){
	  return -1;
	};

	int mid = ( low + high ) / 2;
	if (fabs(A->Value[mid] - key) <= exp) {
		return mid;
	}
	if ( A->Value[mid] < key) return BinSearchValueList(A, key, mid+1, high, exp);
	if ( A->Value[mid] > key ) return BinSearchValueList(A, key, low, mid-1, exp);

	return -1;
}

//---------------------------------------------------------------------------
String FindFilePath(const String &deviceUuid, const String &date, const String &file) {
	String centraDBEventFile = DataModuleMP->sPathToCentralDB + "\\" + deviceUuid + "\\" + date + "\\" + file;
	if (sysFile::IsFileExist(centraDBEventFile.c_str())) {
		return centraDBEventFile;

	} else if (DataModuleMP->isArchiveDBExist == true) {
		String archiveDBEventFile = DataModuleMP->sPathToArchiveDB + "\\" + deviceUuid + "\\" + date + "\\" + file;
		if (sysFile::IsFileExist(archiveDBEventFile.c_str())) {
			return archiveDBEventFile;
		}
	}

	return "";
}
