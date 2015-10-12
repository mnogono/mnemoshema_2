//---------------------------------------------------------------------------

#ifndef deviceH
#define deviceH

#include "record.h"
#include "sensor.h"
//---------------------------------------------------------------------------

struct TDevice : public TRecord {
	String name;
	String type;
	int collection_point_id;
	int rate_msec;
	int data_block_size;

	//size in bytes of all sensors data size of the device
	size_t dataBlockSize;

	//all sensors of the device
	//std::vector<TSensor *> sensors;

	struct TSensorInfo {
		//sensor data type
		int dataType;

		//size of string sensor value
		int size;

		//size of sensor data value in bytes
		size_t sensorDataSize;

		//sensor index information
		int index;

		//sensor uuid
		String uuid;

		//sensor link if sensor available as profile access
		//just for optimize
		TSensor *sensor;

		TSensorInfo();
	};

	//offsets of the sensors data into the device data file format
	std::vector<TSensorInfo *> sensorsInfo;

	TDevice();

	virtual String GetName() const;
};

#endif
