//---------------------------------------------------------------------------
#ifndef file_data_observerH
#define file_data_observerH

#include "mnemoshema_data_manager.h"
//#include "request_file_data.h"
#include "request_time_range_data.h"
#include "record_view.h"
//#include "observerable_types.h"

//---------------------------------------------------------------------------
class TFileDataObserver : public sysObserver::IObserver {
	protected:
		//TMnemoshemaDataManager *mnemoshemaDataManager;
		std::map<const TSensor *, std::list<IMnemoshemaView *> *> *mshViews;
		std::map<const TSensor *, IMnemoshemaView *> *mshView;
		std::map<const TSensorBit *, IMnemoshemaView *> *mshSensorBitViews;
	protected:

		void UpdateDataRecordViews(TTaskRequestTimeRangeData *fileTask);

		void UpdateDataRecordView(TTaskRequestTimeRangeData *fileTask);

		void UpdateDataRecordSensorBitViews(TTaskRequestTimeRangeData *fileTask);
	public:
		TFileDataObserver(
			std::map<const TSensor *, std::list<IMnemoshemaView *> *> &mshViews);

		TFileDataObserver(
			std::map<const TSensor *, IMnemoshemaView *> &mshView);

		virtual void UpdateData(sysObserverable::IObserverable *data = NULL);

		void SetSensorBitViews(std::map<const TSensorBit *, IMnemoshemaView *> &mshSensorBitViews);
};
#endif
