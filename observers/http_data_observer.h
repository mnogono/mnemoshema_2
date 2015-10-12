//---------------------------------------------------------------------------
#ifndef http_data_observerH
#define http_data_observerH

#include "request_http_data.h"
#include "http_data_manager.h"
#include "record_view.h"
//#include "observerable_types.h"
//---------------------------------------------------------------------------
class THTTPDataObserver : public sysObserver::IObserver {
	protected:
		//TMnemoshemaDataManager *mnemoshemaDataManager;

		//theare can be two diffenet containers for record view (list and just a ptr for the TRecordView)
		//can be using one of them for notify the observer
		std::map<const TSensor *, std::list<IMnemoshemaView *> *> *mchViews;

		std::map<const TSensor *, IMnemoshemaView *> *mchView;

		std::map<const TSensorBit *, IMnemoshemaView *> *mshSensorBitViews;

	protected:
		void UpdateDataRecordViews(TTaskRequestHTTPData *httpTask);

		void UpdateDataRecordView(TTaskRequestHTTPData *httpTask);

		void UpdateDataRecordSensorBitViews(TTaskRequestHTTPData *httpTask);
	public:

		THTTPDataObserver(
			std::map<const TSensor *, std::list<IMnemoshemaView *> *> &mchViews);

		THTTPDataObserver(
			std::map<const TSensor *, IMnemoshemaView *> &mchView);

		virtual void UpdateData(sysObserverable::IObserverable *data = NULL);

		void SetSensorBitViews(std::map<const TSensorBit *, IMnemoshemaView *> &mshSensorBitViews);
};

#endif
