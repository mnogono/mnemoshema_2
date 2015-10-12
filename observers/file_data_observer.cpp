//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "file_data_observer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//TODO maybe rename to time_range_data_observer?

TFileDataObserver::TFileDataObserver(
	std::map<const TSensor *, IMnemoshemaView *> &mshView) {

	this->mshViews = NULL;
	this->mshView = &mshView;
	mshSensorBitViews = NULL;
}

TFileDataObserver::TFileDataObserver(
	std::map<const TSensor *, std::list<IMnemoshemaView *> *> &mshViews) {

	this->mshViews = &mshViews;
	mshView = NULL;
	mshSensorBitViews = NULL;
}

void TFileDataObserver::UpdateDataRecordSensorBitViews(TTaskRequestTimeRangeData *fileTask) {
	if (mshSensorBitViews == NULL) {
		return;
	}

	if (mshSensorBitViews->size() == 0) {
		return;
	}

	std::map<const TSensor *, std::list<TSensorData *> *> &data = fileTask->data;

	const TSensor *sensor;
	std::list<IMnemoshemaView *> *views;

	for (std::map<const TSensor *, std::list<TSensorData *> *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		sensor = i->first;

		if (sensor->full_bit_count == 0) {
			continue;
		}

		std::list<TSensorData *> *sensorData = i->second;

		for (int isb = 0, len = sensor->sensorBits.size(); isb < len; ++isb) {
			const TSensorBit *sensorBit = sensor->sensorBits[isb];
			if (sensorBit == NULL) {
				continue;
			}

			if (mshSensorBitViews->find(sensorBit) == mshSensorBitViews->end()) {
            	continue;
			}

			(*mshSensorBitViews)[sensorBit]->DisplayData(sensorData);
		}
	}
}

void TFileDataObserver::UpdateDataRecordViews(TTaskRequestTimeRangeData *fileTask) {
	if (mshViews->size() == 0) {
		return;
	}

	std::map<const TSensor *, std::list<TSensorData *> *> &data = fileTask->data;

	const TSensor *sensor;
	std::list<IMnemoshemaView *> *views;

	for (std::map<const TSensor *, std::list<TSensorData *> *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		sensor = i->first;

		//find the record view
		std::list<TSensorData *> *sensorData = i->second;

		if (mshViews->find(sensor) == mshViews->end()) {
			//sensor record view is not found
			continue;
		}

		views = (*mshViews)[sensor];

		for (std::list<IMnemoshemaView *>::iterator iView = views->begin(), iViewEnd = views->end(); iView != iViewEnd; ++iView) {
			(*iView)->DisplayData(sensorData);
		}
	}
}

void TFileDataObserver::UpdateDataRecordView(TTaskRequestTimeRangeData *fileTask) {
	if (mshView->size() == 0) {
		return;
	}

	std::map<const TSensor *, std::list<TSensorData *> *> &data = fileTask->data;

	const TSensor *sensor;

	for (std::map<const TSensor *, std::list<TSensorData *> *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		sensor = i->first;

		//find the record view
		std::list<TSensorData *> *sensorData = i->second;

		if (mshView->find(sensor) == mshView->end()) {
			//sensor record view is not found
			continue;
		}

		(*mshView)[sensor]->DisplayData(sensorData);
	}
}

void TFileDataObserver::UpdateData(sysObserverable::IObserverable *data) {
	if (data == NULL) {
		return;
	}

	int observerableType = data->GetObserverableType();
	TTaskRequestTimeRangeData *task = NULL;

	if (observerableType == TObserverableTypes::DEVICE_FILE_DATA_FORMAT ||
		observerableType == TObserverableTypes::SENSOR_FILE_DATA) {

		task = static_cast<TTaskRequestTimeRangeData *>(data);

		if (mshViews != NULL) {
			UpdateDataRecordViews(task);
		} else if (mshView != NULL) {
			UpdateDataRecordView(task);
		}

		UpdateDataRecordSensorBitViews(task);
	}
}

void TFileDataObserver::SetSensorBitViews(std::map<const TSensorBit *, IMnemoshemaView *> &mshSensorBitViews) {
	this->mshSensorBitViews = &mshSensorBitViews;
}
