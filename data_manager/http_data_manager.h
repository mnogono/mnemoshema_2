//---------------------------------------------------------------------------
#ifndef http_data_managerH
#define http_data_managerH

#include "mnemoshema_data_manager.h"
//---------------------------------------------------------------------------

//http data manager, notify observers for new sensor data
class THTTPDataManager : public TMnemoshemaDataManager {
	public:
		std::map<TSensor *, TSensorData *> *data;
	public:
		THTTPDataManager();
};

#endif
