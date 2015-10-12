//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)

TDeviceTreeGroup::TDeviceTreeGroup() {
	record_type = TRecordType::RECORD_TYPE_DEVICE_TREE_GROUP;
}

String TDeviceTreeGroup::GetName() const {
	return name;
}
