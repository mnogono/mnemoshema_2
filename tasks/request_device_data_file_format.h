//---------------------------------------------------------------------------
#ifndef request_device_data_file_formatH
#define request_device_data_file_formatH

#include "request_time_range_data.h"
#include "compare_sensor_by_index.h"
//---------------------------------------------------------------------------
class TTaskRequestDeviceDataFileFormat : public TTaskRequestTimeRangeData {
	public:
		//all requested sensors have the same parent device
		const TDevice *device;
	protected:
		//sensors ordered by index! Obligatory contidion!
		std::set<const TSensor *, TCmpSensorByIndex> sensors;

		//output data
		//std::map<const TSensor *, std::list<TSensorData *> *> data;
	public:
		TTaskRequestDeviceDataFileFormat();

		~TTaskRequestDeviceDataFileFormat();

		virtual bool operator == (const sysThread::TThreadTask * task);

		virtual bool operator == (sysThread::TThreadTask * task);

		virtual void Run();

		virtual void CleanUp();

		virtual int GetObserverableType();

		void SetSensors(std::list<const TSensor *> &sensors);
	protected:
		//output offset1 and offset2 - byte offset in FILE f
		virtual void GetOffsets(FILE *f, __int64 fileSize, size_t dataSize, long ms1, long ms2, int &offset1, int &offset2);

	protected:
		void Parse(const wchar_t *pDataFile, size_t dataSize, __int64 iTimeGMT1, __int64 iTimeGMT2, std::map<const TSensor *, std::list<TSensorData *> *> &data);
};

#endif
