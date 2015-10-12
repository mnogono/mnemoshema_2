//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "datasource_object_event.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TDatasourceObjectEvent::TDatasourceObjectEvent() {
    dtLocalTime = 0;
	datetime = "";
	sensorName = "";
	event = "";
	jsonExternalData = "";

	device = NULL;
	sensor = NULL;
}

String & TDatasourceObjectEvent::Value(size_t index) {
	if (index == 0) {
		return datetime;

	} else if (index == 1) {
    	return sensorName;

	} else {
    	return event;
	}
}
