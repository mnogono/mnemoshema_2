//---------------------------------------------------------------------------
#ifndef request_file_event_dataH
#define request_file_event_dataH

#include "request_mnemoshema_data.h"
#include "sensor_event_data.h"
//#include "observerable_types.h"
//---------------------------------------------------------------------------
class TTaskRequestFileEventData : public TTaskRequestMnemoshemaData {
	public:
		const TSensor *sensor;

		//data-time1 of time range, GMT
		TDateTime dt1GMT;

		//data-time2 of time range, GMT
		TDateTime dt2GMT;

		//output data
		std::map<const TSensor *, std::list<TSensorEventData *> *> data;
	public:
		virtual void CleanUp();

		virtual void Run();

		virtual bool operator == (const sysThread::TThreadTask * task);

		virtual bool operator == (sysThread::TThreadTask * task);

		virtual int GetObserverableType();
};

#endif
