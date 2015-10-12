//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)

TSensor::TSensor() {
	record_type = TRecordType::RECORD_TYPE_SENSOR;
}

String TSensor::GetName() const {
	TBrizAnalogMode mode = unit_mode;
	if (mode == TBrizAnalogMode::amdNone){
		return name;
	}
	if (mode == TBrizAnalogMode::amdDirect){
		return GetUnitNameEn(unit) + " - " + name;
	} else if (mode == TBrizAnalogMode::amdPhaseA || mode == TBrizAnalogMode::amdPhaseB || mode == TBrizAnalogMode::amdPhaseC){
		if (unit == UNIT_HZ){
			return GetUnitNameEn(unit) + "* - " + name;
		}
		return GetUnitNameEn(unit) + GetUnitModeName(mode) + " - " + name;
	} else {
		if (unit == UNIT_VT || unit == UNIT_VAR || unit == UNIT_HZ){
			return GetUnitNameEn(unit) + "* - " + name;
		}
		if (mode == TBrizAnalogMode::amdSequence0 && (unit == UNIT_V || unit == UNIT_A)){
			return "3" + GetUnitNameEn(unit) + GetUnitModeName(mode) + " - " + name;
		}
		return GetUnitNameEn(unit) + GetUnitModeName(mode) + " - " + name;
	}
}

size_t TSensor::GetDataSize() const{
	return TSensor::GetDataSize(data_type, size);
}

size_t TSensor::GetDataSize(short data_type, int size) {
	if (data_type == DATA_TYPE_UNSIGNED_SHORT)      return sizeof(long) + sizeof(unsigned short);
	else if (data_type == DATA_TYPE_SHORT)          return sizeof(long) + sizeof(short);
	else if (data_type == DATA_TYPE_CHAR)           return sizeof(long) + sizeof(char);
	else if (data_type == DATA_TYPE_UNSIGNED_CHAR)  return sizeof(long) + sizeof(unsigned char);
	else if (data_type == DATA_TYPE_INT)            return sizeof(long) + sizeof(int);
	else if (data_type == DATA_TYPE_UNSIGNED_INT)   return sizeof(long) + sizeof(unsigned int);
	else if (data_type == DATA_TYPE_LONG)           return sizeof(long) + sizeof(long);
	else if (data_type == DATA_TYPE_DOUBLE)         return sizeof(long) + sizeof(double);
	else if (data_type == DATA_TYPE_STRING)         return sizeof(long) + size;
	else if (data_type == DATA_TYPE_FLOAT)          return sizeof(long) + sizeof(float);
	else if (data_type == DATA_TYPE_INT64)          return sizeof(long) + sizeof(__int64);
	else if (data_type == DATA_TYPE_UNSIGNED_INT64) return sizeof(long) + sizeof(unsigned __int64);
    return 0;
}

void TSensor::SetSensorBit(const TSensorBit *sensorBit) {
	if (sensorBit->bit < sensorBits.size()) {
		sensorBits[sensorBit->bit] = sensorBit;
	}
}
