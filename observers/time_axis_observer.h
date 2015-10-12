//---------------------------------------------------------------------------
#ifndef time_axis_observerH
#define time_axis_observerH

#include "chart_time_axis.h"
//---------------------------------------------------------------------------

class ITimeAxisEvents {
	public:
		virtual void OnTimeAxisChanged(double dt1LocalTime, double dt2LocalTime) = 0;
};

class TTimeAxisObserver : public sysObserver::IObserver {
	private:
		ITimeAxisEvents *events;
	public:
		TTimeAxisObserver(ITimeAxisEvents *events);

		//data is instance of TChartTileAxis
		void UpdateData(sysObserverable::IObserverable *data = NULL);
};

#endif
