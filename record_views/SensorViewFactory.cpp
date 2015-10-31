//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "SensorViewFactory.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

TRecordView * TSensorViewFactory::Build(
	TWinControl *parent,
	TRecord *record,
	void (*pInitFunction)(TRecordView *recordView)) {

	TRecordView *recordView = NULL;

	if (record->record_type == RECORD_TYPE_SENSOR) {
		TDevice *device = GetDevice(((TSensor *)record)->device_id);
		if (device && device->type == "briz") {
			recordView = new TSensorViewEdit(parent, static_cast<TSensor *>(record));
		} else {
			recordView = new TSensorViewEdit(parent, static_cast<TSensor *>(record));
		}

	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		TSensorBit *sensorBit = static_cast<TSensorBit *>(record);
		if (sensorBit->view == "text" || sensorBit->view == "edit" || sensorBit->view == "numeric") {
			recordView = new TSensorViewEditBit(parent, sensorBit);

		} else if (sensorBit->view == "image") {
			recordView = new TSensorBitViewImage(parent, sensorBit);
		}

	} else {
		sysLogger::ERR_A("Can't builder record view, record type is unsupported:");
		sysLogger::ERR_W(IntToStr(record->record_type).c_str());

		return NULL;
	}

	if (recordView) {
		recordView->CreateControls();

		if (pInitFunction) pInitFunction(recordView);
	} else {
		sysLogger::ERR_A("Record view is null:");
		sysLogger::ERR_W(record->uuid.c_str());
	}

	return recordView;
}
