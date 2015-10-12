//---------------------------------------------------------------------------


#pragma hdrstop

#include "SensorViewEditBriz.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TSensorViewEditBriz::TSensorViewEditBriz(TWinControl *parent, TRecord *sensor) : TSensorViewEdit(parent, sensor){
};

void TSensorViewEditBriz::CreateControls() {
	TSensor *sensor = dynamic_cast<TSensor *>(record);
	if (!sensor) return;

	bool isUpdateLeftLabel = sysVCL::FindControl(parent, LABEL_LEFT_PREFIX + sensor->uuid) == NULL;

	TSensorViewEdit::CreateControls();

	TLabel *leftLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_LEFT_PREFIX + sensor->uuid);
	if (leftLabel && isUpdateLeftLabel) {
		leftLabel->Caption = GetSensorTreeItemName(sensor);
	}
}

