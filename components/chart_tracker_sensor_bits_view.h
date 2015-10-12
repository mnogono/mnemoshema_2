//---------------------------------------------------------------------------

#ifndef chart_tracker_sensor_bits_viewH
#define chart_tracker_sensor_bits_viewH

//---------------------------------------------------------------------------
#include "chart_tracker.h"

/*
chart tracker for sensor view bits chart
*/
class TChartTrackerSensorBitsView : public TChartTracker {
	public:
		TChartTrackerSensorBitsView(TChart *chart);

		void Move(int x, int y);
};


#endif
