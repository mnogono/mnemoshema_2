//---------------------------------------------------------------------------
#ifndef mnemoshema_data_history_observerH
#define mnemoshema_data_history_observerH

#include "time_range_data_observer.h"
#include "request_mnemoshema_data_history.h"
//---------------------------------------------------------------------------

class TMnemoshemaDataHistoryObserver : public TTimeRangeDataObserver {
	public:
		TMnemoshemaDataHistoryObserver(std::map<const TSensor *, std::list<IMnemoshemaView *> *> &mchViews);

		TMnemoshemaDataHistoryObserver(std::map<const TSensor *, IMnemoshemaView *> &mshView);

		virtual void UpdateData(sysObserverable::IObserverable *data = NULL);
};


#endif
