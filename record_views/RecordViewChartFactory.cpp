//---------------------------------------------------------------------------


#pragma hdrstop

#include "RecordViewChartFactory.h"

//---------------------------------------------------------------------------

TRecordViewChart * TRecordViewChartFactory::Build(TWinControl *owner, TRecord *record, TUser *user, void (*SetChartSeriesSetings)(TLineSeries *, TUser *, String)){
    TRecordViewChart *recordViewChart;

	if (record->record_type == RECORD_TYPE_SENSOR) {
		recordViewChart = new TRecordSensorViewChart(owner, record, user, SetChartSeriesSetings);

	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		//recordViewChart = new TRecordSensorBitViewChart(owner, record, user);
		recordViewChart = new TRecordSensorBitViewChart(owner, record, user, SetChartSeriesSetings);
	}

	return recordViewChart;
}

#pragma package(smart_init)
