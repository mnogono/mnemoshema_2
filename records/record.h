//---------------------------------------------------------------------------

#ifndef recordH
#define recordH
//---------------------------------------------------------------------------

enum TRecordType {
	RECORD_TYPE_COLLECTION_POINT=1,
	RECORD_TYPE_DEVICE=2,
	RECORD_TYPE_MNEMOSHEMA=3,
	RECORD_TYPE_SENSOR=4,
	RECORD_TYPE_USER=5,
	RECORD_TYPE_SENSOR_GROUP=6,
	RECORD_TYPE_RECORD=7,
	RECORD_TYPE_EVENT=8,
	RECORD_TYPE_GROUP=9,
	RECORD_TYPE_PROFILE=10,
	RECORD_TYPE_SETTINS=11,
	RECORD_TYPE_SENSOR_BIT=12,
	RECORD_TYPE_PARAMETER=13,
	RECORD_TYPE_IMAGE=14,
    RECORD_TYPE_DEVICE_TREE_GROUP=15
	};

struct TRecord {
	TRecordType record_type;

	int id;

	String uuid;

	virtual ~TRecord();

	virtual String GetName() const = 0;
};


#endif
