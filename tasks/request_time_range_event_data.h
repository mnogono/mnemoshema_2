//---------------------------------------------------------------------------
#ifndef request_time_range_event_dataH
#define request_time_range_event_dataH

#include "request_mnemoshema_data.h"
//---------------------------------------------------------------------------
class TTaskRequestTimeRangeEventData : public TTaskRequestMnemoshemaData {
	public:
		//data-time1 of time range, GMT
		TDateTime dt1GMT;

		//data-time2 of time range, GMT
		TDateTime dt2GMT;

		//output data
		std::map<const TSensor *, std::list<TSensorEventData *> *> data;

	public:
		virtual void CleanUp();
};

#endif
