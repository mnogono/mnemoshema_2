//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "crane_mimic_panel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TCraneMimicPanel::TCraneMimicPanel(TCanvas *canvas, TBitmap *bitmap) {
	/*
	bitmapBackground = new TBitmap();

	bitmapBackground->Assign(image->Picture->Graphic);
	*/

	bitmapBackground = bitmap;

	this->canvas = canvas;

	initializeSensors();

	initializeCranes();
}

void TCraneMimicPanel::initializeCranes() {
	String defaultZero = L"0";
	String defaultOne = L"1";

	String sValue;
	double x1Position, x2Position;
	double x1Pixels, x2Pixels;

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1BegiXPositionMeters"), defaultZero);
	x1Position = StrToFloatDef(sValue, 0.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1BeginXPositionPixels"), defaultZero);
	x1Pixels = StrToFloatDef(sValue, 0.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1EndXPositionMeters"), defaultOne);
	x2Position = StrToFloatDef(sValue, 1.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1EndXPositionPixels"), defaultOne);
	x2Pixels = StrToFloatDef(sValue, 1.0);

	double y1Position, y2Position;
	double y1Pixels, y2Pixels;

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1BeginYPositionMeters"), defaultZero);
	y1Position = StrToFloatDef(sValue, 0.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1BeginYPositionPixels"), defaultZero);
	y1Pixels = StrToFloatDef(sValue, 0.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1EndYPositionMeters"), defaultOne);
	y2Position = StrToFloatDef(sValue, 1.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1EndYPositionPixels"), defaultOne);
	y2Pixels = StrToFloatDef(sValue, 1.0);

	double r1Position, r2Position;
	double r1Degree, r2Degree;

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1RotateBeginPositionMeters"), defaultZero);
	r1Position = StrToFloatDef(sValue, 0.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1RotateBeginPositionDegree"), defaultZero);
	r1Degree = StrToFloatDef(sValue, 0.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1RotateEndPositionMeters"), defaultOne);
	r2Position = StrToFloatDef(sValue, 1.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane1RotateEndPositionDegree"), defaultOne);
	r2Degree = StrToFloatDef(sValue, 1.0);

	int balkOffsetX, balkOffsetY;

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditBalkOffsetX"), L"100");
	balkOffsetX = StrToIntDef(sValue, 100);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditBalkOffsetY"), L"50");
	balkOffsetY = StrToIntDef(sValue, 50);

	int carriageOffsetX, carriageOffsetY;

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCarriageOffsetX"), L"-25");
	carriageOffsetX = StrToIntDef(sValue, -25);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCarriageOffsetY"), L"0");
	carriageOffsetY = StrToIntDef(sValue, 0);

	int arrowOffsetX, arrowOffsetY;

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditArrowOffsetX"), L"30");
	arrowOffsetX = StrToIntDef(sValue, 30);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditArrowOffsetY"), L"50");
	arrowOffsetY = StrToIntDef(sValue, 50);

	int balkWidth, balkHeight;

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditBalkWidth"), L"20");
	balkWidth = StrToIntDef(sValue, 20);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditBalkHeight"), L"800");
	balkHeight = StrToIntDef(sValue, 800);

	crane1 = new TCrane("CraneCarriage1");

	double dx = x2Position - x1Position;
	dx = (fabs(dx) < 1e-10) ? 1 : dx;

	double dy = y2Position - y1Position;
	dy = (fabs(dy) < 1e-10) ? 1 : dy;

	double dr = r2Position - r1Position;
	dr = (fabs(dr) < 1e-10) ? 1 : dr;

	crane1->setCoordinateResolution(
		(x2Pixels - x1Pixels) / dx,
		(y2Pixels - y1Pixels) / dy,
		(r2Degree - r1Degree) / dr
		);
	crane1->setBalkBaseOffset(balkOffsetX, balkOffsetY);
	crane1->setCarriageBaseOffset(carriageOffsetX, carriageOffsetY);
	crane1->setArrowCraneBaseOffset(arrowOffsetX, arrowOffsetY);
	crane1->displayArrow(true);
	crane1->setBalkWidth(balkWidth);
	crane1->setBalkHeight(balkHeight);
	crane1->setVisible(true);

	// initialize crane2

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane2BegiXPositionMeters"), defaultZero);
	x1Position = StrToFloatDef(sValue, 0.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane2BeginXPositionPixels"), defaultZero);
	x1Pixels = StrToFloatDef(sValue, 0.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane2EndXPositionMeters"), defaultOne);
	x2Position = StrToFloatDef(sValue, 1.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane2EndXPositionPixels"), defaultOne);
	x2Pixels = StrToFloatDef(sValue, 1.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane2BeginYPositionMeters"), defaultZero);
	y1Position = StrToFloatDef(sValue, 0.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane2BeginYPositionPixels"), defaultZero);
	y1Pixels = StrToFloatDef(sValue, 0.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane2EndYPositionMeters"), defaultOne);
	y2Position = StrToFloatDef(sValue, 1.0);

	sValue = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting(L"EditCrane2EndYPositionPixels"), defaultOne);
	y2Pixels = StrToFloatDef(sValue, 1.0);

	crane2 = new TCrane("CraneCarriage2");

	dx = x2Position - x1Position;
	dx = (fabs(dx) < 1e-10) ? 1 : dx;

	dy = y2Position - y1Position;
	dy = (fabs(dy) < 1e-10) ? 1 : dy;

	crane2->setBalkBaseOffset(balkOffsetX, balkOffsetY);
	crane2->setCarriageBaseOffset(carriageOffsetX, carriageOffsetY);
	crane2->setCoordinateResolution(
		(x2Pixels - x1Pixels) / dx,
		(y2Pixels - y1Pixels) / dy,
		1.0);
	crane2->setBalkWidth(balkWidth);
	crane2->setBalkHeight(balkHeight);
	crane2->setVisible(true);

	addCrane(crane1);

	addCrane(crane2);
}

void TCraneMimicPanel::initializeSensors() {
	crane1x = NULL;
	String Crane1XSensorUUID = DataModuleMP->GetSetting("Crane1XSensorUUID");
	if (!Crane1XSensorUUID.IsEmpty()) {
    	crane1x = GetSensor(Crane1XSensorUUID);
	}
	if (crane1x == NULL) {
		sysLogger::ERR_A("can't find sensor for crane 1 x position");
	}

	crane1y = NULL;
	String Crane1YSensorUUID = DataModuleMP->GetSetting("Crane1YSensorUUID");
	if (!Crane1YSensorUUID.IsEmpty()) {
		crane1y = GetSensor(Crane1YSensorUUID);
	}
	if (crane1y == NULL) {
		sysLogger::ERR_A("can't find sensor for crane 1 y position");
	}

	crane1d = NULL;
	String Crane1DegreeSensorUUID = DataModuleMP->GetSetting("Crane1DegreeSensorUUID");
	if (!Crane1DegreeSensorUUID.IsEmpty()) {
		crane1d = GetSensor(Crane1YSensorUUID);
	}
	if (crane1d == NULL) {
		sysLogger::ERR_A("can't find sensor for crane 1 degree");
	}

	crane2x = NULL;
	String Crane2XSensorUUID = DataModuleMP->GetSetting("Crane2XSensorUUID");
	if (!Crane2XSensorUUID.IsEmpty()) {
		crane2x = GetSensor(Crane2XSensorUUID);
	}
	if (crane2x == NULL) {
		sysLogger::ERR_A("can't find sensor for crane 2 x");
	}

	crane2y = NULL;
	String Crane2YSensorUUID = DataModuleMP->GetSetting("Crane2YSensorUUID");
	if (!Crane2YSensorUUID.IsEmpty()) {
		crane2y = GetSensor(Crane2YSensorUUID);
	}
	if (crane2y == NULL) {
		sysLogger::ERR_A("can't find sensor for crane 2 y");
	}
}

void TCraneMimicPanel::addCrane(TCrane *crane) {
	crane->setCanvas(canvas);
	cranes.push_back(crane);
}

void TCraneMimicPanel::draw() {
	for (std::list<TCrane*>::iterator it = cranes.begin(); it != cranes.end(); ++it) {
		(*it)->draw();
	}
}

void TCraneMimicPanel::clear() {
	//draw background image for clearing
	canvas->Draw(0, 0, bitmapBackground);
}

TCrane * TCraneMimicPanel::getCrane() {
	return cranes.front();
}

void TCraneMimicPanel::move(void *sensorId, double x, double y) {
	TCrane *crane = getCrane();
	if (!crane) {
		return;
	}

	crane->move(x, y);
}

void TCraneMimicPanel::rotate(void *sensorId, double angleDegree) {
	TCrane *crane = getCrane();
	if (!crane) {
		return;
	}

	crane->rotateAsDegree(angleDegree);
}

void TCraneMimicPanel::UpdateData(sysObserverable::IObserverable *task) {
	if (task->GetObserverableType() != TObserverableTypes::DEVICE_HTTP_DATA) {
		return;
	}

	if (crane1x == NULL || crane1y == NULL || crane1d == NULL || crane2x == NULL || crane2y == NULL) {
		return;
	}

	TTaskRequestHTTPData *httpTask = static_cast<TTaskRequestHTTPData *>(task);

	std::map<const TSensor *, TSensorData *> &data = httpTask->data;

	const TSensor *sensor;
	TSensorData *sensorData;

	bool redrawMimicPanel = false;

	for (std::map<const TSensor *, TSensorData *>::iterator i = data.begin(), iEnd = data.end(); i != iEnd; ++i) {
		sensor = i->first;

		sensorData = i->second;
		if (sensorData == NULL) {
			continue;
		}

		if (sensor == crane1x) {
			double xMeters = SensorDataToDouble(crane1x, sensorData);
			crane1->moveXMeters(xMeters);
			redrawMimicPanel = true;

		} else if (sensor == crane1y) {
			double yMeters = SensorDataToDouble(crane1y, sensorData);
			crane1->moveYMeters(yMeters);
			redrawMimicPanel = true;

		} else if (sensor == crane1d) {
			double rotatePosition = SensorDataToDouble(crane1d, sensorData);
			crane1->rotateAsMeters(rotatePosition);
			redrawMimicPanel = true;

		} else if (sensor == crane2x) {
			double xMeters = SensorDataToDouble(crane2x, sensorData);
			crane2->moveXMeters(xMeters);
			redrawMimicPanel = true;

		} else if (sensor == crane2x) {
			double yMeters = SensorDataToDouble(crane2x, sensorData);
			crane2->moveYMeters(yMeters);
			redrawMimicPanel = true;
		}
	}

	if (redrawMimicPanel) {
		clear();
		draw();
	}
}
