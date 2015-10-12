//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "request_time_range_event_data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void TTaskRequestTimeRangeEventData::CleanUp() {
	dt1GMT = 0;
	dt2GMT = 0;

	for (std::map<const TSensor *, std::list<TSensorEventData *> *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		for (std::list<TSensorEventData *>::iterator j = i->second->begin(), jEnd = i->second->end(); j != jEnd; ++j) {
			SENSOR_EVENT_DATA_POOL.Release(*j);
		}

		delete i->second;
	}

	data.clear();
}
