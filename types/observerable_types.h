//---------------------------------------------------------------------------
#ifndef observerable_typesH
#define observerable_typesH
//---------------------------------------------------------------------------

enum TObserverableTypes {
	HISTORY_DATE_TIME = 1,
	DEVICE_HTTP_DATA,
	SENSOR_FILE_DATA,
	DEVICE_FILE_DATA,
	SENSOR_FILE_EVENT_DATA,

	//new binary data format
	DEVICE_FILE_DATA_FORMAT,

	DEVICE_FILE_EVENT_DATA_FORMAT,

	NEW_VERTICAL_LINE,
	NEW_SUPER_VERTICAL_LINE,
	NEW_TEXT_MARKER
};

#endif
