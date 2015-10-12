//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "request_device_file_data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TTaskRequestDeviceFileData::TTaskRequestDeviceFileData() {
}

TTaskRequestDeviceFileData::~TTaskRequestDeviceFileData() {
}

bool TTaskRequestDeviceFileData::operator == (const sysThread::TThreadTask *task) {
	const TTaskRequestDeviceFileData *_task = static_cast<const TTaskRequestDeviceFileData *>(task);
	return device == _task->device &&
		abs(dtGMT.Val - _task->dtGMT.Val) <= 1 * sysTime::SEC2DAY;
}

bool TTaskRequestDeviceFileData::operator == (sysThread::TThreadTask *task) {
	TTaskRequestDeviceFileData *_task = static_cast<TTaskRequestDeviceFileData *>(task);
	return device == _task->device &&
		abs(dtGMT.Val - _task->dtGMT.Val) <= 1 * sysTime::SEC2DAY;
}

void TTaskRequestDeviceFileData::Run() {
	wchar_t wDbCentralPath[MAX_PATH];
	memset(wDbCentralPath, 0, MAX_PATH);

	wchar_t wDbArchivePath[MAX_PATH];
	memset(wDbArchivePath, 0, MAX_PATH);

	std::list<TSensor *> sensors = GetSensors(device);

	sysLogger::PROFILE_A("..");

	for (std::list<TSensor *>::iterator i = sensors.begin(), iEnd = sensors.end(); i != iEnd; ++i) {
		TSensor *sensor = *i;

		data[sensor] = NULL;

		int centralDbOffset = swprintf(wDbCentralPath, L"%s\\%s", DataModuleMP->sPathToCentralDB.c_str(), sensor->uuid.c_str());
		int archiveDbOffset = swprintf(wDbArchivePath, L"%s\\%s", DataModuleMP->sPathToArchiveDB.c_str(), sensor->uuid.c_str());

		wchar_t *pDataFile;

		size_t dataSize = sensor->GetDataSize();
		__int64 iTimeGMT = sysTime::ConvertToUnixTimeMilliseconds(dtGMT);

		pDataFile = FindDataFile(wDbCentralPath, wDbArchivePath, dtGMT, centralDbOffset, archiveDbOffset, sensor);

		if (pDataFile == NULL) {
			//file can't be found in central and in archive db folders
			continue;
		}

		//read data from pDataFile
		FILE *f = _wfopen(pDataFile, L"rb");
		if (f == NULL) {
			sysLogger::ERR_W(L"The data file can't be opened:");
			sysLogger::ERR_W(pDataFile);
			sysLogger::ERR_W(L"errno:");
			char sErrno[10];
			ltoa(errno, &sErrno[0], 10);
			sysLogger::ERR_A(sErrno);

			continue;
		}

		//the length name of the data file is LEN_DATA_FILE_NAME (10) symbols
		//the file name it is the count of seconds since 1970
		__int64 fileOffsetMsec = _wtoi64(&pDataFile[wcslen(pDataFile) - LEN_DATA_FILE_NAME]) * sysTime::SEC2MSEC;

		long ms = iTimeGMT - fileOffsetMsec;

		__int64 fileSize = sysFile::GetFileSize64(pDataFile);

		//find file position of iTimeGMT in file, using binary search
		int iOffset = FindPositionByteOffsetInDataFile(f, fileSize, dataSize, ms);

		//read milliseconds and data from file
		BYTE *bytes = new BYTE[dataSize];
		fseek(f, iOffset, SEEK_SET);
		bool readResult = fread(&bytes[0], dataSize, 1, f) == 1;
		fclose(f);

		if (readResult == true) {
			//parse bytes data into TSensorData
			data[sensor] = CreateSensorDataFromBytes(
				sensor,
				&bytes[0],
				fileOffsetMsec);
		} else {
			sysLogger::ERR_A("can't read the data from file:");
			sysLogger::ERR_W(pDataFile);
		}
		delete []bytes;
	}

	sysLogger::PROFILE_A("readed device data");
}

void TTaskRequestDeviceFileData::CleanUp() {
	for (std::map<const TSensor *, TSensorData *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		SENSOR_DATA_POOL.Release(i->second);
	}
	data.clear();
}

/*
TTaskType TTaskRequestDeviceFileData::GetTaskType() {
	return TTaskType::DEVICE_SMB;
}
*/
