//---------------------------------------------------------------------------

#ifndef sensorH
#define sensorH

#include "record.h"
//---------------------------------------------------------------------------

struct TSensorBit;

struct TSensor : public TRecord {
	String name;

	int device_id;

	short data_type;

	int rate_msec;

	//size of string data
	int size;

	//size of event string
	int alarm_size;

	//type of event data __int64 or doule
	short alarm_type;

	// count of sensor_bit records depend on current sensor
	int full_bit_count;

	String desc;

	//id of device tree group (where the sensor can be grouped in)
	int deviceTreeGroupId;

	//lib.GetUnitName(int unit)
	int unit; // unit of measure data signal

	//unit title
	String unitName;

	float max_value;

	float min_value;

	//number of digits after decimal point
	int precision;

	//power type value, see lib.cpp POWER_* values
	int power;

	//power prefix title
	String powerPrefix;

	TBrizAnalogMode unit_mode;

	String unitModeName;

	//factor value, multiply by this value before dysplay
	double powerValue;

	// check is in current moment sensor has alarm status
	int currentEventType;

	int index;

	std::valarray<const TSensorBit *> sensorBits;

	TSensor();

	String GetName() const;

	size_t GetDataSize() const;

	static size_t GetDataSize(short data_type, int size);

	void SetSensorBit(const TSensorBit *);
};

#endif
