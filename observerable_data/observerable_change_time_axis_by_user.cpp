//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "observerable_change_time_axis_by_user.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TObserverableChangeTimeAxisByUser::TObserverableChangeTimeAxisByUser(double dt1LocalTime, double dt2LocalTime) :
	TObserverableHistoryDateTime(dt1LocalTime, dt2LocalTime) {
	}

int TObserverableChangeTimeAxisByUser::GetObserverableType() {
    return TObserverableTypes::TIME_AXIS_CHANGE_BY_USER;
}
