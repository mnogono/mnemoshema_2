//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "request_file_event_data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TSensorEventData * CreateSensorEventDataFromBytes(const TSensor *sensor, BYTE *bytes, __int64 time) {
	TSensorEventData *sensorEventData = *(SENSOR_EVENT_DATA_POOL.Get<TSensorEventData>());

	long milliseconds;
	memcpy(&milliseconds, &bytes[0], sizeof(milliseconds));
	sensorEventData->timeGMT = time + (__int64)milliseconds;

    unsigned char eventType;

	memcpy(&eventType,                        &bytes[4], sizeof(BYTE));
	memcpy(&(sensorEventData->compareType),   &bytes[5], sizeof(BYTE));
	memcpy(&(sensorEventData->compareResult), &bytes[6], sizeof(BYTE));
	memcpy(&(sensorEventData->dataType),      &bytes[7], sizeof(BYTE));

	sensorEventData->eventType0 = (event::TSensorEventType)eventType >> 4;
	sensorEventData->eventType0 = (event::TSensorEventType)eventType & 0xF;

	if (sensorEventData->dataType == event::TSensorEventDataType::INT64) {
		memcpy(&(sensorEventData->i64Value), &bytes[8], sizeof(__int64));

	} else if (sensorEventData->dataType == event::TSensorEventDataType::DOUBLE) {
		memcpy(&(sensorEventData->dValue), &bytes[4], sizeof(double));
	}

	return sensorEventData;
}

//---------------------------------------------------------------------------
void Parse(const wchar_t *pEventFile, FILE *f, __int64 iTimeGMT1, __int64 iTimeGMT2, const TSensor *sensor, std::map<const TSensor *, std::list<TSensorEventData *> *> &data) {
	//the length name of the data file is LEN_DATA_FILE_NAME (10) symbols
	//the file name it is the count of seconds since 1970
	__int64 fileOffsetMsec = _wtoi64(&pEventFile[wcslen(pEventFile) - LEN_EVENT_DATA_FILE_NAME]) * sysTime::SEC2MSEC;

	long ms1 = iTimeGMT1 - fileOffsetMsec;
	long ms2 = iTimeGMT2 - fileOffsetMsec;

	__int64 fileSize = sysFile::GetFileSize64(pEventFile);

	//correspond with documentation 16 bytes
	static int eventDataSize = 16;

	//int dataBlockSize = dataSize + sizeof(long);
	//find file position of iTimeGMT1 and iTimeGMT2 in file, using binary search
	int iOffset1 = FindPositionByteOffsetInDataFile(f, fileSize, eventDataSize, ms1);
	int iOffset2 = FindPositionByteOffsetInDataFile(f, fileSize, eventDataSize, ms2);

	if (iOffset2 < iOffset1) {
		return;
	}

	int count = (iOffset2 - iOffset1) / eventDataSize;

	if (count <= 0) {
		return;
	}

	BYTE *bytes = new BYTE[count * eventDataSize];
	//read count * dataSize bytes once from file
	fseek(f, iOffset1, SEEK_SET);
	bool readResult = fread(&bytes[0], count * eventDataSize, 1, f) == 1;
	if (readResult == true) {
		//parse bytes data into TSensorData
		for (int i = 0; i < count; ++i) {
			TSensorEventData *sensorEventData = CreateSensorEventDataFromBytes(
				sensor,
				&bytes[i * eventDataSize],
				fileOffsetMsec
			);

			data[sensor]->push_back(sensorEventData);
		}
	} else {
		sysLogger::ERR_A("can't read the data from event file:");
		sysLogger::ERR_W(pEventFile);
	}

	delete []bytes;
}

//---------------------------------------------------------------------------
//for search the data file in central or archive db folders
static wchar_t * FindDataFile(
	wchar_t *wDbCentralPath,
	wchar_t *wDbArchivePath,
	const TDateTime &dtGMT,
	const int &centralDbOffset,
	const int &archiveDbOffset,
	const TSensor *sensor) {

	WORD year, month, day;
	dtGMT.DecodeDate(&year, &month, &day);

	wchar_t *pEventDataFile = NULL;

	//prepare the folder in central db
	swprintf(&wDbCentralPath[centralDbOffset], L"\\%4d%02d%02d\\", year, month, day);

	int date = ((int)year)*10000 + ((int)month)*100 + (int)day;
	dataFileLocation::TDataFileLocation location;

	//String dataFile = DataModuleMP->FindDataFile(sensor, date, location);
	String dataFile = "";

	if (dataFile.IsEmpty() || sysFile::IsFileExist(dataFile.c_str()) == false) {
		if (sysFile::IsFolderExist(wDbCentralPath)) {
			if (GetFileByFileNameLength(wDbCentralPath, LEN_EVENT_DATA_FILE_NAME, &wDbCentralPath[centralDbOffset + 10])) {
				//data file was found
				pEventDataFile = &wDbCentralPath[0];

				//DataModuleMP->SaveDataFile(sensor, date, pDataFile, dataFileLocation::DCS);
			}
		}

		if (pEventDataFile == NULL) {
			//prepare the folder in archive db
			swprintf(&wDbArchivePath[archiveDbOffset], L"\\%4d%02d%02d\\", year, month, day);
			if (sysFile::IsFolderExist(wDbArchivePath)) {
				if (GetFileByFileNameLength(wDbArchivePath, LEN_EVENT_DATA_FILE_NAME, &wDbArchivePath[archiveDbOffset + 10])) {
					pEventDataFile = &wDbArchivePath[0];

					//DataModuleMP->SaveDataFile(sensor, date, pDataFile, dataFileLocation::ARCHIVE);
				}
			}
		}
	} else {
		/*
		if (location == dataFileLocation::DCS) {
			swprintf(wDbCentralPath, L"%s", dataFile.c_str());
			pDataFile = &wDbCentralPath[0];
		} else if (location == dataFileLocation::ARCHIVE) {
			swprintf(wDbArchivePath, L"%s", dataFile.c_str());
			pDataFile = &wDbArchivePath[0];
		}
		*/
	}

	return pEventDataFile;
}

//---------------------------------------------------------------------------
bool TTaskRequestFileEventData::operator == (const sysThread::TThreadTask * task) {
	const TTaskRequestFileEventData *_task = static_cast<const TTaskRequestFileEventData *>(task);
	return sensor == _task->sensor;
}

//---------------------------------------------------------------------------
bool TTaskRequestFileEventData::operator == (sysThread::TThreadTask * task) {
	TTaskRequestFileEventData *_task = static_cast<TTaskRequestFileEventData *>(task);
	return sensor == _task->sensor;
}

//---------------------------------------------------------------------------
void TTaskRequestFileEventData::CleanUp() {
}

//---------------------------------------------------------------------------
void TTaskRequestFileEventData::Run() {
	data[sensor] = new std::list<TSensorEventData *>();

	wchar_t wDbCentralPath[MAX_PATH];
	memset(wDbCentralPath, 0, MAX_PATH);

	wchar_t wDbArchivePath[MAX_PATH];
	memset(wDbArchivePath, 0, MAX_PATH);

	int centralDbOffset = swprintf(wDbCentralPath, L"%s\\%s", DataModuleMP->sPathToCentralDB.c_str(), sensor->uuid.c_str());
	int archiveDbOffset = swprintf(wDbArchivePath, L"%s\\%s", DataModuleMP->sPathToArchiveDB.c_str(), sensor->uuid.c_str());

	TDateTime dtGMT = dt1GMT;

	wchar_t *pEventFile;

	size_t dataSize = sensor->GetDataSize();
	__int64 iTimeGMT1 = sysTime::ConvertToUnixTimeMilliseconds(dt1GMT);
	__int64 iTimeGMT2 = sysTime::ConvertToUnixTimeMilliseconds(dt2GMT);

	for (TDateTime dtGMT = dt1GMT; dtGMT <= dt2GMT; dtGMT.Val += 1.0) {
		pEventFile = FindDataFile(wDbCentralPath, wDbArchivePath, dtGMT, centralDbOffset, archiveDbOffset, sensor);

		if (pEventFile == NULL) {
			//file can't be found in central and in archive db folders
			continue;
		}

		//read data from pDataFile
		FILE *f = _wfopen(pEventFile, L"rb");
		if (f == NULL) {
			if (sysLogger::CheckLogLevel(sysLogger::LOG_LEVEL_ERROR)) {
				sysLogger::ERR_W(L"The event file can't be opened:");
				sysLogger::ERR_W(pEventFile);
				sysLogger::ERR_W(L"errno:");
				char sErrno[10];
				ltoa(errno, &sErrno[0], 10);
				sysLogger::ERR_A(sErrno);
			}
			continue;
		}

		Parse(pEventFile, f, iTimeGMT1, iTimeGMT2, sensor, data);

		fclose(f);
	}
}


//---------------------------------------------------------------------------
int TTaskRequestFileEventData::GetObserverableType() {
	return TObserverableTypes::SENSOR_FILE_EVENT_DATA;
}
