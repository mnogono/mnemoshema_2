//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "request_file_data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TTaskRequestFileData::TTaskRequestFileData() /*: dt1GMT(0.0), dt2GMT(0.0) */{
	dt1GMT = 0;
	dt2GMT = 0;
	sensor = NULL;
	resolution = 0;
}

//---------------------------------------------------------------------------
TTaskRequestFileData::~TTaskRequestFileData() {
}

//---------------------------------------------------------------------------
bool TTaskRequestFileData::operator == (const sysThread::TThreadTask * task) {
	const TTaskRequestFileData *_task = static_cast<const TTaskRequestFileData *>(task);
	return sensor == _task->sensor &&
		(abs(dt1GMT - _task->dt1GMT) <= 1 * sysTime::SEC2DAY) &&
		(abs(dt2GMT - _task->dt2GMT) <= 1 * sysTime::SEC2DAY);
}

//---------------------------------------------------------------------------
bool TTaskRequestFileData::operator == (sysThread::TThreadTask * task) {
	TTaskRequestFileData *_task = static_cast<TTaskRequestFileData *>(task);
	return sensor == _task->sensor &&
		(abs(dt1GMT - _task->dt1GMT) <= 1 * sysTime::SEC2DAY) &&
		(abs(dt2GMT - _task->dt2GMT) <= 1 * sysTime::SEC2DAY);
}

//---------------------------------------------------------------------------
/*
extract first file name from folder if file name length eq with fileNameLength
@param folder
@param wFile output parameter, memmory must be allocated before execute function
@return true if file was found
*/
/*
static bool GetDataFile(const wchar_t *folder, wchar_t *wDataFile) {
	WIN32_FIND_DATAW ffd;

	HANDLE hFind = INVALID_HANDLE_VALUE;

	wchar_t wBuffer[MAX_PATH];

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

		wcsncpy(wDataFile, ffd.cFileName, LEN_DATA_FILE_NAME);
		res = true;

		break;
	} while (FindNextFileW(hFind, &ffd));

	FindClose(hFind);

	return res;
}
*/

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
void Parse(const wchar_t *pDataFile, FILE *f, size_t dataSize, __int64 iTimeGMT1, __int64 iTimeGMT2, const TSensor *sensor, std::map<const TSensor *, std::list<TSensorData *> *> &data) {
	//the length name of the data file is LEN_DATA_FILE_NAME (10) symbols
	//the file name it is the count of seconds since 1970
	__int64 fileOffsetMsec = _wtoi64(&pDataFile[wcslen(pDataFile) - LEN_DATA_FILE_NAME]) * sysTime::SEC2MSEC;

	long ms1 = iTimeGMT1 - fileOffsetMsec;
	long ms2 = iTimeGMT2 - fileOffsetMsec;

	__int64 fileSize = sysFile::GetFileSize64(pDataFile);

	//int dataBlockSize = dataSize + sizeof(long);
	//find file position of iTimeGMT1 and iTimeGMT2 in file, using binary search
	int iOffset1 = FindPositionByteOffsetInDataFile(f, fileSize, dataSize, ms1);
	int iOffset2 = FindPositionByteOffsetInDataFile(f, fileSize, dataSize, ms2);

	if (iOffset2 < iOffset1) {
		return;
	}

	int count = (iOffset2 - iOffset1) / dataSize;

	if (count <= 0) {
		return;
	}

	BYTE *bytes = new BYTE[count * dataSize];
	//read count * dataSize bytes once from file
	fseek(f, iOffset1, SEEK_SET);
	bool readResult = fread(&bytes[0], count * dataSize, 1, f) == 1;
	if (readResult == true) {
		//parse bytes data into TSensorData
		for (int i = 0; i < count; ++i) {
			TSensorData *sensorData = CreateSensorDataFromBytes(
				sensor,
				&bytes[i * dataSize],
				fileOffsetMsec);

			data[sensor]->push_back(sensorData);
		}
	} else {
		sysLogger::ERR_A("can't read the data from file:");
		sysLogger::ERR_W(pDataFile);
	}

	delete []bytes;
}

//---------------------------------------------------------------------------
void TTaskRequestFileData::Run() {
	data[sensor] = new std::list<TSensorData *>();

	wchar_t wDbCentralPath[MAX_PATH];
	memset(&wDbCentralPath[0], 0, MAX_PATH);

	wchar_t wDbArchivePath[MAX_PATH];
	memset(&wDbArchivePath[0], 0, MAX_PATH);

	int centralDbOffset = swprintf(wDbCentralPath, L"%s\\%s", DataModuleMP->sPathToCentralDB.c_str(), sensor->uuid.c_str());
	int archiveDbOffset = swprintf(wDbArchivePath, L"%s\\%s", DataModuleMP->sPathToArchiveDB.c_str(), sensor->uuid.c_str());

	TDateTime dtGMT = dt1GMT;

	wchar_t *pDataFile;

	size_t dataSize = sensor->GetDataSize();
	__int64 iTimeGMT1 = sysTime::ConvertToUnixTimeMilliseconds(dt1GMT);
	__int64 iTimeGMT2 = sysTime::ConvertToUnixTimeMilliseconds(dt2GMT);

	//for (TDateTime dtGMT = dt1GMT; dtGMT <= dt2GMT; dtGMT.Val += 1.0) {
	for (TDateTime dtGMT = dt1GMT; floor(dtGMT.Val) <= floor(dt2GMT.Val); dtGMT.Val += 1.0) {
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

		Parse(pDataFile, f, dataSize, iTimeGMT1, iTimeGMT2, sensor, data);

		fclose(f);
	}
}

//---------------------------------------------------------------------------
void TTaskRequestFileData::CleanUp() {
    TTaskRequestTimeRangeData::CleanUp();
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
	sensor = NULL;
	dt1GMT = 0;
	dt2GMT = 0;
}

//---------------------------------------------------------------------------
/*
TTaskType TTaskRequestFileData::GetTaskType() {
	return TTaskType::SMB;
}
*/
//---------------------------------------------------------------------------
int TTaskRequestFileData::GetObserverableType() {
	return TObserverableTypes::SENSOR_FILE_DATA;
}

