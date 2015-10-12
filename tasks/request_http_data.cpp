//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "request_http_data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//read from wStrHex sensor data into sensorData variable using sensor->type information
static bool PrepareSensorData(const TSensor *sensor, const wchar_t *wStrHex, TSensorData *sensorData);

//---------------------------------------------------------------------------
TTaskRequestHTTPData::TTaskRequestHTTPData() {
	device = NULL;
	http = new TIdHTTP();
}

//---------------------------------------------------------------------------
TTaskRequestHTTPData::~TTaskRequestHTTPData() {
	delete http;
}

//---------------------------------------------------------------------------
bool TTaskRequestHTTPData::operator == (const sysThread::TThreadTask *taskHTTPData) {
	return this->device == (static_cast<const TTaskRequestHTTPData *>(taskHTTPData))->device;
}

//---------------------------------------------------------------------------
bool TTaskRequestHTTPData::operator == (sysThread::TThreadTask *taskHTTPData) {
	return this->device == (static_cast<TTaskRequestHTTPData *>(taskHTTPData))->device;
}

//---------------------------------------------------------------------------
void TTaskRequestHTTPData::Run() {
	String url = DataModuleMP->sConfigURL + "/data/device/" + device->uuid;

	try {
		String httpResponse = http->Get(url);
		/*
		std::string data = sysFile::ReadFile(L"empty_data.json");
		String httpResponse = String(data.c_str());
		*/
		Parse(httpResponse);
	} catch (Exception &e) {
		sysLogger::ERR_W(e.ToString().c_str());
		http->Disconnect();

		try {
            sysLogger::ERR_A("trying to reconnecting");
			//trying to reconnect to server
			http->Connect(DataModuleMP->host, DataModuleMP->port);
		} catch (Exception &e) {
			sysLogger::ERR_W(e.ToString().c_str());
		}
	}
}

//---------------------------------------------------------------------------
void TTaskRequestHTTPData::Parse(String &httpResponse) {
	TJSONObject *jsonData = NULL;

	//mark all sensor as no data
	std::list<TSensor *> sensors = GetSensors(device);
	for (std::list<TSensor *>::iterator i = sensors.begin(), iEnd = sensors.end(); i != iEnd; ++i) {
		data[*i] = NULL;
	}

	try {
		jsonData = (TJSONObject *)TJSONObject::ParseJSONValue(httpResponse);
		if (!jsonData || jsonData->Null) {
			throw EHTTPDataNotRecive("json is missing");
		}

		TJSONObject *jsonContent = dynamic_cast<TJSONObject *>(sysJSON::Get(jsonData, L"content"));
		if (!jsonContent || jsonContent->Null) {
			throw EHTTPDataNotRecive("json content is missing");
		}

		TJSONNumber *jsonVersion = dynamic_cast<TJSONNumber *>(sysJSON::Get(jsonData, L"version"));
		if (!jsonVersion || jsonVersion->Null) {
			throw EHTTPVersionNotRecive("json version is missing");
		}

		String sensorUUID;
		String value;
		String ts;

		int sensorDataCount = jsonContent->Count;

		for (int k = 0; k < sensorDataCount; ++k) {
			sensorUUID = jsonContent->Pairs[k]->JsonString->Value();
			TSensor *sensor = GetSensor(sensorUUID);
			if (sensor == NULL) {
				continue;
			}

			TJSONObject *jsonValue = dynamic_cast<TJSONObject *>(jsonContent->Get(sensorUUID)->JsonValue);
			if (jsonValue == NULL || jsonValue->Null) {
				continue;
			}

			//retrive the sensor value as hex string
			value = jsonValue->Get("value")->JsonValue->Value();
			if (value == NULL || value.IsEmpty()) {
				continue;
			}

			//create and prepare sensor data
			TSensorData *sensorData = *(SENSOR_DATA_POOL.Get<TSensorData>());

			//skip 4 byte of file offset information
			if (PrepareSensorData(sensor, &(value.c_str()[8]), sensorData) == false) {
				SENSOR_DATA_POOL.Release(sensorData);
				continue;
			}

			ts = jsonValue->Get("ts")->JsonValue->Value();
			sensorData->timeGMT = StrToInt64(ts);

			data[sensor] = sensorData;
		}
	} catch (EHTTPDataNotRecive &e) {
		//disable this log, too many 'json content is missing' messages
	} catch (Exception &e) {
		if (sysLogger::CheckLogLevel(sysLogger::LOG_LEVEL_ERROR)) {
			String url = DataModuleMP->sConfigURL + "/data/device/" + device->uuid;
			sysLogger::WARN_W(url.c_str());
			sysLogger::WARN_W(httpResponse.c_str());
			sysLogger::WARN_W(e.Message.c_str());
		}
	}

	if (jsonData != NULL) {
		jsonData->Free();
		jsonData = NULL;
	}
}

//---------------------------------------------------------------------------
void TTaskRequestHTTPData::CleanUp() {
	for (std::map<const TSensor *, TSensorData *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		SENSOR_DATA_POOL.Release(i->second);
	}
	data.clear();

	device = NULL;
}

//---------------------------------------------------------------------------
/*
TTaskType TTaskRequestHTTPData::GetTaskType() {
	return TTaskType::HTTP;
}
*/

//---------------------------------------------------------------------------
int TTaskRequestHTTPData::GetObserverableType() {
	return TObserverableTypes::DEVICE_HTTP_DATA;
}

//---------------------------------------------------------------------------
bool PrepareSensorData(const TSensor *sensor, const wchar_t *wStrHex, TSensorData *sensorData) {
	//count of bytes, retrive from the value
	size_t byteCount = wcslen(wStrHex) / 2;
	if (byteCount <= 0/* || sensor->data_type == 8*/) {
		return false;
	}

	int *_value = new int[byteCount];

	for (size_t itByteValue = 0; itByteValue < byteCount; ++itByteValue) {
		swscanf(&wStrHex[itByteValue * 2], L"%2X", &_value[itByteValue]);
	}

	//create value for sensor data, correspond with data type
	if (sensor->data_type == DATA_TYPE_CHAR) {
		sensorData->cValue = (CHAR)(_value[0] & 0xFF);

	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
		sensorData->ucValue = (BYTE)(_value[0] & 0xFF);

	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
		sensorData->usValue = ((WORD)(_value[0] & 0xFF)) |
							  ((WORD)(_value[1] & 0xFF) << 8);

	} else if (sensor->data_type == DATA_TYPE_SHORT) {
		sensorData->sValue = ((SHORT)(_value[0] & 0xFF)) |
							  ((SHORT)(_value[1] & 0xFF) << 8);

	} else if (sensor->data_type == DATA_TYPE_LONG) {
		sensorData->lValue = ((LONG)(_value[0] & 0xFF)) |
							  ((LONG)(_value[1] & 0xFF) << 8) |
							  ((LONG)(_value[2] & 0xFF) << 16) |
							  ((LONG)(_value[3] & 0xFF) << 24);

	} else if (sensor->data_type == DATA_TYPE_INT) {
		sensorData->iValue = ((_value[0] & 0xFF)) |
							  ((_value[1] & 0xFF) << 8) |
							  ((_value[2] & 0xFF) << 16) |
							  ((_value[3] & 0xFF) << 24);

	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
		sensorData->uiValue = ((UINT)(_value[0] & 0xFF)) |
							  ((UINT)(_value[1] & 0xFF) << 8) |
							  ((UINT)(_value[2] & 0xFF) << 16) |
							  ((UINT)(_value[3] & 0xFF) << 24);

	} else if (sensor->data_type == DATA_TYPE_FLOAT) {
		float fValue;
		BYTE bytes[] = {
			(BYTE)(_value[0] & 0xFF),
			(BYTE)(_value[1] & 0xFF),
			(BYTE)(_value[2] & 0xFF),
			(BYTE)(_value[3] & 0xFF)
		};
		memcpy(&fValue, &bytes[0], sizeof(float));

		sensorData->fValue = fValue;

	} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
		double dValue;
		BYTE bytes[] = {
			(BYTE)(_value[0] & 0xFF),
			(BYTE)(_value[1] & 0xFF),
			(BYTE)(_value[2] & 0xFF),
			(BYTE)(_value[3] & 0xFF),
			(BYTE)(_value[4] & 0xFF),
			(BYTE)(_value[5] & 0xFF),
			(BYTE)(_value[6] & 0xFF),
			(BYTE)(_value[7] & 0xFF)
		};
		memcpy(&dValue, &bytes[0], sizeof(double));

		sensorData->dValue = dValue;

	} else if (sensor->data_type == DATA_TYPE_INT64) {
		sensorData->i64Value = ((INT64)(_value[0] & 0xFF)) |
							   ((INT64)(_value[1] & 0xFF) << 8) |
							   ((INT64)(_value[2] & 0xFF) << 16) |
							   ((INT64)(_value[3] & 0xFF) << 24) |
							   ((INT64)(_value[4] & 0xFF) << 32) |
							   ((INT64)(_value[5] & 0xFF) << 40) |
							   ((INT64)(_value[6] & 0xFF) << 48) |
							   ((INT64)(_value[7] & 0xFF) << 56);
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT64) {
		sensorData->ui64Value = ((UINT64)(_value[0] & 0xFF)) |
								((UINT64)(_value[1] & 0xFF) << 8) |
								((UINT64)(_value[2] & 0xFF) << 16) |
								((UINT64)(_value[3] & 0xFF) << 24) |
								((UINT64)(_value[4] & 0xFF) << 32) |
								((UINT64)(_value[5] & 0xFF) << 40) |
								((UINT64)(_value[6] & 0xFF) << 48) |
								((UINT64)(_value[7] & 0xFF) << 56);

	} else if (sensor->data_type == DATA_TYPE_STRING) {
		sensorData->strValue = new char[byteCount + 1];
		sensorData->strValue[byteCount] = 0;

		sensorData->size = byteCount;

		for (size_t i = 0; i < byteCount; ++i) {
			sprintf(&(sensorData->strValue[i]), "%c", _value[i]);
		}
	}

	delete []_value;

	return true;
}

