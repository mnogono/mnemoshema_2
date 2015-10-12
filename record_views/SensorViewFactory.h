//---------------------------------------------------------------------------

#ifndef SensorViewFactoryH
#define SensorViewFactoryH

#include "SensorView.h"
#include "SensorViewEdit.h"
#include "SensorViewEditBit.h"
#include "sensor_bit_view_image.h"
//#include "SensorBitViewImage.h"
//#include "SensorViewEditBriz.h"

//---------------------------------------------------------------------------
class TSensorViewFactory {
	public:
		//build new TSensorView or null, if can;t identify record view
		static TRecordView * Build(
			TWinControl *parent,
			TRecord *record,
			void (*pInitFunction)(TRecordView *recordView) = NULL);
};

#endif
