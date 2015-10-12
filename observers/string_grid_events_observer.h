//---------------------------------------------------------------------------
#ifndef string_grid_events_observerH
#define string_grid_events_observerH
//---------------------------------------------------------------------------

#include "datasource_object_event.h"
#include "request_device_event_data_file_format.h"
#include "sensor_event_data.h"

#pragma option push -w-8130

class TStringGridEventsObserver : public sysObserver::IObserver {
	public:
		TStringGridDatasource<TDatasourceObjectEvent> *stringGrid;
	public:
		TStringGridEventsObserver();

		virtual void UpdateData(sysObserverable::IObserverable *data = NULL);

		void SetStringGrid(TStringGridDatasource<TDatasourceObjectEvent> *stringGrid);

		void NewJsonExternalEvent(
			TTaskRequestDeviceEventDataFileFormat *request,
			const TDevice *device,
			const TSensor *sensor,
			TSensorEventData *sensorEventData);
};

#pragma option pop

#endif
