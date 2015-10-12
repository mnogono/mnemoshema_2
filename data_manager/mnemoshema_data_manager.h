//---------------------------------------------------------------------------
#ifndef mnemoshema_data_managerH
#define mnemoshema_data_managerH

#include "request_mnemoshema_data.h"
//---------------------------------------------------------------------------
class TMnemoshemaDataManager : public sysObserverable::TObserverable {
	private:
		//current finished task
		TTaskRequestMnemoshemaData *finishedTask;

		//mnemoshema date time range
		TDateTime *dt1GMT;

		TDateTime *dt2GMT;

		//bool isHistory;
	private:
		//set null to all managed data
		void Clear();
	public:
    	TMnemoshemaDataManager();

		//clear and set finished task
		void SetFinishedTask(TTaskRequestMnemoshemaData *finishedTask);

		//clear and set time range
		void SetTimeRange(TDateTime *dt1GMT, TDateTime *dt2GMT);

		TTaskRequestMnemoshemaData * GetFinishedTask();

		void GetTimeRange(TDateTime **dt1GMT, TDateTime **dt2GMT);
};

#endif
