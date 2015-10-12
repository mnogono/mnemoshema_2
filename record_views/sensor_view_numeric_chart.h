//---------------------------------------------------------------------------
#ifndef sensor_view_numeric_chartH
#define sensor_view_numeric_chartH

#include "sensor_view_chart.h"
#include "chart_time_axis.h"
#include "data_filter_spike_remove.h"
#include "data_filter_averaging.h"
#include "data_filter_median.h"
//---------------------------------------------------------------------------
class TSensorViewNumericChart : public TSensorViewChart {
	protected:
		TSpike lastFoundSpike;

		double *gaussKernal;
	public:
		TSensorViewNumericChart(TWinControl *parent, const TSensor *sensor, TChartTimeAxis *timeAxis);

		inline __fastcall virtual ~TSensorViewNumericChart();

		virtual void DisplayData(TSensorData *data = NULL);

		virtual void DisplayData(std::list<TSensorData *> *data = NULL);

		virtual void UpdateChart();

		virtual void SetDefaultTitle();

		virtual void UpdateValue();
};

#endif
