//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)

TSensorBit::TSensorBit() {
	record_type = TRecordType::RECORD_TYPE_SENSOR_BIT;
}

String TSensorBit::GetName() const {
	return desc;
}
