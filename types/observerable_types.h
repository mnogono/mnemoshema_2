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
	TIME_AXIS_CHANGE_BY_USER,

	//new binary data format
	DEVICE_FILE_DATA_FORMAT,

	DEVICE_FILE_EVENT_DATA_FORMAT,

	MNEMOSHEMA_DATA_HISTORY,

	NEW_VERTICAL_LINE,
	NEW_SUPER_VERTICAL_LINE,
	NEW_TEXT_MARKER
};

#endif
