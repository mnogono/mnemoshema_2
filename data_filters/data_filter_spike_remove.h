//---------------------------------------------------------------------------
#ifndef data_filter_spike_removeH
#define data_filter_spike_removeH

#include "spike.h"
#include "chart_data.h"
//---------------------------------------------------------------------------

/*
remove all found spikes
*/
void SpikeRemove(const TSensor *sensor, std::list<TSensorData *> *data, double sensorPeriodDay);

void SpikeRemove(const TSensor *sensor, TChartData &chartData, double sensorPeriodDay);

/*
remove only zero spikes, when value suddenly drop to 0
*/
void ZeroSpikeRemove(const TSensor *sensor, std::list<TSensorData *> *data);

void ZeroSpikeRemove(const TSensor *sensor, TChartData &chartData);

#endif
