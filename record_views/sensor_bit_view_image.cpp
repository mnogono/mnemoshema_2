//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "sensor_bit_view_image.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TDefaultSensorBitImageBitmap *TDefaultSensorBitImageBitmap::instance = NULL;

//---------------------------------------------------------------------------
TDefaultSensorBitImageBitmap::TDefaultSensorBitImageBitmap() {
	imgBitValue  = new Graphics::TBitmap;
	imgBitValue0 = new Graphics::TBitmap;
	imgBitValue1 = new Graphics::TBitmap;

	imgBitValue->LoadFromResourceName(NULL,  L"ID_BMP_IMG_BIT_VALUE");
	imgBitValue0->LoadFromResourceName(NULL, L"ID_BMP_IMG_BIT_VALUE_0");
	imgBitValue1->LoadFromResourceName(NULL, L"ID_BMP_IMG_BIT_VALUE_1");
};

//---------------------------------------------------------------------------
TDefaultSensorBitImageBitmap * TDefaultSensorBitImageBitmap::GetInstance() {
	if (!instance) {
		instance = new TDefaultSensorBitImageBitmap();
	}
	return instance;
}

//---------------------------------------------------------------------------
TDefaultSensorBitImageBitmap::~TDefaultSensorBitImageBitmap() {
	delete imgBitValue;
	delete imgBitValue0;
	delete imgBitValue1;
}

//---------------------------------------------------------------------------
TSensorBitViewImage::TSensorBitViewImage(TWinControl *parent, TSensorBit *sensorBit) : TRecordView(parent, sensorBit) {
	this->sensorBit = sensorBit;

	imgValue0 = NULL;

	imgValue1 = NULL;
}

//---------------------------------------------------------------------------
void TSensorBitViewImage::DisplayData(TSensorData *data) {
	TImage *imgBitValue = static_cast<TImage *>(viewControl);
	String value = "";
	if (data != NULL) {
		value = SensorBitDataToString(sensorBit, data);
	}

	if (value.IsEmpty()) {
		imgBitValue->Picture->Bitmap = TDefaultSensorBitImageBitmap::GetInstance()->imgBitValue;

	} else if (value == L"0") {
		imgBitValue->Picture->Bitmap = imgValue0;

	} else if (value == L"1") {
		imgBitValue->Picture->Bitmap = imgValue1;
	}
}

//---------------------------------------------------------------------------
void TSensorBitViewImage::DisplayData(std::list<TSensorData *> *data) {
	TImage *imgBitValue = static_cast<TImage *>(viewControl);
	String value = "";
	if (data != NULL && data->size() != 0) {
		value = SensorBitDataToString(sensorBit, data->back());
	}

	if (value.IsEmpty()) {
		imgBitValue->Picture->Bitmap = TDefaultSensorBitImageBitmap::GetInstance()->imgBitValue;

	} else if (value == L"0") {
		imgBitValue->Picture->Bitmap = imgValue0;

	} else if (value == L"1") {
		imgBitValue->Picture->Bitmap = imgValue1;
	}
}

//---------------------------------------------------------------------------
void TSensorBitViewImage::MoveTo(int x, int y) {
}

//---------------------------------------------------------------------------
void TSensorBitViewImage::CreateControls() {
	TRecordView::CreateControls();

	int imgIdValue0 = StrToIntDef(sensorBit->value0, -1);
	int imgIdValue1 = StrToIntDef(sensorBit->value1, -1);

	if (imgIdValue0 > 0) {
		TImg *image = DataModuleMP->GetImage(imgIdValue0);
		if (image != NULL) {
			imgValue0 = image->bitmap;
		} else {
			sysLogger::WARN_A("can't find image value 0 in database");
		}
	}

	if (imgValue0 == NULL) {
		imgValue0 = TDefaultSensorBitImageBitmap::GetInstance()->imgBitValue0;
	}

    if (imgIdValue1 > 0) {
		TImg *image = DataModuleMP->GetImage(imgIdValue1);
		if (image != NULL) {
			imgValue1 = image->bitmap;
		} else {
			sysLogger::WARN_A("can't find image value 1 in database");
		}
	}

	if (imgValue1 == NULL) {
		imgValue1 = TDefaultSensorBitImageBitmap::GetInstance()->imgBitValue1;
	}

	if (!dynamic_cast<TImage *>(viewControl)) {
		controls.erase(viewControl);
		delete viewControl;
		viewControl = NULL;
	}

	if (viewControl == NULL) {
		viewControl = new TImage(parent);
		viewControl->Parent = parent;
		viewControl->Name = record->uuid;
		viewControl->Height = 32;
		viewControl->Width = 32;
	}

    TImage *imgBitValue = static_cast<TImage *>(viewControl);

	imgBitValue->Picture->Bitmap = TDefaultSensorBitImageBitmap::GetInstance()->imgBitValue;

	leftLabel = TRecordView::CreateLeftLabel(parent, record);

	rightLabel = TRecordView::CreateRightLabel(parent, record);

	controls.insert(viewControl);

	controls.insert(leftLabel);

	controls.insert(rightLabel);
}

//---------------------------------------------------------------------------
void TSensorBitViewImage::Highlight() {
}

//---------------------------------------------------------------------------
void TSensorBitViewImage::UndoHighlight() {
}

//---------------------------------------------------------------------------
bool TSensorBitViewImage::IsHighlighted() {
	return false;
}

//---------------------------------------------------------------------------
void TSensorBitViewImage::SetPopupMenu(TPopupMenu *menu) {
	static_cast<TImage *>(viewControl)->PopupMenu = menu;
}

