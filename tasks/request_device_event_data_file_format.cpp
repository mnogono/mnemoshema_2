//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "request_device_event_data_file_format.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TTaskRequestDeviceEventDataFileFormat::TTaskRequestDeviceEventDataFileFormat() {
}

//---------------------------------------------------------------------------
TTaskRequestDeviceEventDataFileFormat::~TTaskRequestDeviceEventDataFileFormat() {
}

//---------------------------------------------------------------------------
bool TTaskRequestDeviceEventDataFileFormat::operator == (const sysThread::TThreadTask * task) {
	return device == (static_cast<const TTaskRequestDeviceEventDataFileFormat *>(task))->device;
}

//---------------------------------------------------------------------------
bool TTaskRequestDeviceEventDataFileFormat::operator == (sysThread::TThreadTask * task) {
	return device == (static_cast<const TTaskRequestDeviceEventDataFileFormat *>(task))->device;
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
	int offset = swprintf(&wDbCentralPath[centralDbOffset], L"\\%4d%02d%02d\\", year, month, day);

	int date = ((int)year)*10000 + ((int)month)*100 + (int)day;
	dataFileLocation::TDataFileLocation location;

	String dataFile = "";

	if (sysFile::IsFileExist(dataFile.c_str()) == false) {
		if (sysFile::IsFolderExist(wDbCentralPath)) {
			if (GetFileByFileNameLength(wDbCentralPath, LEN_EVENT_DATA_FILE_NAME, &wDbCentralPath[centralDbOffset + offset])) {
				//data file was found
				pDataFile = &wDbCentralPath[0];
			}
		}

		if (pDataFile == NULL) {
			//prepare the folder in archive db
			swprintf(&wDbArchivePath[archiveDbOffset], L"\\%4d%02d%02d\\", year, month, day);
			if (sysFile::IsFolderExist(wDbArchivePath)) {
				if (GetFileByFileNameLength(wDbArchivePath, LEN_EVENT_DATA_FILE_NAME, &wDbArchivePath[archiveDbOffset + offset])) {
					pDataFile = &wDbArchivePath[0];
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
/*
parse event data file format:
4 bytes sensor index
4 bytes time file offset
1 byte event type
1 byte event compare
1 byte event compare result
1 byte data type
8 data - for event type (3)

4 bytes sensor index
4 bytes time file offset
1 byte event type
11 byte - data for event type (5)
*/
static void Parse(
	const wchar_t *pDataFile,
	__int64 fileOffsetMsec,
	size_t dataSize,
	const TDevice *device,
	std::set<const TSensor *, TCmpSensorByIndex> &sensors,
	std::map<const TSensor *, std::list<TSensorEventData *> *> &data,
	double dt1GMT,
	double dt2GMT) {

	FILE *f = _wfopen(pDataFile, L"rb");
	if (f == NULL) {
		sysLogger::ERR_W(L"The event data file can't be opened:");
		sysLogger::ERR_W(pDataFile);
		sysLogger::ERR_W(L"errno:");
		char sErrno[10];
		ltoa(errno, &sErrno[0], 10);
		sysLogger::ERR_A(sErrno);

		return;
	}

	__int64 fileSize = sysFile::GetFileSize64(pDataFile);

	int count = fileSize / dataSize;

	BYTE *bytes = new BYTE[count * dataSize];

	//read count * dataSize bytes once from file
	fseek(f, 0, SEEK_SET);

	bool readResult = fread(&bytes[0], count * dataSize, 1, f) == 1;
	long milliseconds;
	__int64 eventDataTimeGMT;

	//be cerful with event type, if new event type need to implement!!!
	if (readResult == true) {
		for (int  i = 0; i < count; ++i) {
			//check date-time range before create sensor event data instance
			memcpy(&milliseconds,  &bytes[i * dataSize + 4],  sizeof(milliseconds));
			eventDataTimeGMT = fileOffsetMsec + (__int64)milliseconds;

			if (sysTime::ConvertToDays(eventDataTimeGMT / 1000) < dt1GMT) {
				continue;
			}
			if (sysTime::ConvertToDays(eventDataTimeGMT / 1000) > dt2GMT) {
				continue;
			}

			//read sensor index from bynary event data
			TSensorEventData *sensorEventData = CreateSensorEventDataFromBytes(
				&bytes[i * dataSize],
				fileOffsetMsec,
				count * dataSize,
				dataSize
			);

			if (sensorEventData == NULL) {
				continue;
			}

			const TSensor *sensor = device->sensorsInfo[sensorEventData->sensorIndex]->sensor;
			if (sensorEventData->eventType1 != event::TSensorEventType::RECORD_START) {
				continue;
			}

			if (sensor != NULL) {
				if (!data.count(sensor)) {
					data[sensor] = new std::list<TSensorEventData *>();
				}
				data[sensor]->push_back(sensorEventData);
			}
		}
	}

	delete []bytes;

	fclose(f);
}

//---------------------------------------------------------------------------
void TTaskRequestDeviceEventDataFileFormat::Run() {
	wchar_t wDbCentralPath[MAX_PATH];
	memset(wDbCentralPath, 0, MAX_PATH);

	wchar_t wDbArchivePath[MAX_PATH];
	memset(wDbArchivePath, 0, MAX_PATH);

	if (device == NULL) {
		return;
	}

	int centralDbOffset = swprintf(wDbCentralPath, L"%s\\%s", DataModuleMP->sPathToCentralDB.c_str(), device->uuid.c_str());
	int archiveDbOffset = swprintf(wDbArchivePath, L"%s\\%s", DataModuleMP->sPathToArchiveDB.c_str(), device->uuid.c_str());

	wchar_t *pDataFile;

	//data block size for event will be [count of sensors] * [size of event data = 16 bytes]
	size_t dataBlockSize = 20; //device event block data format

	__int64 fileOffsetMsec;

	for (TDateTime dtGMT = dt1GMT; floor(dtGMT.Val) <= floor(dt2GMT.Val); dtGMT.Val += 1.0) {
		pDataFile = FindDataFile(wDbCentralPath, wDbArchivePath, dtGMT, centralDbOffset, archiveDbOffset);

		if (pDataFile == NULL) {
			//file can't be found in central and in archive db folders
			continue;
		}

		fileOffsetMsec = _wtoi64(&pDataFile[wcslen(pDataFile) - LEN_EVENT_DATA_FILE_NAME]) * sysTime::SEC2MSEC;

		//parse snapshot file
		swprintf(&pDataFile[wcslen(pDataFile) - 6 /*strlen("_event")*/], L"_snapshot");
		if (sysFile::IsFileExist(pDataFile)) {
			Parse(pDataFile, fileOffsetMsec, dataBlockSize, device, sensors, data, dt1GMT.Val, dt2GMT.Val);
		}

		//parse event file
		swprintf(&pDataFile[wcslen(pDataFile) - 9/*strlen("_snapshot")*/], L"_event");
		Parse(pDataFile, fileOffsetMsec, dataBlockSize, device, sensors, data, dt1GMT.Val, dt2GMT.Val);
	}
}

//---------------------------------------------------------------------------
void TTaskRequestDeviceEventDataFileFormat::CleanUp() {
	TTaskRequestTimeRangeEventData::CleanUp();

	device = NULL;
}

//---------------------------------------------------------------------------
int TTaskRequestDeviceEventDataFileFormat::GetObserverableType() {
	return TObserverableTypes::DEVICE_FILE_EVENT_DATA_FORMAT;
}

//---------------------------------------------------------------------------
void TTaskRequestDeviceEventDataFileFormat::SetSensors(std::list<const TSensor *> &requireSensors) {
	//insert sensors and order it by index
	for (std::list<const TSensor *>::const_iterator i = requireSensors.begin(), iEnd = requireSensors.end(); i != iEnd; ++i) {
		const TSensor *sensor = *i;
		sensors.insert(sensor);
	}
}

