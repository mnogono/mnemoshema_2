//---------------------------------------------------------------------------

#ifndef RecordViewChartFactoryH
#define RecordViewChartFactoryH

#include "Record.h"
#include "RecordViewChart.h"
#include "RecordSensorViewChart.h"
#include "RecordSensorBitViewChart.h"

//---------------------------------------------------------------------------

class TRecordViewChartFactory {
	private:
		TRecordViewChartFactory();
	public:
		 static TRecordViewChart * Build(TWinControl *owner, TRecord *record, TUser *user, void (*SetChartSeriesSetings)(TLineSeries *, TUser *, String));
};

#endif
