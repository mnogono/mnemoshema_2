//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "observerable_history_date_time.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

double TObserverableHistoryDateTime::fLocalTimeBiasInDays = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;

TObserverableHistoryDateTime::TObserverableHistoryDateTime(double dt1LocalTime, double dt2LocalTime) :
	dt1LocalTime(dt1LocalTime),
	dt2LocalTime(dt2LocalTime) {
}

int TObserverableHistoryDateTime::GetObserverableType() {
	return TObserverableTypes::HISTORY_DATE_TIME;
}

double TObserverableHistoryDateTime::GetDateTime1AsGMT() {
	return dt1LocalTime + fLocalTimeBiasInDays;
}

double TObserverableHistoryDateTime::GetDateTime2AsGMT() {
	return dt2LocalTime + fLocalTimeBiasInDays;
}
