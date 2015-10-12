//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "time_axis_observer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TTimeAxisObserver::TTimeAxisObserver(/*TChartTimeAxis *timeAxis, */ITimeAxisEvents *events) {
	//this->timeAxis = timeAxis;
	this->events = events;
}

void TTimeAxisObserver::UpdateData(sysObserverable::IObserverable *data) {
	if (data == NULL) {
		return;
	}

	double dt1LocalTime, dt2LocalTime;

	//timeAxis->GetTimeAxisBounds(dt1LocalTime, dt2LocalTime);
	((TChartTimeAxis *)(data))->GetTimeAxisBounds(dt1LocalTime, dt2LocalTime);

	events->OnTimeAxisChanged(dt1LocalTime, dt2LocalTime);
}

