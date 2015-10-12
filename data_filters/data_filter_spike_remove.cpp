//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "data_filter_spike_remove.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
void ZeroSpikeRemove(const TSensor *sensor, std::list<TSensorData *> *data) {
	if (data == NULL || data->size() == 0) {
		return;
	}

	double prevNonZeroValue = 0;
	double y;

	std::list<TSensorData *>::iterator iBegin = data->begin(), iEnd = data->end();
	for (std::list<TSensorData *>::iterator iData = iBegin; iData != iEnd; ++iData) {
		y = SensorDataToDouble(sensor, *iData);
		if (y != 0) {
			prevNonZeroValue = y;
		} else {
			if (sensor->data_type == DATA_TYPE_CHAR) {
				(*iData)->cValue = (char)prevNonZeroValue;
			} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
				(*iData)->ucValue = (unsigned char)prevNonZeroValue;
			} else if (sensor->data_type == DATA_TYPE_SHORT) {
				(*iData)->sValue = (short)prevNonZeroValue;
			} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
				(*iData)->usValue = (unsigned short)prevNonZeroValue;
			} else if (sensor->data_type == DATA_TYPE_INT) {
				(*iData)->iValue = (int)prevNonZeroValue;
			} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
				(*iData)->uiValue = (unsigned int)prevNonZeroValue;
			} else if (sensor->data_type == DATA_TYPE_LONG) {
				(*iData)->lValue = (long)prevNonZeroValue;
			} else if (sensor->data_type == DATA_TYPE_FLOAT) {
				(*iData)->fValue = (float)prevNonZeroValue;
			} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
				(*iData)->dValue = prevNonZeroValue;
			} else if (sensor->data_type == DATA_TYPE_INT64) {
				(*iData)->i64Value = (__int64)prevNonZeroValue;
			}
        }

	}
}

//---------------------------------------------------------------------------
std::list<TSpike> GetSpikes(const TSensor *sensor, std::list<TSensorData *> *data) {
	std::list<TSpike> spikes;

	if (data == NULL || data->size() == 0) {
		return spikes;
	}

	std::list<TSensorData *>::iterator iBegin = data->begin(), iEnd = data->end();

	double x, y, y1, y2;

	for (std::list<TSensorData *>::iterator i = iBegin; i != iEnd; ++i) {
		if (i != iBegin) {
			y1 = y;
		}

		y = SensorDataToDouble(sensor, *i);
		x = sysTime::ConvertToDaysLocalTime((*i)->timeGMT * sysTime::MSEC2SEC);

		y2 = y;

		if (i != iBegin && TSpike::IsSpike(y2, y1) == true) {
			spikes.push_back(TSpike(x, y2 > y1, i));
		}
	}

	return spikes;
}

//---------------------------------------------------------------------------
std::list<TSpike> GetSpikes(const TSensor *sensor, TChartData &chartData) {
	std::list<TSpike> spikes;

	if (chartData.dataSize == 0) {
		return spikes;
	}

	double x, y, y1, y2;

	for (size_t i = 0; i < chartData.dataSize; ++i) {
		if (i != 0) {
			y1 = y;
		}

		y = chartData.dataY[i];
		x = chartData.dataX[i];

		y2 = y;

		if (i != 0 && TSpike::IsSpike(y2, y1) == true) {
			spikes.push_back(TSpike(x, y2 > y1, i - 1));
		}
	}

	return spikes;
}

//---------------------------------------------------------------------------
void SpikeRemove(const TSensor *sensor, std::list<TSensorData *> *data, double sensorPeriodDay) {
	std::list<TSpike> spikes;
	std::list<TSensorData *>::iterator iBegin = data->begin(), iEnd = data->end();
	std::list<TSensorData *>::iterator iBeginNext = ++iBegin;

	double x, y, y1, y2;

	for (std::list<TSensorData *>::iterator i = iBegin; i != iEnd; ++i) {
		if (i != iBegin) {
			y1 = y;
		}

		y = SensorDataToDouble(sensor, *i);
		x = sysTime::ConvertToDaysLocalTime((*i)->timeGMT * sysTime::MSEC2SEC);

		y2 = y;

		if (i != iBegin && TSpike::IsSpike(y2, y1) == true) {
			spikes.push_back(TSpike(x, y2 > y1, --i));
			++i;
		}
	}

	if (spikes.size() == 0) {
		return;
	}

	std::list<TSpike>::iterator itNext = spikes.begin();
	++itNext;

	for (std::list<TSpike>::iterator it = spikes.begin(), iEnd = spikes.end(); it != iEnd; ++it, ++itNext) {
		if (itNext == iEnd) {
			break;
		}

		if (((*it).isPositive != (*itNext).isPositive) &&
			((*itNext).time - (*it).time < sensorPeriodDay)) {
			//remove spike (change value of all data between spikes)

			TSensorData *replaced = *((*it).itSensorData);
			for (std::list<TSensorData *>::iterator iData = (*it).itSensorData, iDataEnd = ++(*itNext).itSensorData; iData != iDataEnd; ++iData) {
				if (sensor->data_type == DATA_TYPE_CHAR) {
					(*iData)->cValue = replaced->cValue;
				} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
					(*iData)->ucValue = replaced->ucValue;
				} else if (sensor->data_type == DATA_TYPE_SHORT) {
					(*iData)->sValue = replaced->sValue;
				} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
					(*iData)->usValue = replaced->usValue;
				} else if (sensor->data_type == DATA_TYPE_INT) {
					(*iData)->iValue = replaced->iValue;
				} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
					(*iData)->uiValue = replaced->uiValue;
				} else if (sensor->data_type == DATA_TYPE_LONG) {
					(*iData)->lValue = replaced->lValue;
				} else if (sensor->data_type == DATA_TYPE_FLOAT) {
					(*iData)->fValue = replaced->fValue;
				} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
					(*iData)->dValue = replaced->dValue;
				} else if (sensor->data_type == DATA_TYPE_INT64) {
					(*iData)->i64Value = replaced->i64Value;
				}
			}

			++it;
			++itNext;
		}
	}
}

//---------------------------------------------------------------------------
void SpikeRemove(const TSensor *sensor, TChartData &chartData, double sensorPeriodDay) {
	int attempt = 0;

	std::list<TSpike> spikes = GetSpikes(sensor, chartData);
	if (spikes.size() == 0) {
		return;
	}

	std::list<TSpike>::iterator itNext = spikes.begin();
	++itNext;

	for (std::list<TSpike>::iterator it = spikes.begin(), iEnd = spikes.end(); it != iEnd; ++it, ++itNext) {
		if (itNext == iEnd) {
			break;
		}

		if (((*it).isPositive != (*itNext).isPositive) &&
			((*itNext).time - (*it).time < 10 * sensorPeriodDay)) {
			//remove spike (change value of all data between spikes)

			for (int i = (*it).index; i <= (*itNext).index; ++i) {
				chartData.dataY[i] = chartData.dataY[(*it).index];
			}

			++it;
			++itNext;
		}
	}
}

//---------------------------------------------------------------------------
void ZeroSpikeRemove(const TSensor *sensor, TChartData &chartData) {
	if (chartData.dataSize == 0) {
		return;
	}

	double prevNonZeroValue = 0;
	double y;

	for (size_t i = 0; i < chartData.dataSize; ++i) {
		y = chartData.dataY[i];
		if (y != 0) {
			prevNonZeroValue = y;
		} else {
			chartData.dataY[i] = prevNonZeroValue;
		}
	}
}
