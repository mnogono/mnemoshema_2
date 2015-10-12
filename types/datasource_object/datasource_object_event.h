//---------------------------------------------------------------------------
#ifndef datasource_object_eventH
#define datasource_object_eventH

//---------------------------------------------------------------------------
struct TDatasourceObjectEvent : public TDatasourceObject {
	const TDevice *device;
	const TSensor *sensor;

	/* local time in days */
	double dtLocalTime;

	String datetime;
	String sensorName;
	String event;

	/* json string of external alarm event data */
	String jsonExternalData;

	TDatasourceObjectEvent();

    virtual String & Value(size_t index);
};

#endif
