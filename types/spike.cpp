//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "spike.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TSpike::TSpike() {
	time = -1;
}

TSpike::TSpike(double time, bool isPositive, std::list<TSensorData *>::iterator itSensorData) : time(time), isPositive(isPositive), itSensorData(itSensorData) {}

TSpike::TSpike(double time, bool isPositive, int index) : time(time), isPositive(isPositive), index(index) {}

bool TSpike::IsSpike(double y1, double y2) {
	double percent = 5 / 100.0;
	double allowDeviation = percent * y1;
	if (allowDeviation == 0) {
		allowDeviation = percent * y2;
	}
	if (allowDeviation == 0) {
		return false;
	}

	return fabs(y1 - y2) > allowDeviation;
}
