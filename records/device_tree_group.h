//---------------------------------------------------------------------------

#ifndef device_tree_groupH
#define device_tree_groupH

//#include "record.h"
//---------------------------------------------------------------------------

struct TDeviceTreeGroup : public TRecord {
	String name;
	String parent_uuid;

	TDeviceTreeGroup();

	String GetName() const;
};

#endif
