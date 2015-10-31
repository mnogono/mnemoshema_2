//---------------------------------------------------------------------------
#ifndef request_mnemoshema_data_historyH
#define request_mnemoshema_data_historyH

#include "request_device_data_file_format.h"
//---------------------------------------------------------------------------
class TTaskRequestMnemoshemaDataHistory : public TTaskRequestDeviceDataFileFormat {
	public:
		TTaskRequestMnemoshemaDataHistory();

		virtual ~TTaskRequestMnemoshemaDataHistory();

		virtual int GetObserverableType();
	protected:
		//override parent function, need to retrive only one data
		virtual void GetOffsets(FILE *f, __int64 fileSize, size_t dataSize, long ms1, long ms2, int &offset1, int &offset2);
};

#endif
