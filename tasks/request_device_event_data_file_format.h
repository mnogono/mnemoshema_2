//---------------------------------------------------------------------------

#ifndef request_device_event_data_file_formatH
#define request_device_event_data_file_formatH

#include "request_time_range_event_data.h"
#include "compare_sensor_by_index.h"
//---------------------------------------------------------------------------

class TTaskRequestDeviceEventDataFileFormat : public TTaskRequestTimeRangeEventData {
	public:
		const TDevice *device;

		//sensors ordered by index! Obligatory contidion!
		std::set<const TSensor *, TCmpSensorByIndex> sensors;

	public:
		TTaskRequestDeviceEventDataFileFormat();

		~TTaskRequestDeviceEventDataFileFormat();

		virtual bool operator == (const sysThread::TThreadTask * task);

		virtual bool operator == (sysThread::TThreadTask * task);

		virtual void Run();

		virtual void CleanUp();

		virtual int GetObserverableType();

		void SetSensors(std::list<const TSensor *> &sensors);
};

#endif
