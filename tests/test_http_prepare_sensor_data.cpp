#include "pch.h"
#pragma hdrstop

#include <TestFramework.hpp>
#include "test_http_prepare_sensor_data.h"

class TTestSensorData : public TTestCase {
public:
	String strHexValue;
public:
	__fastcall virtual TTestSensorData(System::String name) : TTestCase(name) {
	}
	virtual void __fastcall SetUp();
	virtual void __fastcall TearDown();

__published:
	void __fastcall TestHTTPPrepareCharSensorData();
	void __fastcall TestHTTPPrepareUnsignedCharSensorData();
	void __fastcall TestHTTPPrepareShortSensorData();
	void __fastcall TestHTTPPrepareUnsignedShortSensorData();
	void __fastcall TestHTTPPrepareIntSensorData();
	void __fastcall TestHTTPPrepareFloatSensorData();
	void __fastcall TestHTTPPrepareInt64SensorData();
	void __fastcall TestHTTPPrepareUnsignedInt64SensorData();
	void __fastcall TestHTTPPrepareDoubleSensorData();
	void __fastcall TestHTTPPrepareStringSensorData();
};

void __fastcall TTestSensorData::SetUp() {
}

void __fastcall TTestSensorData::TearDown() {
}

void __fastcall TTestSensorData::TestHTTPPrepareCharSensorData() {
	TSensor sensor;
	sensor.data_type = DATA_TYPE_CHAR;
	TSensorData sensorData;

	String hex = "ca5f4000ff";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);

	CheckEquals((char)0xFF, sensorData.cValue, "char value 0xFF is wrong");

	hex = "ca5f400010";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);
	CheckEquals((char)0x10, sensorData.cValue, "char value 0x10 is wrong");
}

void __fastcall TTestSensorData::TestHTTPPrepareUnsignedCharSensorData() {
	TSensor sensor;
	sensor.data_type = DATA_TYPE_UNSIGNED_CHAR;
	TSensorData sensorData;

	String hex = "ca5f4000ff";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);

	CheckEquals((unsigned char)0xFF, sensorData.ucValue, "unsigned char value 0xFF is wrong");

	hex = "ca5f400010";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);
	CheckEquals((unsigned char)0x10, sensorData.ucValue, "unsigned char value 0x10 is wrong");
}

void __fastcall TTestSensorData::TestHTTPPrepareShortSensorData() {
	TSensor sensor;
	sensor.data_type = DATA_TYPE_SHORT;
	TSensorData sensorData;

	String hex = "ca5f4000ac0c";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);

	CheckEquals((short)0x0CAC, sensorData.sValue, "shot value 0x0CAC is wrong");

	hex = "ca5f40001000";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);
	CheckEquals((short)0x0010, sensorData.sValue, "unsigned char value 0x0010 is wrong");
}

void __fastcall TTestSensorData::TestHTTPPrepareUnsignedShortSensorData() {
	TSensor sensor;
	sensor.data_type = DATA_TYPE_UNSIGNED_SHORT;
	TSensorData sensorData;

	String hex = "ca5f4000ac0c";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);

	CheckEquals((unsigned short)0x0CAC, sensorData.usValue, "unsigned shot value 0x0CAC is wrong");

	hex = "ca5f40001000";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);
	CheckEquals((unsigned short)0x0010, sensorData.usValue, "unsigned char value 0x0010 is wrong");
}

void __fastcall TTestSensorData::TestHTTPPrepareIntSensorData() {
	TSensor sensor;
	sensor.data_type = DATA_TYPE_INT;
	TSensorData sensorData;

	String hex = "ca5f4000ac0cff09";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);

	CheckEquals((int)0x09FF0CAC, sensorData.iValue, "int value 0x09FF0CAC is wrong");

	hex = "ca5f400010000000";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);
	CheckEquals((int)0x00000010, sensorData.iValue, "int value 0x00000010 is wrong");
}

void __fastcall TTestSensorData::TestHTTPPrepareFloatSensorData() {
	TSensor sensor;
	sensor.data_type = DATA_TYPE_FLOAT;
	TSensorData sensorData;

	float fExpectedValue = 9059.12045;
	BYTE bytes[4];
	memcpy(&bytes[0], &fExpectedValue, 4);

	wchar_t wHex[17];
	memset(&wHex[0], 0, sizeof(wchar_t) * 17);
	swprintf(&wHex[0], L"ca5f4000%.2x%.2x%.2x%.2x", bytes[0], bytes[1], bytes[2], bytes[3]);
	PrepareSensorData(&sensor, wHex + 8, &sensorData);
	CheckEquals(fExpectedValue, sensorData.fValue, "float value "+FloatToStr(fExpectedValue)+" is wrong");

	fExpectedValue = 0;
	memcpy(&bytes[0], &fExpectedValue, 4);

	memset(&wHex[0], 0, sizeof(wchar_t) * 17);
	swprintf(&wHex[0], L"ca5f4000%.2x%.2x%.2x%.2x", bytes[0], bytes[1], bytes[2], bytes[3]);
	PrepareSensorData(&sensor, wHex + 8, &sensorData);
	CheckEquals(fExpectedValue, sensorData.fValue, "float value "+FloatToStr(fExpectedValue)+" is wrong");
}

void __fastcall TTestSensorData::TestHTTPPrepareInt64SensorData() {
	TSensor sensor;
	sensor.data_type = DATA_TYPE_INT64;
	TSensorData sensorData;

	String hex = "ca5f4000ac0cff09ffccaaff";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);

	CheckEquals((__int64)0xFFAACCFF09FF0CAC, sensorData.i64Value, "__int64 value 0xFFAACCFF09FF0CAC is wrong");

	hex = "ca5f40000000000000000000";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);
	CheckEquals((__int64)0x0, sensorData.i64Value, "__int64 value 0x0 is wrong");
}

void __fastcall TTestSensorData::TestHTTPPrepareUnsignedInt64SensorData() {
	TSensor sensor;
	sensor.data_type = DATA_TYPE_UNSIGNED_INT64;
	TSensorData sensorData;

	String hex = "ca5f4000ac0cff09ffccaaff";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);

	CheckEquals((unsigned __int64)0xFFAACCFF09FF0CAC, sensorData.ui64Value, "unsigned __int64 value 0xFFAACCFF09FF0CAC is wrong");

	hex = "ca5f40000000000000000000";
	PrepareSensorData(&sensor, hex.c_str() + 8, &sensorData);
	CheckEquals((unsigned __int64)0x0, sensorData.ui64Value, "unsigned __int64 value 0x0 is wrong");
}

void __fastcall TTestSensorData::TestHTTPPrepareDoubleSensorData() {
	TSensor sensor;
	sensor.data_type = DATA_TYPE_DOUBLE;
	TSensorData sensorData;

	double dExpectedValue = 9059.12045;
	BYTE bytes[8];
	memcpy(&bytes[0], &dExpectedValue, 8);

	wchar_t wHex[25];
	memset(&wHex[0], 0, sizeof(wchar_t) * 25);
	swprintf(&wHex[0], L"ca5f4000%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x", bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7]);
	PrepareSensorData(&sensor, wHex + 8, &sensorData);
	CheckEquals(dExpectedValue, sensorData.dValue, "double value "+FloatToStr(dExpectedValue)+" is wrong");

	dExpectedValue = 0;
	memcpy(&bytes[0], &dExpectedValue, 8);

	memset(&wHex[0], 0, sizeof(wchar_t) * 25);
	swprintf(&wHex[0], L"ca5f4000%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x", bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7]);
	PrepareSensorData(&sensor, wHex + 8, &sensorData);
	CheckEquals(dExpectedValue, sensorData.dValue, "double value "+FloatToStr(dExpectedValue)+" is wrong");
}

void __fastcall TTestSensorData::TestHTTPPrepareStringSensorData() {
	TSensor sensor;
	sensor.data_type = DATA_TYPE_STRING;
	const int size = 11;
	sensor.size = size;
	TSensorData sensorData;
	BYTE bytes[size];

	char strExpectedValue[] = "Hello world";
	memcpy(&bytes[0], &strExpectedValue[0], size);

	const size_t hexSize = 8 + (size - 1) * 2 + 1;
	wchar_t wHex[hexSize];
	memset(&wHex[0], 0, sizeof(wchar_t) * hexSize);
	swprintf(&wHex[0], L"ca5f4000");
	for (int i = 0; i < size; ++i) {
		swprintf(&wHex[8+2*i], L"%.2x", bytes[i]);
	}
	PrepareSensorData(&sensor, wHex + 8, &sensorData);
	CheckEquals(String(strExpectedValue), String(sensorData.strValue), "eng string value "+String(strExpectedValue)+" is wrong");

	char strExpectedValue2[] = "Привет мир!";
	memcpy(&bytes[0], &strExpectedValue2[0], size);

	memset(&wHex[0], 0, sizeof(wchar_t) * hexSize);
	swprintf(&wHex[0], L"ca5f4000");
	for (int i = 0; i < size; ++i) {
		swprintf(&wHex[8+2*i], L"%.2x", bytes[i]);
	}
	PrepareSensorData(&sensor, wHex + 8, &sensorData);
	CheckEquals(String(strExpectedValue2), String(sensorData.strValue), "rus string value "+String(strExpectedValue2)+" is wrong");
}

static void registerTests() {
	Testframework::RegisterTest(TTestSensorData::Suite());
}
#pragma startup registerTests 33
