//---------------------------------------------------------------------------
#ifndef time_range_data_observerH
#define time_range_data_observerH
//---------------------------------------------------------------------------

#include "mnemoshema_data_manager.h"
#include "request_time_range_data.h"
#include "record_view.h"

//---------------------------------------------------------------------------
class TTimeRangeDataObserver : public sysObserver::IObserver {
	protected:
		std::map<const TSensor *, std::list<IMnemoshemaView *> *> *mshViews;
		std::map<const TSensor *, IMnemoshemaView *> *mshView;
		std::map<const TSensorBit *, IMnemoshemaView *> *mshSensorBitViews;
	protected:
		void UpdateDataRecordViews(TTaskRequestTimeRangeData *fileTask);

		void UpdateDataRecordView(TTaskRequestTimeRangeData *fileTask);

		void UpdateDataRecordSensorBitViews(TTaskRequestTimeRangeData *fileTask);

	public:
		TTimeRangeDataObserver(std::map<const TSensor *, std::list<IMnemoshemaView *> *> &mshViews);

		TTimeRangeDataObserver(std::map<const TSensor *, IMnemoshemaView *> &mshView);

		virtual void UpdateData(sysObserverable::IObserverable *data = NULL);

		void SetSensorBitViews(std::map<const TSensorBit *, IMnemoshemaView *> &mshSensorBitViews);
};

#endif
