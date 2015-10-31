//---------------------------------------------------------------------------
#ifndef device_file_data_observerH
#define device_file_data_observerH

#include "mnemoshema_data_manager.h"
#include "request_device_file_data.h"
#include "mnemoshema_view.h"
#include "record_view.h"
//---------------------------------------------------------------------------
class TDeviceFileDataObserver : public sysObserver::IObserver {
	protected:
		TMnemoshemaDataManager *mnemoshemaDataManager;
		std::map<const TSensor *, std::list<IMnemoshemaView *> *> *mchViews;
		std::map<const TSensorBit *, std::list<IMnemoshemaView *> *> *mchSensorBitViews;
	public:

		TDeviceFileDataObserver(
			TMnemoshemaDataManager &mnemoshemaDataManager,
			std::map<const TSensor *, std::list<IMnemoshemaView *> *> &mchViews,
			std::map<const TSensorBit *, std::list<IMnemoshemaView *> *> &mchSensorBitViews);

		virtual void UpdateData(sysObserverable::IObserverable *data = NULL);

		void UpdateSensorViews(TTaskRequestDeviceFileData *deviceFileTask);

		void UpdateSensorBitViews(TTaskRequestDeviceFileData *deviceFileTask);
};
#endif
