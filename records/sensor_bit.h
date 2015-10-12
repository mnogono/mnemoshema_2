//---------------------------------------------------------------------------

#ifndef sensor_bitH
#define sensor_bitH

#include "record.h"
//---------------------------------------------------------------------------

struct TSensorBit : public TRecord {
	String value0;

	String value1;

	String desc;

	int sensor_id;

	String view;

	/*
	bit = 0..16
	*/
	short bit;

	//non db field
	String name;

	//parent sensor record
	const TSensor *sensor;

	TSensorBit();

	String GetName() const;
};

#endif
