// ---------------------------------------------------------------------------

#ifndef SensorViewEditBrizH
#define SensorViewEditBrizH

#include "SensorViewEdit.h"
#include "lib.h"

// ---------------------------------------------------------------------------

class TSensorViewEditBriz : public TSensorViewEdit {
public:

	TSensorViewEditBriz(TWinControl *parent, TRecord *sensor);

	virtual void CreateControls();
};

#endif
