//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "compare_sensor_by_index.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

bool TCmpSensorByIndex::operator()(const TSensor *s1, const TSensor *s2) const {
	return s1->index < s2->index;
}
