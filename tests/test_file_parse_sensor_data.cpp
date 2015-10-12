#include "pch.h"
#pragma hdrstop

#include <TestFramework.hpp>
#include "test_file_parse_sensor_data.h"

class TTestTFileParseSensorData : public TTestCase
{
public:
	std::wstring wTestFilesPath;
	std::wstring wTestChartSensorDataFilePath;
public:
  __fastcall virtual TTestTFileParseSensorData(System::String name) : TTestCase(name) {}
  virtual void __fastcall SetUp();
  virtual void __fastcall TearDown();

__published:
  void __fastcall TestFormDestroy();
  void __fastcall TTestTFileParseSensorData::TestParseCharSensorData();
};


void __fastcall TTestTFileParseSensorData::SetUp()
{
	//create files with different sensors types
	//test read the data
	//and clean up the files
	wTestFilesPath = sysFile::GetExecutableFolder() + std::wstring(L"\\test_files");
	if (sysFile::CreateFolder(wTestFilesPath.c_str()) == true) {
		TDateTime dt(2015, 4, 26, 12, 0, 0, 0);
		__int64 unixTime = sysTime::ConvertToUnixTime(dt);
		wchar_t buff[LEN_DATA_FILE_NAME];
		_i64tow(unixTime, buff, 10);
		wTestChartSensorDataFilePath = wTestFilesPath + std::wstring(L"\\") + std::wstring(buff);
		FILE *f = _wfopen(wTestChartSensorDataFilePath.c_str(), L"wb");
		if (f != NULL) {
			for (int i = 0; i < 100; i++) {
				long offset1 = i * 1000;
				fwrite(&offset1, sizeof(long), 1, f);
				char c1 = i;
				fwrite(&c1, sizeof(char), 1, f);
			}
		}
		fclose(f);
	}
}

void __fastcall TTestTFileParseSensorData::TearDown()
{
	sysFile::DeleteFolder(L"test_files");
}

void __fastcall TTestTFileParseSensorData::TestParseCharSensorData() {
	CheckEquals(true, sysFile::IsFileExist(wTestChartSensorDataFilePath.c_str()), "char sensor data file test not exist");

	TDateTime dt1(2015, 4, 26, 12, 0, 0, 0);
	TDateTime dt2(2015, 4, 26, 12, 0, 20, 0);

	TSensor sensor;
	sensor.data_type == DATA_TYPE_CHAR;
	size_t dataSize = sensor.GetDataSize();
	__int64 iTimeGMT1 = sysTime::ConvertToUnixTimeMilliseconds(dt1);
	__int64 iTimeGMT2 = sysTime::ConvertToUnixTimeMilliseconds(dt2);
	std::map<const TSensor *, std::list<TSensorData *> *> data;
	data[&sensor] = new std::list<TSensorData *>;

	FILE *f = _wfopen(wTestChartSensorDataFilePath.c_str(), L"rb");
	Parse(wTestChartSensorDataFilePath.c_str(), f, dataSize, iTimeGMT1, iTimeGMT2, &sensor, data);
	fclose(f);

	CheckEquals(20, (int)data[&sensor]->size(), "size is not expected");

	std::list<TSensorData *> *listSensorData = data[&sensor];

	int val = 0;
	TDateTime dt(2015, 4, 26, 12, 0, 0, 0);
	__int64 msec = sysTime::ConvertToUnixTimeMilliseconds(dt);
	for (std::list<TSensorData *>::iterator i = listSensorData->begin(), iEnd = listSensorData->end(); i != iEnd; ++i) {
		TSensorData *sensorData = *i;
		CheckEquals(val, sensorData->cValue, "value is wrong");
		CheckEquals((__int64)(val * 1000) + msec, sensorData->timeGMT, "time is wrong");
		++val;
	}
}

void __fastcall TTestTFileParseSensorData::TestFormDestroy()
{
  // void FormDestroy(TObject *Sender)
}

static void registerTests()
{
  Testframework::RegisterTest(TTestTFileParseSensorData::Suite());
}
#pragma startup registerTests 33
