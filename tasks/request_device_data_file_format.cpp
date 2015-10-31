//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "request_device_data_file_format.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TTaskRequestDeviceDataFileFormat::TTaskRequestDeviceDataFileFormat() {
	sensors.clear();

	resolution = 0.0;
}

//---------------------------------------------------------------------------
TTaskRequestDeviceDataFileFormat::~TTaskRequestDeviceDataFileFormat() {
}

//---------------------------------------------------------------------------
bool TTaskRequestDeviceDataFileFormat::operator == (const sysThread::TThreadTask * task) {
	return device == (static_cast<const TTaskRequestDeviceDataFileFormat *>(task))->device;
	//return sensor == (static_cast<const TTaskRequestDeviceDataFileFormat *>(task))->sensor;
	//TODO compare all sensors in task and current object
	//return false;
}

//---------------------------------------------------------------------------
bool TTaskRequestDeviceDataFileFormat::operator == (sysThread::TThreadTask * task) {
	return device == (static_cast<TTaskRequestDeviceDataFileFormat *>(task))->device;
	//return sensor == (static_cast<TTaskRequestDeviceDataFileFormat *>(task))->sensor;
	//TODO compare all sensors in task and current object
	//return false;
}

//---------------------------------------------------------------------------
//for search the data file in central or archive db folders
static wchar_t * FindDataFile(
	wchar_t *wDbCentralPath,
	wchar_t *wDbArchivePath,
	const TDateTime &dtGMT,
	const int &centralDbOffset,
	const int &archiveDbOffset) {

	WORD year, month, day;
	dtGMT.DecodeDate(&year, &month, &day);

	wchar_t *pDataFile = NULL;

	//prepare the folder in central db
	swprintf(&wDbCentralPath[centralDbOffset], L"\\%4d%02d%02d\\", year, month, day);

	int date = ((int)year)*10000 + ((int)month)*100 + (int)day;
	dataFileLocation::TDataFileLocation location;

	//trying to increase the find the data file folder using the db for store it
	//String dataFile = DataModuleMP->FindDataFile(sensor, date, location);

	String dataFile = "";

	if (dataFile.IsEmpty() || sysFile::IsFileExist(dataFile.c_str()) == false) {
		if (sysFile::IsFolderExist(wDbCentralPath)) {
			if (GetFileByFileNameLength(wDbCentralPath, LEN_DATA_FILE_NAME, &wDbCentralPath[centralDbOffset + 10])) {
				//data file was found
				pDataFile = &wDbCentralPath[0];

				//DataModuleMP->SaveDataFile(sensor, date, pDataFile, dataFileLocation::DCS);
			}
		}

		if (pDataFile == NULL) {
			//prepare the folder in archive db
			swprintf(&wDbArchivePath[archiveDbOffset], L"\\%4d%02d%02d\\", year, month, day);
			if (sysFile::IsFolderExist(wDbArchivePath)) {
				if (GetFileByFileNameLength(wDbArchivePath, LEN_DATA_FILE_NAME, &wDbArchivePath[archiveDbOffset + 10])) {
					pDataFile = &wDbArchivePath[0];

					//DataModuleMP->SaveDataFile(sensor, date, pDataFile, dataFileLocation::ARCHIVE);
				}
			}
		}
	} else {
		if (location == dataFileLocation::DCS) {
			swprintf(wDbCentralPath, L"%s", dataFile.c_str());
			pDataFile = &wDbCentralPath[0];
		} else if (location == dataFileLocation::ARCHIVE) {
			swprintf(wDbArchivePath, L"%s", dataFile.c_str());
			pDataFile = &wDbArchivePath[0];
		}
	}

	return pDataFile;
}

//---------------------------------------------------------------------------
inline TSensorData * CreateSensorDataFromBytes(int data_type, int size, BYTE *bytes, __int64 time, __int64 milliseconds) {
	TSensorData *sensorData = *(SENSOR_DATA_POOL.Get<TSensorData>());

	sensorData->timeGMT = time + milliseconds;

	if (data_type == DATA_TYPE_CHAR) {
		memcpy(&(sensorData->cValue), bytes, sizeof(char));
	} else if (data_type == DATA_TYPE_UNSIGNED_CHAR) {
		memcpy(&(sensorData->ucValue), bytes, sizeof(unsigned char));
	} else if (data_type == DATA_TYPE_SHORT) {
		memcpy(&(sensorData->sValue), bytes, sizeof(short));
	} else if (data_type == DATA_TYPE_UNSIGNED_SHORT) {
		memcpy(&(sensorData->usValue), bytes, sizeof(unsigned short));
	} else if (data_type == DATA_TYPE_INT) {
		memcpy(&(sensorData->iValue), bytes, sizeof(int));
	} else if (data_type == DATA_TYPE_UNSIGNED_INT) {
		memcpy(&(sensorData->uiValue), bytes, sizeof(unsigned int));
	} else if (data_type == DATA_TYPE_LONG) {
		memcpy(&(sensorData->lValue), bytes, sizeof(long));
	} else if (data_type == DATA_TYPE_FLOAT) {
		memcpy(&(sensorData->fValue), bytes, sizeof(float));
	} else if (data_type == DATA_TYPE_DOUBLE) {
		memcpy(&(sensorData->dValue), bytes, sizeof(double));
	} else if (data_type == DATA_TYPE_INT64) {
		memcpy(&(sensorData->i64Value), bytes, sizeof(__int64));
	} else if (data_type == DATA_TYPE_STRING) {
		sensorData->strValue = new char[size + 1];
		memcpy(&(sensorData->strValue[0]), bytes, size);
		sensorData->strValue[size] = 0;
	}

	return sensorData;
}

/*
std::map<const TSensor *, std::list<TSensorData *> *> *DATA;

void initializeData(const TSensor *sensor) {
	(*DATA)[sensor] = new std::list<TSensorData *>();
}
*/

//---------------------------------------------------------------------------
/*
static void Parse(
	const wchar_t *pDataFile,
	size_t dataSize,
	__int64 iTimeGMT1,
	__int64 iTimeGMT2,
	const TDevice *device,
	std::set<const TSensor *, TCmpSensorByIndex> &sensors,
	std::map<const TSensor *, std::list<TSensorData *> *> &data) {
*/

void TTaskRequestDeviceDataFileFormat::GetOffsets(FILE *f, __int64 fileSize, size_t dataSize, long ms1, long ms2, int &offset1, int &offset2) {
	offset1 = FindPositionByteOffsetInDataFile(f, fileSize, dataSize, ms1);
	offset2 = FindPositionByteOffsetInDataFile(f, fileSize, dataSize, ms2);
}

void TTaskRequestDeviceDataFileFormat::Parse(
	const wchar_t *pDataFile,
	size_t dataSize,
	__int64 iTimeGMT1,
	__int64 iTimeGMT2,
	std::map<const TSensor *, std::list<TSensorData *> *> &data) {

	FILE *f = _wfopen(pDataFile, L"rb");
	if (f == NULL) {
		sysLogger::ERR_W(L"The data file can't be opened:");
		sysLogger::ERR_W(pDataFile);
		sysLogger::ERR_W(L"errno:");
		char sErrno[10];
		ltoa(errno, &sErrno[0], 10);
		sysLogger::ERR_A(sErrno);

		return;
	}


	//the length name of the data file is LEN_DATA_FILE_NAME (10) symbols
	//the file name it is the count of seconds since 1970
	__int64 fileOffsetMsec = _wtoi64(&pDataFile[wcslen(pDataFile) - LEN_DATA_FILE_NAME]) * sysTime::SEC2MSEC;

	long ms1 = iTimeGMT1 - fileOffsetMsec;
	long ms2 = iTimeGMT2 - fileOffsetMsec;

	__int64 fileSize = sysFile::GetFileSize64(pDataFile);

	//find file position of iTimeGMT1 and iTimeGMT2 in file, using binary search
	//int iOffset1 = FindPositionByteOffsetInDataFile(f, fileSize, dataSize, ms1);
	//int iOffset2 = FindPositionByteOffsetInDataFile(f, fileSize, dataSize, ms2);
	int iOffset1, iOffset2;
	GetOffsets(f, fileSize, dataSize, ms1, ms2, iOffset1, iOffset2);

	if (iOffset1 == iOffset2 || iOffset2 < iOffset1) {
		fclose(f);
		return;
	}

	int count = (iOffset2 - iOffset1) / dataSize;

	sysLogger::LOG_A("...", sysLogger::LOG_LEVEL_PROFILE);

	BYTE *bytes = new BYTE[count * dataSize];
	//read count * dataSize bytes once from file
	fseek(f, iOffset1, SEEK_SET);
	bool readResult = fread(&bytes[0], count * dataSize, 1, f) == 1;
	if (readResult == true) {

		TDevice::TSensorInfo *sensorInfo;

		//trick to link external data for perform for_each procedure
		for (std::set<const TSensor *, TCmpSensorByIndex>::const_iterator i = sensors.begin(), iEnd = sensors.end(); i != iEnd; ++i) {
			if (!data.count(*i)) {
				data[*i] = new std::list<TSensorData *>();
			}
		}

		//sensors ordered by index!
		std::set<const TSensor *, TCmpSensorByIndex>::iterator itSensor;

		//milliseconds offset from the file
		long milliseconds;

		//offset of begining
		long milliseconds0;
		memcpy(&milliseconds0, &bytes[0], sizeof(milliseconds0));

		long resolutionMs = resolution * sysTime::DAY2MSEC;

		size_t offset;

		for (int  i = 0; i < count; ++i) {
			//get milliseconds offset from the file name for the each data block
			memcpy(&milliseconds, &bytes[i * dataSize], sizeof(milliseconds));

			if (milliseconds - milliseconds0 < resolutionMs) {
				continue;
			}
			milliseconds0 = milliseconds;

			//skip the milliseconds data
			offset = sizeof(long);

			itSensor = sensors.begin();
			for (std::vector<TDevice::TSensorInfo *>::const_iterator iSensorInfo = device->sensorsInfo.begin(), iSensorInfoEnd = device->sensorsInfo.end(); iSensorInfo != iSensorInfoEnd; ++iSensorInfo) {
				sensorInfo = *iSensorInfo;

				//read sensor data from byte buffer only for required sensor
				if ((*itSensor)->index == sensorInfo->index) {
					const TSensor *sensor = *itSensor;
					TSensorData *sensorData = CreateSensorDataFromBytes(
						sensor->data_type,
						sensor->size,
						&bytes[i * dataSize + offset],
						fileOffsetMsec,
						(__int64)milliseconds);

					data[sensor]->push_back(sensorData);

					//iterate throw all require sensors
					++itSensor;
					if (itSensor == sensors.end()) {
						break;
					}
				}

				offset += sensorInfo->sensorDataSize;
			}
		}
	}

	delete []bytes;

	fclose(f);

	sysLogger::LOG_A("request sensor data", sysLogger::LOG_LEVEL_PROFILE);
}

//---------------------------------------------------------------------------
void TTaskRequestDeviceDataFileFormat::Run() {
	wchar_t wDbCentralPath[MAX_PATH];
	memset(wDbCentralPath, 0, MAX_PATH);

	wchar_t wDbArchivePath[MAX_PATH];
	memset(wDbArchivePath, 0, MAX_PATH);

	if (device == NULL) {
		return;
	}

	int centralDbOffset = swprintf(wDbCentralPath, L"%s\\%s", DataModuleMP->sPathToCentralDB.c_str(), device->uuid.c_str());
	int archiveDbOffset = swprintf(wDbArchivePath, L"%s\\%s", DataModuleMP->sPathToArchiveDB.c_str(), device->uuid.c_str());

	TDateTime dtGMT = dt1GMT;

	wchar_t *pDataFile;

	size_t dataBlockSize = device->data_block_size;

	__int64 iTimeGMT1 = sysTime::ConvertToUnixTimeMilliseconds(dt1GMT);
	__int64 iTimeGMT2 = sysTime::ConvertToUnixTimeMilliseconds(dt2GMT);

	//for (TDateTime dtGMT = floor(dt1GMT); dtGMT <= dt2GMT; dtGMT.Val += 1.0) {
	for (TDateTime dtGMT = dt1GMT; floor(dtGMT.Val) <= floor(dt2GMT.Val); dtGMT.Val += 1.0) {
		pDataFile = FindDataFile(wDbCentralPath, wDbArchivePath, dtGMT, centralDbOffset, archiveDbOffset);

		if (pDataFile == NULL) {
			//file can't be found in central and in archive db folders
			continue;
		}

		//Parse(pDataFile, dataBlockSize, iTimeGMT1, iTimeGMT2, device, sensors, data);
		Parse(pDataFile, dataBlockSize, iTimeGMT1, iTimeGMT2, data);
	}
}

//---------------------------------------------------------------------------
void TTaskRequestDeviceDataFileFormat::CleanUp() {
	int usedBefore = SENSOR_DATA_POOL.GetUsedSize();

	TTaskRequestTimeRangeData::CleanUp();

	//int usedAfter = SENSOR_DATA_POOL.GetUsedSize();

	/*
	for (std::map<const TSensor *, std::list<TSensorData *> *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		for (std::list<TSensorData *>::iterator j = i->second->begin(), jEnd = i->second->end(); j != jEnd; ++j) {
			SENSOR_DATA_POOL.Release(*j);
		}
	}

	for (std::map<const TSensor *, std::list<TSensorData *> *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		delete i->second;
	}
	data.clear();
	*/
	//device = NULL;
	//sensor = NULL;
	device = NULL;
	sensors.clear();

	dt1GMT = 0;
	dt2GMT = 0;
}

//---------------------------------------------------------------------------
int TTaskRequestDeviceDataFileFormat::GetObserverableType() {
	return TObserverableTypes::DEVICE_FILE_DATA_FORMAT;
}

/*
bool sortingSensorsByIndex(const TSensor *s1, const TSensor *s2) {
	return s1->index > s2->index;
}
*/

//---------------------------------------------------------------------------
void TTaskRequestDeviceDataFileFormat::SetSensors(std::list<const TSensor *> &requireSensors) {
	//insert sensors and order it by index
	for (std::list<const TSensor *>::const_iterator i = requireSensors.begin(), iEnd = requireSensors.end(); i != iEnd; ++i) {
		const TSensor *sensor = *i;
		sensors.insert(sensor);
	}
}
