//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "record_view_chart_factory.h"
//---------------------------------------------------------------------------
IMnemoshemaView * TRecordViewChartFactory::Build(TWinControl *parent, const TRecord *record, TChartTimeAxis *timeAxis){
	IMnemoshemaView *mchView = NULL;

	if (record->record_type == RECORD_TYPE_SENSOR) {
		const TSensor *sensor = static_cast<const TSensor *>(record);
		if (sensor->full_bit_count != 0) {
			mchView = new TSensorBitsViewNumericChart(parent, sensor, timeAxis);
		} else {
			if (sensor->data_type == DATA_TYPE_STRING) {

			} else {
				mchView = new TSensorViewNumericChart(parent, sensor, timeAxis);
			}
		}
	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		const TSensorBit *sensorBit = static_cast<const TSensorBit *>(record);

		if (sensorBit->view == "numeric" || "image") {
			mchView = new TSensorBitViewNumericChart(parent, sensorBit, timeAxis);

		} else if (sensorBit->view == "text") {
			mchView = new TSensorBitViewTextChart(parent, sensorBit, timeAxis);
		}
	}

	return mchView;
}

#pragma package(smart_init)





