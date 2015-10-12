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
	private:
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
};

#endif
