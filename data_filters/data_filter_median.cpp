//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "data_filter_median.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void MedianFilter3(const TSensor *sensor, TChartData &chartData) {
	if (chartData.dataSize < 3) {
		return;
	}

	double left = chartData.dataY[0];
	double center = chartData.dataY[1];
	double right = chartData.dataY[2];
	double d[3];
	for (int i = 1, len = chartData.dataSize - 1; i < len; ++i) {
		d[0] = left;
		d[1] = center;
		d[2] = right;

		if (d[1] < d[0]) {
			std::swap(d[0], d[1]);
		}
		if (d[2] < d[0]) {
			std::swap(d[0], d[2]);
		}
		if (d[2] < d[1]) {
			std::swap(d[2], d[1]);
		}

		chartData.dataY[i] = d[1];

		left = center;
		center = right;
		right = chartData.dataY[i+1];
	}
}
