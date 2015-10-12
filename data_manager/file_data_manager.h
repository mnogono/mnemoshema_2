//---------------------------------------------------------------------------
#ifndef file_data_managerH
#define file_data_managerH

#include "mnemoshema_data_manager.h"
//---------------------------------------------------------------------------
class TFileDataManager : public TMnemoshemaDataManager {
	public:
		std::map<TSensor *, std::list<TSensorData *> *> *data;
	public:
		TFileDataManager();
};

#endif
