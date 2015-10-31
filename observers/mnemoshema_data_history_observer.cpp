//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "mnemoshema_data_history_observer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMnemoshemaDataHistoryObserver::TMnemoshemaDataHistoryObserver(std::map<const TSensor *, std::list<IMnemoshemaView *> *> &mchViews) :
	TTimeRangeDataObserver(mchViews) {
}

//---------------------------------------------------------------------------
TMnemoshemaDataHistoryObserver::TMnemoshemaDataHistoryObserver(std::map<const TSensor *, IMnemoshemaView *> &mshView) :
	TTimeRangeDataObserver(mshView) {
}

//---------------------------------------------------------------------------
void TMnemoshemaDataHistoryObserver::UpdateData(sysObserverable::IObserverable *data) {
	if (data == NULL) {
		return;
	}

	if (data->GetObserverableType() != TObserverableTypes::MNEMOSHEMA_DATA_HISTORY) {
		return;
	}

	TTaskRequestMnemoshemaDataHistory *task = static_cast<TTaskRequestMnemoshemaDataHistory *>(data);

	if (mshViews != NULL) {
		UpdateDataRecordViews(task);

	} else if (mshView != NULL) {
		UpdateDataRecordView(task);
	}

	UpdateDataRecordSensorBitViews(task);
}
