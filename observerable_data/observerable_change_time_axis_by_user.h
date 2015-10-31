//---------------------------------------------------------------------------
#ifndef observerable_change_time_axis_by_userH
#define observerable_change_time_axis_by_userH

#include "observerable_history_date_time.h"
//---------------------------------------------------------------------------

struct TObserverableChangeTimeAxisByUser : public TObserverableHistoryDateTime {
	TObserverableChangeTimeAxisByUser(double dt1LocalTime, double dt2LocalTime);

	virtual int GetObserverableType();
};

#endif
