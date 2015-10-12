//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "sensor_event_data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TSensorEventData::TSensorEventData() {
	timeGMT = 0;
	eventType0 = event::TSensorEventType::UNKNOWN;
	eventType1 = event::TSensorEventType::UNKNOWN;
	compareType = event::LT;
	compareResult = event::NO;
	dataType = event::INT64;

	bufferSize = 0;
	buffer = NULL;
}

void TSensorEventData::CleanUp() {
	timeGMT = 0;
	eventType0 = event::TSensorEventType::UNKNOWN;
	eventType1 = event::TSensorEventType::UNKNOWN;
	compareType = event::LT;
	compareResult = event::NO;
	dataType = event::INT64;

	if (buffer != NULL) {
		delete []buffer;
		buffer = NULL;
	}
}
