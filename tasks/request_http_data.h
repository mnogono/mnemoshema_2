//---------------------------------------------------------------------------
#ifndef request_http_dataH
#define request_http_dataH

#include "request_mnemoshema_data.h"
//#include "observerable_types.h"
//---------------------------------------------------------------------------
/*
Task for request current device data by http protocol
*/
class TTaskRequestHTTPData : public TTaskRequestMnemoshemaData {
	protected:
		TIdHTTP *http;
	public:
		std::map<const TSensor *, TSensorData *> data;

		const TDevice *device;

	private:
		void Parse(String &httpResponse);
	public:

		TTaskRequestHTTPData();

		~TTaskRequestHTTPData();

		virtual bool operator == (const sysThread::TThreadTask * taskHTTPData);

		virtual bool operator == (sysThread::TThreadTask * taskHTTPData);

		virtual void Run();

		//return sensor data into global SENSOR_DATA_POOL
		virtual void CleanUp();

		virtual int GetObserverableType();
};

#endif
