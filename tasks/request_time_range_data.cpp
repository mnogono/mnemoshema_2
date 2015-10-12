//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "request_time_range_data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
void TTaskRequestTimeRangeData::CleanUp() {
	dt1GMT = 0;
	dt2GMT = 0;

	for (std::map<const TSensor *, std::list<TSensorData *> *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		for (std::list<TSensorData *>::iterator j = i->second->begin(), jEnd = i->second->end(); j != jEnd; ++j) {
			SENSOR_DATA_POOL.Release(*j);
		}
	}

	//sensors still one for device data file format, but in real it was 3!
	for (std::map<const TSensor *, std::list<TSensorData *> *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		delete i->second;
	}

	data.clear();
}
