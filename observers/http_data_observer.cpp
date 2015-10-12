//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "http_data_observer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
THTTPDataObserver::THTTPDataObserver(
	std::map<const TSensor *, std::list<IMnemoshemaView *> *> &mchViews) {

	this->mchViews = &mchViews;
	mchView = NULL;
	mshSensorBitViews = NULL;
}

//---------------------------------------------------------------------------
THTTPDataObserver::THTTPDataObserver(
	//TMnemoshemaDataManager &mnemoshemaDataManager,
	std::map<const TSensor *, IMnemoshemaView *> &mchView) {

	//this->mnemoshemaDataManager = &mnemoshemaDataManager;
	mchViews = NULL;
	this->mchView = &mchView;
	mshSensorBitViews = NULL;
}

//---------------------------------------------------------------------------
void THTTPDataObserver::UpdateDataRecordViews(TTaskRequestHTTPData *httpTask) {
	if (!mchViews->size()) {
		return;
	}

	std::map<const TSensor *, TSensorData *> &data = httpTask->data;

	const TSensor *sensor;
	TSensorData *sensorData;
	std::list<IMnemoshemaView *> *views;

	for (std::map<const TSensor *, TSensorData *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		sensor = i->first;

		//find the record view
		sensorData = i->second;

		if (mchViews->find(sensor) == mchViews->end()) {
			//sensor record view is not found
			continue;
		}

		views = (*mchViews)[sensor];

		for (std::list<IMnemoshemaView *>::iterator iView = views->begin(), iViewEnd = views->end(); iView != iViewEnd; ++iView) {
			(*iView)->DisplayData(sensorData);
		}
	}
}

//---------------------------------------------------------------------------
void THTTPDataObserver::UpdateDataRecordView(TTaskRequestHTTPData *httpTask) {
	std::map<const TSensor *, TSensorData *> &data = httpTask->data;

	const TSensor *sensor;
	TSensorData *sensorData;

	for (std::map<const TSensor *, TSensorData *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		sensor = i->first;

		//find the record view
		sensorData = i->second;

		if (mchView->find(sensor) == mchView->end()) {
			//sensor record view is not found
			continue;
		}

		(*mchView)[sensor]->DisplayData(sensorData);
	}
}

//---------------------------------------------------------------------------
void THTTPDataObserver::UpdateDataRecordSensorBitViews(TTaskRequestHTTPData *httpTask) {
	if (mshSensorBitViews == NULL) {
		return;
	}

	if (mshSensorBitViews->size() == 0) {
		return;
	}

	std::map<const TSensor *, TSensorData *> &data = httpTask->data;

	const TSensor *sensor;
	TSensorData *sensorData;
	std::list<IMnemoshemaView *> *views;

	for (std::map<const TSensor *, TSensorData *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		sensor = i->first;

		if (sensor->full_bit_count == 0) {
			continue;
		}

		TSensorData *sensorData = i->second;

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

//---------------------------------------------------------------------------
void THTTPDataObserver::UpdateData(sysObserverable::IObserverable *data) {
	if (data == NULL) {
		return;
	}

	if (data->GetObserverableType() != TObserverableTypes::DEVICE_HTTP_DATA) {
		return;
	}

	TTaskRequestHTTPData *httpTask = static_cast<TTaskRequestHTTPData *>(data);

	if (mchViews != NULL) {
		UpdateDataRecordViews(httpTask);
	} else if (mchView != NULL) {
		UpdateDataRecordView(httpTask);
	}

	UpdateDataRecordSensorBitViews(httpTask);
}

//---------------------------------------------------------------------------
void THTTPDataObserver::SetSensorBitViews(std::map<const TSensorBit *, IMnemoshemaView *> &mshSensorBitViews) {
	this->mshSensorBitViews = &mshSensorBitViews;
}
