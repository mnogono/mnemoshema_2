//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)

TDevice::TDevice() {
	record_type = TRecordType::RECORD_TYPE_DEVICE;
}

String TDevice::GetName() const {
	return name;
}

TDevice::TSensorInfo::TSensorInfo() {
	sensor = NULL;
}

