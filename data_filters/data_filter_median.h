//---------------------------------------------------------------------------
#ifndef data_filter_medianH
#define data_filter_medianH

#include "chart_data.h"
//---------------------------------------------------------------------------

//execute medial filtering on the chart data using window (data point count)
void MedianFilter(const TSensor *sensor, TChartData &chartData, int window);

//median filter for window == 3
void MedianFilter3(const TSensor *sensor, TChartData &chartData);

#endif
