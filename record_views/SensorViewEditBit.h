//---------------------------------------------------------------------------

#ifndef SensorViewEditBitH
#define SensorViewEditBitH
//---------------------------------------------------------------------------

#include "SensorViewEdit.h"
#include "Sensor.h"

class TSensorViewEditBit : public TRecordViewEdit {
	public:
		TSensorViewEditBit(TWinControl *parent, TSensorBit *sensorBit);

		virtual void DisplayData(TSensorData *data = NULL);

		virtual void DisplayData(std::list<TSensorData *> *data = NULL);

		virtual void CreateControls();
};

#endif
