//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "device_file_data_observer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TDeviceFileDataObserver::TDeviceFileDataObserver(
	TMnemoshemaDataManager &mnemoshemaDataManager,
	std::map<const TSensor *, std::list<IMnemoshemaView *> *> &mchViews,
	std::map<const TSensorBit *, std::list<IMnemoshemaView *> *> &mchSensorBitViews) {

	this->mnemoshemaDataManager = &mnemoshemaDataManager;
	this->mchViews = &mchViews;
	this->mchSensorBitViews = &mchSensorBitViews;
}

void TDeviceFileDataObserver::UpdateSensorViews(TTaskRequestDeviceFileData *deviceFileTask) {
	if (!mchViews->size()) {
		return;
	}

	std::map<const TSensor *, TSensorData *> &taskData = deviceFileTask->data;

	const TSensor *sensor;
	std::list<IMnemoshemaView *> *views;

	String uuid;

	for (std::map<const TSensor *, TSensorData *>::iterator i = taskData.begin(), iEnd = taskData.end(); i != iEnd; ++i) {
		sensor = i->first;

		//find the record view
		TSensorData *sensorData = i->second;

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

void TDeviceFileDataObserver::UpdateSensorBitViews(TTaskRequestDeviceFileData *deviceFileTask) {
	/*
	if (!mchSensorBitViews->size()) {
		return;
	}

	std::map<const TSensor *, TSensorData *> &taskData = deviceFileTask->data;

	const TSensor *sensor;
	std::list<IMnemoshemaView *> *views;

	String uuid;

	for (std::map<const TSensor *, TSensorData *>::iterator i = taskData.begin(), iEnd = taskData.end(); i != iEnd; ++i) {
		sensor = i->first;

		//find the record view
		TSensorData *sensorData = i->second;

		if (mchViews->find(sensor) == mchViews->end()) {
			//sensor record view is not found
			continue;
		}

		views = (*mchViews)[sensor];

		for (std::list<IMnemoshemaView *>::iterator iView = views->begin(), iViewEnd = views->end(); iView != iViewEnd; ++iView) {
			(*iView)->DisplayData(sensorData);
		}
	}
	*/
}

void TDeviceFileDataObserver::UpdateData(sysObserverable::IObserverable *data) {
	if (data == NULL) {
		return;
	}

	if (data->GetObserverableType() != TObserverableTypes::DEVICE_FILE_DATA) {
		return;
	}

	TTaskRequestMnemoshemaData *task = static_cast<TTaskRequestMnemoshemaData *>(data);
	if (task == NULL) {
		return;
	}

	TTaskRequestDeviceFileData *deviceFileTask = static_cast<TTaskRequestDeviceFileData *>(task);

	UpdateSensorViews(deviceFileTask);

	UpdateSensorBitViews(deviceFileTask);
}
