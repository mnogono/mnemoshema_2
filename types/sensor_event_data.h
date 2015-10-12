//---------------------------------------------------------------------------
#ifndef sensor_event_dataH
#define sensor_event_dataH
//---------------------------------------------------------------------------

namespace event {

enum TSensorEventType {
	//unknwon event type
	UNKNOWN = 0,

	//sensor became to offline
	SENSOR_OFFLINE = 1,

	//sensor became to online
	SENSOR_ONLINE = 2,

	//sensor value out of setpoint
	SETPOINT = 3,

	//external information, stored in events.json file
	EXTERNAL = 4,

	//external information record is started
	RECORD_START = 5,

	//external information record is continue
	RECORD_CONTINUE = 6
};

enum TSensorEventCompareType {
	//less
	LT = 0,

	//grater
	GT = 1,

	//less or equal
	LE = 2,

	//grater or equal
	GE = 3,

	//equal
	EQ = 4,

	//not equal
	NE = 5
};

enum TSensorEventCompareResult {
	//compare result is false
	NO = 0,

	//cmpare result is true
	YES = 1
};

enum TSensorEventDataType {
	INT64 = 0,

	DOUBLE = 1,

	STRING = 2
};

}

struct TSensorEventData : public sysPatterns::IPooledObject {
		//date-time in msec since 1970
		__int64 timeGMT;

		//first event type
		event::TSensorEventType eventType0;

		//second event type
		event::TSensorEventType eventType1;

		event::TSensorEventCompareType compareType;

		event::TSensorEventCompareResult compareResult;

		event::TSensorEventDataType dataType;

		long sensorIndex;

		union {
			//int 64 value
			__int64 i64Value;

			//double value
			double dValue;

			//string value
			char strValue[8];
		};

		//buffer for any binary data
		char *buffer;

		//size of bynary buffer
		__int64 bufferSize;

		TSensorEventData();

		virtual void CleanUp();
};

#endif
