//---------------------------------------------------------------------------
#ifndef globalH
#define globalH
//---------------------------------------------------------------------------

//len of the data file name in symbols
const BYTE LEN_DATA_FILE_NAME = 10;

const BYTE LEN_EVENT_DATA_FILE_NAME = 16;

const BYTE LEN_EVENT_SNAPSHOT_FILE_NAME = 19;

/*
global sensor data pool, all sensor data object stored in this container
don't create sensor data manually
only pool create new instance if it need
*/
extern sysPatterns::TPool<TSensorData> SENSOR_DATA_POOL;

/*
global sensor event pool, all sensor event data object stored in this container
don't create sensor event data manually
only pool create new instance if it need
*/
extern sysPatterns::TPool<TSensorEventData> SENSOR_EVENT_DATA_POOL;

//global variable of curreant autorization user
extern TUser USER;

//global list of all available of user collection points
extern std::list<TCollectionPoint *> COLLECTION_POINTS;

//global list of all available devices
extern std::list<TDevice *> DEVICES;

//global list of all available sensors
extern std::list<TSensor *> SENSORS;

//global list of all available sensors bits
extern std::list<TSensorBit *> SENSORS_BITS;

//global list of all available tree groups
extern std::list<TDeviceTreeGroup *> DEVICE_TREE_GROUPS;

//global list of all available mnemoshems
extern std::list<TMnemoshema *> MNEMOSHEMS;

//map of ptr for all records
//key - record uuid, value - instatnce of record
extern std::map<String, TRecord *> RECORDS;

extern bool GAUSSIAN_FILTER_ENABLED;

extern bool MEDIAN_FILTER_ENABLED;

/*
global thread pool for execute any async worker of application
amount of threads depends on CPU cures of the machine
THREAD_POOL created by mp module
*/
class TThreadHTTPDataUISynchronize;
extern sysThread::TThreadPoolBorland<TThreadHTTPDataUISynchronize> *THREAD_POOL;

/*
global pool of sensor file request
each sensor file request executed by one of thread from THREAD_POOL
FILE_REQUEST_POOL created by mp module
*/
/*
old request pool, does not using any more
class TTaskRequestFileData;
extern sysPatterns::TPool<TTaskRequestFileData> *FILE_REQUEST_POOL;
*/



/*
global pool of http request
each http request executed by one of thread from THREAD_POOL
HTTP_REQUEST_POOL created by mp module
*/
class TTaskRequestHTTPData;
extern sysPatterns::TPool<TTaskRequestHTTPData> *HTTP_REQUEST_POOL;

/*
global pool of device binary data files
each request executed by one of thread from THREAD_POOL
DEVICE_DATA_FILE_FORMAT_REQUEST_POOL created by mp module
*/
class TTaskRequestDeviceDataFileFormat;
extern sysPatterns::TPool<TTaskRequestDeviceDataFileFormat> *DEVICE_DATA_FILE_FORMAT_REQUEST_POOL;

/*
global pool of device binary event data files
each request executed by one of thread from THREAD_POOL
DEVICE_EVENT_DATA_FILE_FORMAT_REQUEST_POOL created by mp module
*/
class TTaskRequestDeviceEventDataFileFormat;
extern sysPatterns::TPool<TTaskRequestDeviceEventDataFileFormat> *DEVICE_EVENT_DATA_FILE_FORMAT_REQUEST_POOL;

/*
global pool of device binary event data files
each request executed by one of thread from THREAD_POOL
MNEMOSHEMA_DATA_HISTORY_REQUEST_POOL created by mp module
*/
class TTaskRequestMnemoshemaDataHistory;
extern sysPatterns::TPool<TTaskRequestMnemoshemaDataHistory> *MNEMOSHEMA_DATA_HISTORY_REQUEST_POOL;

/*
global ptr for function of create request
task for sensor binary data (FILE_REQUEST_POOL) or
task for device binary data (DEVICE_DATA_FILE_FORMAT_REQUEST_POOL)
initialized in mp module
*/
typedef void (RequestTimeRangeData)(std::set<const TSensor *> &sensors, double dt1GMT, double dt2GMT, double resolution);
extern RequestTimeRangeData *REQUEST_TIME_RANGE_DATA;

/*
global ptr for function of create request
task for all sensors data for dashboard
*/
typedef void (RequestMnemoshemaData)(std::set<const TSensor *> &sensors, double dtGMT);
extern RequestMnemoshemaData *REQUEST_MNEMOSHEMA_DATA;

//find and return ptr to sensor or NULL
TSensor * GetSensor(const String &uuid);

//find and return ptr to sensor or NULL
TSensor * GetSensor(int sensorId);

//find and return ptr to record or NULL
TRecord * GetRecord(const String &uuid);

//find and return ptr to device record or NULL
TDevice * GetDevice(int deviceId);

//find and return ptr to collection point record or NULL
TCollectionPoint * GetCollectionPoint(int collectionPointId);

std::list<TSensor *> GetSensors(const TDevice *device);

/*
repare sensor data from hex string (prepare only value part)
sensorData must be allocated before invoke this function
*/
//bool PrepareSensorData(const TSensor *sensor, const wchar_t *wStrHex, TSensorData *sensorData);

//TSensorData * CreateSensorDataFromBytes(const TSensor *sensor, BYTE *bytes, __int64 time);

//convert sensor data into string
String SensorDataToString(const TSensor *sensor, const TSensorData *sensorData, bool displayUnit);

String SensorDoubleDataToString(const TSensor *sensor, double data, bool displayUnit);

//convert sensor bit data into string
String SensorBitDataToString(const TSensorBit *sensorBit, const TSensorData *sensorData);

//convert sensor bit data into 0 or 1, return -1 if no sensor data
char SensorBitDataToByte(const TSensorBit *sensorBit, const TSensorData *sensorData);

//convert sensor data into double number
double SensorDataToDouble(const TSensor *sensor, const TSensorData *sensorData);

//convert sensor data into __int64 value ( 0 - for float and double values)
__int64 SensorDataToInt64(const TSensor *sensor, const TSensorData *sensorData);

//return sensor formatted name
//String GetSensorFormattedName(const TSensor *sensor);

/*
extract first file name from folder
@param folder
@param fileNameLength criteria of folder scan procedure
@param wFile output parameter, memmory must be allocated before execute function
@return true if file was found
*/
bool GetFileByFileNameLength(const wchar_t *folder, size_t fileNameLength, wchar_t *wDataFile);

//---------------------------------------------------------------------------
//find first data value, where millisecond more or eq ms
int FindPositionByteOffsetInDataFile(FILE *f, size_t fileSize, size_t dataSize, long ms);

/*
put into sensor data the data value
*/
//void SetSensorData(const TSensor *sensor, TSensorData *sensorData, void *data);

/*
create sensor data from bytes array
*/
//TSensorData * CreateSensorDataFromBytes(int data_type, int size, BYTE *bytes, __int64 time, __int64 milliseconds);

TSensorData * CreateSensorDataFromBytes(const TSensor *sensor, BYTE *bytes, __int64 time);

/*
create sensor event data from bytes buffer
*/
TSensorEventData * CreateSensorEventDataFromBytes(BYTE *bytes, __int64 time, size_t bytesSize, size_t dataSize);

void ResizeValarray(std::valarray<double> &x, std::valarray<double> &y, size_t newDataSize);

/*
resize x, and y containers if it needed, return real size of x and y container in out dataSize variable
*/
void ConvertSensorDataToValarray(const TSensor *sensor, std::list<TSensorData *> *data, std::valarray<double> &x, std::valarray<double> &y, size_t &dataSize);

/*
append new data into container x and y
*/
void AppendSensorDataToValarray(const TSensor *sensor, TSensorData *data, std::valarray<double> &x, std::valarray<double> &y, size_t &dataSize);

int BinSearchValueList(TChartValueList *A,  double key, int low, int high, double exp);

String FindFilePath(const String &deviceUuid, const String &date, const String &file);
/*
prepare data
*/
//void ChartDataPrepare(std::valarray<double> &x, std::valarray<double> &y, size_t dataSize);
#endif
