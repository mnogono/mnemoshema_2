//---------------------------------------------------------------------------

#ifndef db_moduleH
#define db_moduleH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
//---------------------------------------------------------------------------

namespace dataFileLocation {
	enum TDataFileLocation {UNKNOWN = 0, DCS = 1, ARCHIVE = 2};
}

class TDataModuleMP : public TDataModule
{
__published:	// IDE-managed Components
	TADOConnection *ADOConnection;
	TADOQuery *ADOQuery;
	TADOQuery *ADOQuerySensorFolder;
	void __fastcall DataModuleCreate(TObject *Sender);
	void __fastcall DataModuleDestroy(TObject *Sender);
private:	// User declarations
	//cache of string settings
	std::map<String, String> cacheStringSettings;

	//cache of integer settings
	std::map<String, int> cacheIntegerSettings;

	//cache of double settings
	std::map<String, double> cacheDoubleSettings;

	//user->id, cacheStringSettings
	std::map<int, std::map<String, String> > cacheUserStringSettings;

	/*cache of image records
	key - record img.id, value - instance of record
	*/
	std::map<int, TImg *> images;

	CRITICAL_SECTION csDB;

public:		// User declarations

	int updateDataManagerProtocol;

	int httpDeviceUpdater;

	int logRequestDeviceDataForFileReader;

	int httpConnectTimeout;

	int httpReadTimeout;

	String sConfigURL;

	int countOfPeriod;

	int disconnectAfterHTTPGet;

	String sPathToCentralDB;

	String sPathToArchiveDB;

	bool isCentralDBExist;

	bool isArchiveDBExist;

	int updateAlarmDataManagerProtocol;

	String host;

	int port;

public:

	__fastcall TDataModuleMP(TComponent* Owner);

	bool __fastcall CheckDBConnection();

	void __fastcall InitializeGlobalVariables();

	/*
	if connection with database exist, return setting from database
	else return setting value from file '.application_setting.json'

	return empty string '' if setting name is not defined
	*/
	String __fastcall GetSetting(String name);

	/**
	return 0 if setting name is not defined
	if DataModuleMP has connection to db function return db settings
	if DataModuleMP has no noeection, try to find {name} settings in project setting json file
	*/
	int __fastcall GetSettingInt(String name);

	/**
	return 0 if setting name is not defined
	if DataModuleMP has connection to db function return db settings
	if DataModuleMP has no noeection, try to find {name} settings in project setting json file
	*/
	double __fastcall GetSettingFloat(String name);

	void __fastcall SaveUserSetting(int userId, String name, String value);

	String __fastcall GetUserSetting(int userId, String name);

	bool __fastcall IsSettingExist(const String &name) const;

	void __fastcall SetSetting(const String &name, const String &value, const String &desc);

	void __fastcall SaveSetting(const String &name, const String &value, const String &desc);

	//void __fastcall SetSettingFloat(const String &name, double value, const String &desc);

	void __fastcall CreateDefaultAdminUser();

	//find in db the location of data file on the central server (in central file db or archive file db)
	String __fastcall FindDataFile(const TSensor *, int date, dataFileLocation::TDataFileLocation &location);

	//save the last location of data file in data base
	void __fastcall SaveDataFile(const TSensor *sensor, int date, const String &path, dataFileLocation::TDataFileLocation location);

	//get image record by id or NULL
	TImg * GetImage(int id);

	//get image record by uuid or NULL
	TImg * GetImage(const String &uuid);
private:
	//load image record from db or NULL
	TImg * LoadImage(int id);

};
//---------------------------------------------------------------------------
extern PACKAGE TDataModuleMP *DataModuleMP;
//---------------------------------------------------------------------------
#endif
