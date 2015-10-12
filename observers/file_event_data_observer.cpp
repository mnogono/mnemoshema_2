//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "file_event_data_observer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/*

TFileEventDataObserver::TFileEventDataObserver(std::map<const TRecord *, std::list<IMnemoshemaView *> *> &mshViews) {
	this->mshViews = &mshViews;
	mshView = NULL;
}

TFileEventDataObserver::TFileEventDataObserver(std::map<const TRecord *, IMnemoshemaView *> &mshView) {
	this->mshViews = NULL;
	this->mshView = &mshView;
}

void TFileEventDataObserver::UpdateData(sysObserverable::IObserverable *data) {
}

void TFileEventDataObserver::UpdateDataRecordViews(TTaskRequestFileEventData *task) {
	if (task == NULL) {
		return;
	}

	if (mshViews->size() == 0) {
		return;
	}

	std::map<const TSensor *, std::list<TSensorEventData *> *> &data = task->data;

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

void TFileEventDataObserver::UpdateDataRecordView(TTaskRequestFileEventData *task) {
}

};
*/
