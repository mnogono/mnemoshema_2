//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "data_filter_averaging.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void Averaging(const TSensor *sensor, TChartData &chartData, double timePeriodAveraging) {
	if (chartData.dataSize == 0) {
		return;
	}

	int startRangeIndex = 0;
	double sumValueInTimeRange = chartData.dataY[0];
	for (size_t i = 1; i < chartData.dataSize; ++i) {
		if (chartData.dataX[i] - chartData.dataX[startRangeIndex] < timePeriodAveraging) {
			sumValueInTimeRange += chartData.dataY[i];
		} else {
			size_t count = i - startRangeIndex;
			if (count > 0) {
				double average = sumValueInTimeRange / (double)count;
				for (size_t j = startRangeIndex; j < i; ++j) {
					chartData.dataY[j] = average;
				}
				startRangeIndex = i;
				sumValueInTimeRange = chartData.dataY[i];
			}
		}
	}
}
