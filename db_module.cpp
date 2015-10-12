//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "db_module.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TDataModuleMP *DataModuleMP;
//---------------------------------------------------------------------------
__fastcall TDataModuleMP::TDataModuleMP(TComponent* Owner)
	: TDataModule(Owner)
{
}

// ---------------------------------------------------------------------------
static String EmptyTo(String testValue, String defaultValue) {
	return (testValue == NULL || testValue.IsEmpty()) ? defaultValue : testValue;
}

// ---------------------------------------------------------------------------
static int EmptyTo(int testValue, int defaultValue) {
	return (testValue) ? testValue : defaultValue;
}

//---------------------------------------------------------------------------
void __fastcall TDataModuleMP::DataModuleCreate(TObject *Sender)
{
	#ifdef __DEBUG__
		ReportMemoryLeaksOnShutdown = True;
	#endif

	sysLogger::Init(ExtractFileName(Application->ExeName).c_str(), sysLogger::LOG_LEVEL_TRACE);

	setlocale(LC_ALL, ".1251");
	setlocale(LC_NUMERIC, "en_US");
	FormatSettings.DecimalSeparator = L'.';

	if (!CheckDBConnection()){
		return;
	}

	InitializeCriticalSection(&csDB);
}
//---------------------------------------------------------------------------
bool __fastcall TDataModuleMP::CheckDBConnection(){
	try {
		ADOConnection->Connected = true;

		InitializeGlobalVariables();

		//set log level as describe in db
		sysLogger::CURRENT_LOG_LEVEL = GetSettingInt("current_log_level");

		return true;
	} catch (Exception &e){
		sysLogger::ERR_A("Can't connect to db");
		sysLogger::ERR_W(e.Message.c_str());

		String msg;
		msg.sprintf(L"Не Удалось подключиться к базе данных.\nСтрока подключения: %s", ADOConnection->ConnectionString);

		MessageDlg(msg, mtError, TMsgDlgButtons() << mbOK, 0);
	}

	return false;
}
// ---------------------------------------------------------------------------
void __fastcall TDataModuleMP::InitializeGlobalVariables() {
	updateDataManagerProtocol = GetSettingInt(L"mnemoshma_update_data_manager_protocol");

	httpDeviceUpdater = GetSettingInt(L"http_device_updater");

	logRequestDeviceDataForFileReader = GetSettingInt(L"log_file_reader");

	int httpConnectTimeout = GetSettingInt("http_connect_timeout_ms");
	if (!httpConnectTimeout){
		httpConnectTimeout = 10000;
	}

	int httpReadTimeout = GetSettingInt("http_read_timeout_ms");
	if (!httpReadTimeout){
		httpReadTimeout = 5000;
	}

	updateAlarmDataManagerProtocol = GetSettingInt(L"mnemoshma_update_alarm_data_manager_protocol");

	sConfigURL = GetSetting("config_url");
	if (!sConfigURL.IsEmpty()) {
		std::vector<std::wstring> splited;
		sysStr::Split(sConfigURL.c_str(), L":", splited);
		host = String(splited[1].c_str() + 2);
		port = StrToInt(splited[2].c_str());
	}

	countOfPeriod = GetSettingInt("count_of_period_before_brake");

	disconnectAfterHTTPGet = GetSettingInt("disconnect_after_http_get");

	sPathToCentralDB = GetSetting("path_to_central_db");

	sPathToArchiveDB = GetSetting("path_to_archive_db");

	isCentralDBExist = sysFile::IsFolderExist(sPathToCentralDB.c_str());

	isArchiveDBExist = sysFile::IsFolderExist(sPathToArchiveDB.c_str());
}
// ---------------------------------------------------------------------------
String __fastcall TDataModuleMP::GetSetting(String name){

	String value = "";

	if (cacheStringSettings.find(name) != cacheStringSettings.end()){
		return cacheStringSettings[name];
	}

	try {
		ADOQuery->Close();
		ADOQuery->SQL->Text = "select value from settings where name = '" + name + "'";

		ADOQuery->Open();

		sysLogger::DEBUG_W(ADOQuery->SQL->Text.c_str());

		if (ADOQuery->RecordCount > 0){
			value = ADOQuery->Fields->Fields[0]->AsString;
		}
		ADOQuery->Close();
	} catch (Exception &e){
		sysLogger::ERR_W(ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	cacheStringSettings[name] = value;

	return value;
}

// ---------------------------------------------------------------------------
int __fastcall TDataModuleMP::GetSettingInt(String name){
	if (cacheIntegerSettings.find(name) != cacheIntegerSettings.end()){
		return cacheIntegerSettings[name];
	} else {
		String value = GetSetting(name);
		int intValue = StrToIntDef(EmptyTo(value, "0"), 0);

		cacheIntegerSettings[name] = intValue;

		return intValue;
	}
}

String __fastcall TDataModuleMP::FindDataFile(const TSensor *sensor, int date, dataFileLocation::TDataFileLocation &location) {
	EnterCriticalSection(&csDB);

	String path = "";

	try {
		ADOQuerySensorFolder->SQL->Text = "SELECT  path, location  FROM sensor_folder WHERE uuid = '" +sensor->uuid+ "' AND date = " + IntToStr(date);
		/*
		TParameter *paramUUID = ADOQuerySensorFolder->Parameters->ParamByName("UUID");
		paramUUID->Value = sensor->uuid;
		paramUUID->DataType = ftString;

		TParameter *paramDate = ADOQuerySensorFolder->Parameters->ParamByName("DATE");
		paramDate->Value = date;
		paramDate->DataType = ftInteger;

		ADOQuerySensorFolder->Prepared = true;
		*/
		ADOQuerySensorFolder->Open();
		if (ADOQuerySensorFolder->RecordCount > 0) {
			path = ADOQuerySensorFolder->Fields->Fields[0]->AsString;
			location = (dataFileLocation::TDataFileLocation)ADOQuerySensorFolder->Fields->Fields[1]->AsInteger;
		} else {
			path == "";
			location = dataFileLocation::UNKNOWN;
        }
	} catch (Exception &e) {
		sysLogger::ERR_W(ADOQuerySensorFolder->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	ADOQuerySensorFolder->Close();

	LeaveCriticalSection(&csDB);

	return path;
}

void __fastcall TDataModuleMP::SaveDataFile(const TSensor *sensor, int date, const String &path, dataFileLocation::TDataFileLocation location) {
	EnterCriticalSection(&csDB);

	ADOQuerySensorFolder->SQL->Text = "DELETE FROM sensor_folder WHERE uuid = '"+sensor->uuid+"' AND date = " + IntToStr(date);
	try {
		ADOQuerySensorFolder->ExecSQL();
	} catch (Exception &e) {
		sysLogger::ERR_W(ADOQuerySensorFolder->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	ADOQuerySensorFolder->Close();

	wchar_t *screenPath;
	sysStr::ScreenSlash(path.c_str(), screenPath);

	ADOQuerySensorFolder->SQL->Text = "INSERT INTO sensor_folder SET "
		"uuid = '"+sensor->uuid+"', "
		"date = " + IntToStr(date) + ", "
		"path = '" + String(screenPath) + "', "
		"location = " + IntToStr(location);
	try {
		ADOQuerySensorFolder->ExecSQL();
	} catch (Exception &e) {
		sysLogger::ERR_W(ADOQuerySensorFolder->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	ADOQuerySensorFolder->Close();

	delete []screenPath;

	LeaveCriticalSection(&csDB);
}

//---------------------------------------------------------------------------
void __fastcall TDataModuleMP::DataModuleDestroy(TObject *Sender) {
	DeleteCriticalSection(&csDB);
}

//---------------------------------------------------------------------------
TImg * TDataModuleMP::LoadImage(int id) {
	TImg *image = NULL;

	ADOQuery->SQL->Text = "SELECT * FROM img WHERE id = "+IntToStr(id);
	try {
		ADOQuery->Open();
		if (ADOQuery->RecordCount) {
			image = new TImg();
			image->id = ADOQuery->Fields->FieldByName("id")->AsInteger;
			image->uuid = ADOQuery->Fields->FieldByName("uuid")->AsString;
			image->name = ADOQuery->Fields->FieldByName("name")->AsString;
			image->crc32 = ADOQuery->Fields->FieldByName("crc32")->AsInteger;
			image->height = ADOQuery->Fields->FieldByName("height")->AsInteger;
			image->width = ADOQuery->Fields->FieldByName("width")->AsInteger;
			image->size = ADOQuery->Fields->FieldByName("size")->AsInteger;

			TByteDynArray bytes = ADOQuery->Fields->FieldByName("img")->AsBytes;

			image->img = new char[bytes.Length];
			memset(image->img, 0, bytes.Length);

			for (int itByte = 0; itByte < bytes.Length; ++itByte) {
				image->img[itByte] = bytes[itByte];
			}

			TStream *stream = new TMemoryStream();
			stream->WriteBuffer(image->img, bytes.Length);
			stream->Position = 0;

			image->bitmap->LoadFromStream(stream);

			delete stream;
		}
	} catch (Exception &e){
		sysLogger::ERR_W(e.ToString().c_str());
	}

	DataModuleMP->ADOQuery->Close();

	return image;
}

//---------------------------------------------------------------------------
TImg * TDataModuleMP::GetImage(int id) {
	if (images.find(id) == images.end()) {
		TImg *image = LoadImage(id);
		images[id] = image;
	}

	return images[id];
}

//---------------------------------------------------------------------------
TImg * TDataModuleMP::GetImage(const String &uuid) {
	int id = 0;

	try {
		ADOQuery->SQL->Text = "SELECT id FROM img WHERE uuid = '" + uuid + "'";
		ADOQuery->Open();
		if (ADOQuery->RecordCount) {
			id = ADOQuery->Fields->FieldByName("id")->AsInteger;
		}
	} catch (Exception &e) {
		sysLogger::ERR_W(e.ToString().c_str());
	}

	DataModuleMP->ADOQuery->Close();

	if (id == 0) {
		return NULL;
	}

	if (images.find(id) == images.end()) {
		TImg *image = LoadImage(id);
		images[id] = image;
	}

	return images[id];
}
