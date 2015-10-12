//---------------------------------------------------------------------------
#ifndef request_file_dataH
#define request_file_dataH

#include "request_time_range_data.h"
#include "request_mnemoshema_data.h"
//#include "observerable_types.h"

//---------------------------------------------------------------------------
//for search the data file in central or archive db folders
wchar_t * FindDataFile(
	wchar_t *wDbCentralPath,
	wchar_t *wDbArchivePath,
	const TDateTime &dtGMT,
	const int &centralDbOffset,
	const int &archiveDbOffset,
	const TSensor *sensor);

/*
int FindPositionByteOffsetInDataFile(
	FILE *f,
	size_t fileSize,
	size_t dataSize,
	long ms);
*/

/*
request offline data, via SMB protocol
*/
//class TTaskRequestFileData : public TTaskRequestMnemoshemaData {
class TTaskRequestFileData : public TTaskRequestTimeRangeData {
	public:
		//requested sensor data
		const TSensor *sensor;

		//output data
		//std::map<const TSensor *, std::list<TSensorData *> *> data;
	private:
	public:
		TTaskRequestFileData();

		~TTaskRequestFileData();

		virtual bool operator == (const sysThread::TThreadTask * task);

		virtual bool operator == (sysThread::TThreadTask * task);

		virtual void Run();

		virtual void CleanUp();

		virtual int GetObserverableType();
};
#endif
