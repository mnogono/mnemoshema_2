//---------------------------------------------------------------------------
#ifndef request_device_file_dataH
#define request_device_file_dataH

#include "request_mnemoshema_data.h"
#include "request_file_data.h"
//---------------------------------------------------------------------------
/*
task for updating history data for dashboard
request only one data per sensor
*/
class TTaskRequestDeviceFileData : public TTaskRequestMnemoshemaData {
	public:
		//requested sensor data
		const TDevice *device;

		//data-time of requesting data
		TDateTime dtGMT;

		//output data
		std::map<const TSensor *, TSensorData *> data;
	public:
		TTaskRequestDeviceFileData();

		~TTaskRequestDeviceFileData();

		virtual bool operator == (const sysThread::TThreadTask * task);

		virtual bool operator == (sysThread::TThreadTask * task);

		virtual void Run();

		virtual void CleanUp();

		//virtual TTaskType GetTaskType();
};
#endif
