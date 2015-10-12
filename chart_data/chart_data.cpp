//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "chart_data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
void ResizeValarray(std::valarray<double> &x, std::valarray<double> &y, size_t newDataSize) {
	if (x.size() <= newDataSize) {
		size_t size = x.size();
		double *cpy = new double[size];
		memcpy(&cpy[0], &x[0], size * sizeof(double));
		x.resize(newDataSize * 2);
		memcpy(&x[0], &cpy[0], size * sizeof(double));

		memcpy(&cpy[0], &y[0], size * sizeof(double));
		y.resize(newDataSize * 2);
		memcpy(&y[0], &cpy[0], size * sizeof(double));

		delete []cpy;
		/*
		std::valarray<double> cpy = x[std::slice(0, x.size(), 1)];

		x.resize(newDataSize * 2);
		for (size_t i = 0, len = cpy.size(); i < len; ++i) {
			x[i] = cpy[i];
		}

		cpy = y[std::slice(0, y.size(), 1)];

		y.resize(newDataSize * 2);
		for (size_t i = 0, len = cpy.size(); i < len; ++i) {
			y[i] = cpy[i];
		}
		*/
	}
}

//---------------------------------------------------------------------------
TChartData::TChartData() {
	dataSize = 0;
}

//---------------------------------------------------------------------------
TChartData::TChartData(const TChartData &src) {
	dataSize = src.dataSize;

	dataX.resize(src.dataX.size());
	memcpy(
		&dataX[0],
		&(const_cast<std::valarray<double> &>(src.dataX))[0],
		src.dataX.size() * sizeof(double));

	dataY.resize(src.dataY.size());
	memcpy(
		&dataY[0],
		&(const_cast<std::valarray<double> &>(src.dataY))[0],
		src.dataY.size() * sizeof(double));
}

//---------------------------------------------------------------------------
void TChartData::SetInitialSize(size_t initialCapacity) {
	dataX.resize(initialCapacity, 0.0);

	dataY.resize(initialCapacity, 0.0);
}

//---------------------------------------------------------------------------
void TChartData::SetSensorData(const TSensor *sensor, std::list<TSensorData *> *data) {
	dataSize = 0;

	if (data == NULL) {
		return;
	}

	size_t size = data->size();
	if (size == 0) {
		return;
	}

	ResizeValarray(dataX, dataY, size);

	dataSize = size;

	size_t i = 0;
	if (sensor->data_type == DATA_TYPE_CHAR) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			dataX[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			dataY[i] = (*it)->cValue;
		}
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			dataX[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			dataY[i] = (*it)->ucValue;
		}
	} else if (sensor->data_type == DATA_TYPE_SHORT) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			dataX[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			dataY[i] = (*it)->sValue;
		}
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			dataX[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			dataY[i] = (*it)->usValue;
		}
	} else if (sensor->data_type == DATA_TYPE_INT) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			dataX[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			dataY[i] = (*it)->iValue;
		}
	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			dataX[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			dataY[i] = (*it)->uiValue;
		}
	} else if (sensor->data_type == DATA_TYPE_LONG) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			dataX[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			dataY[i] = (*it)->lValue;
		}
	} else if (sensor->data_type == DATA_TYPE_FLOAT) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			dataX[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			dataY[i] = (*it)->fValue;
		}
	} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			dataX[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			dataY[i] = (*it)->dValue;
		}
	} else if (sensor->data_type == DATA_TYPE_INT64) {
		for (std::list<TSensorData *>::iterator it = data->begin(), iEnd = data->end(); it != iEnd; ++it, ++i) {
			dataX[i] = sysTime::ConvertToDaysLocalTime((*it)->timeGMT * sysTime::MSEC2SEC);
			dataY[i] = (*it)->i64Value;
		}
	}
}

//---------------------------------------------------------------------------
void TChartData::AppendSensorData(const TSensor *sensor, TSensorData *data) {
	if (data == NULL) {
		return;
	}

	ResizeValarray(dataX, dataY, dataSize + 1);

	dataX[dataSize] = sysTime::ConvertToDaysLocalTime(data->timeGMT * sysTime::MSEC2SEC);

	dataY[dataSize] = SensorDataToDouble(sensor, data);

	++dataSize;
}

