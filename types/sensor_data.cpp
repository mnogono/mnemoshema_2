//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "sensor_data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TSensorData::TSensorData() {
	size = 0;
	strValue = NULL;
}

void TSensorData::CleanUp() {
	if (size != 0) {
		delete []strValue;
		strValue = NULL;
		size = 0;
	}
}
