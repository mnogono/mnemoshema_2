//---------------------------------------------------------------------------

#ifndef RecordSensorViewChartH
#define RecordSensorViewChartH

#include "RecordViewChart.h"
#include <limits>
#include "DataModuleMP.h"
#include "sysMath.h"

//---------------------------------------------------------------------------

class TRecordSensorViewChart : public TRecordViewChart{
	private:
    	int count_of_period_before_brake;
	public:
		TRecordSensorViewChart(TWinControl *owner, TRecord *record, TUser *user, void (*SetChartSeriesSetings)(TLineSeries *, TUser *, String));

		inline __fastcall virtual ~TRecordSensorViewChart(void);

		void virtual DisplayData(std::vector<TData *> *data, bool bClearSeries = true);

		virtual String __fastcall GetName();
};

#endif
