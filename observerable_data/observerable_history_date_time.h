//---------------------------------------------------------------------------
#ifndef observerable_history_date_timeH
#define observerable_history_date_timeH

//#include "observerable_types.h"
//---------------------------------------------------------------------------

struct TObserverableHistoryDateTime : public sysObserverable::TObserverable {
	static double fLocalTimeBiasInDays;

	double dt1LocalTime;

	double dt2LocalTime;

	TObserverableHistoryDateTime(double dt1LocalTime, double dt2LocalTime);

	virtual int GetObserverableType();

	/*
	convert dt1LocalTime into GMT time
	@return date time in days
	*/
	double GetDateTime1AsGMT();

	/*
	convert dt2LocalTime into GMT time
	@return date time in days
	*/
	double GetDateTime2AsGMT();
};

#endif
