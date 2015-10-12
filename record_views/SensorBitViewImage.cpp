//---------------------------------------------------------------------------


#pragma hdrstop

#include "SensorBitViewImage.h"

extern TUser *USER;

TDefaultGraphicsBitmap * TDefaultGraphicsBitmap::instance = NULL;
/*
Graphics::TBitmap *TSensorBitViewImage::imgBitValue  = new Graphics::TBitmap;
Graphics::TBitmap *TSensorBitViewImage::imgBitValue0 = new Graphics::TBitmap;
Graphics::TBitmap *TSensorBitViewImage::imgBitValue1 = new Graphics::TBitmap;
*/
//---------------------------------------------------------------------------
TSensorBitViewImage::TSensorBitViewImage(TWinControl *parent, TSensorBit *sensorBit) : TSensorView(parent, sensorBit){
    if (TLogger::logger->CheckLogLevel(LOG_LEVEL_TRACE)) {
		TLogger::logger->Log("TSensorBitViewImage create: " + record->uuid, LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);
    }

	sensor = USER->GetSensor(sensorBit->sensor_id);

	this->sensorBit = sensorBit;

	CreateControls();
}

//---------------------------------------------------------------------------
void TSensorBitViewImage::DisplayData(TData *data){
    TImage *imgBitValue = (TImage *)viewControl;

	if (!data) {
		//imgBitValue->Picture->Bitmap = TSensorBitViewImage::imgBitValue;
		imgBitValue->Picture->Bitmap = TDefaultGraphicsBitmap::GetInstance()->imgBitValue;
		return;
	}

	bool bitValue;


	if (sensor->data_type == DATA_TYPE_CHAR) {
		std::bitset<8> bits(((TDataFile<char> *)data)->data);
		bitValue = bits.test(sensorBit->bit);

	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
		std::bitset<8> bits(((TDataFile<unsigned char> *)data)->data);
		bitValue = bits.test(sensorBit->bit);

	} else if (sensor->data_type == DATA_TYPE_SHORT) {
		std::bitset<16> bits(((TDataFile<short> *)data)->data);
		bitValue = bits.test(sensorBit->bit);

	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
		std::bitset<16> bits(((TDataFile<unsigned short> *)data)->data);
		bitValue = bits.test(sensorBit->bit);

	} else if (sensor->data_type == DATA_TYPE_INT) {
		std::bitset<32> bits(((TDataFile<int> *)data)->data);
		bitValue = bits.test(sensorBit->bit);

	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
		std::bitset<32> bits(((TDataFile<unsigned int> *)data)->data);
		bitValue = bits.test(sensorBit->bit);

	} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
		throw EWrongDisplayType("Ќеверное отображение бита данных значени€ double");

	} else if (sensor->data_type == DATA_TYPE_FLOAT) {
		throw EWrongDisplayType("Ќеверное отображение бита данных значени€ float");

	} else if (sensor->data_type == DATA_TYPE_INT64) {
		std::bitset<64> bits(((TDataFile<__int64> *)data)->data);
		bitValue = bits.test(sensorBit->bit);

	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT64) {
		std::bitset<64> bits(((TDataFile<unsigned __int64> *)data)->data);
		bitValue = bits.test(sensorBit->bit);

	} else if (sensor->data_type == DATA_TYPE_STRING) {
		throw EWrongDisplayType("Ќеверное отображение бита данных значени€ string");
	}

	if (bitValue) {
        imgBitValue->Picture->Bitmap = imgValue1;

	} else {
        imgBitValue->Picture->Bitmap = imgValue0;
    }
}

//---------------------------------------------------------------------------
void TSensorBitViewImage::CreateControls(){
	TSensorBit *sensorBit = dynamic_cast<TSensorBit *>(record);
	if (!sensorBit) return;

    int imgIdValue0 = -1;
    int imgIdValue1 = -1;

    try {
	    imgIdValue0 = StrToInt(sensorBit->value0);
    } catch (Exception &e) {
		TLogger::logger->Log(e.ToString(), LOG_LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__);
    }

    try {
        imgIdValue1 = StrToInt(sensorBit->value1);
    } catch (Exception &e) {
		TLogger::logger->Log(e.ToString(), LOG_LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__);
    }

    if (imgIdValue0 > 0) {
		TImageMnemo *imageMnemo = IMAGES.Get(imgIdValue0);
        if (imageMnemo) {
			imgValue0 = imageMnemo->bitmap;
        } else {
        	if (TLogger::logger->CheckLogLevel(LOG_LEVEL_WARNING)) {
            	String log = "can't find imgage in database, imgIdValue0: " + IntToStr(imgIdValue0);
				TLogger::logger->Log(log.c_str(), LOG_LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__);
            }
			//imgValue0 = TSensorBitViewImage::imgBitValue0;
			imgValue0 = TDefaultGraphicsBitmap::GetInstance()->imgBitValue0;
		}
	} else {
		//imgValue0 = TSensorBitViewImage::imgBitValue0;
		imgValue0 = TDefaultGraphicsBitmap::GetInstance()->imgBitValue0;
	}

    if (imgIdValue1 > 0) {
		TImageMnemo *imageMnemo = IMAGES.Get(imgIdValue1);
        if (imageMnemo) {
			imgValue1 = imageMnemo->bitmap;
        } else {
        	if (TLogger::logger->CheckLogLevel(LOG_LEVEL_WARNING)) {
            	String log = "can't find imgage in database, imgIdValue1: " + IntToStr(imgIdValue1);
				TLogger::logger->Log(log.c_str(), LOG_LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__);
            }
			//imgValue1 = TSensorBitViewImage::imgBitValue1;
			imgValue1 = TDefaultGraphicsBitmap::GetInstance()->imgBitValue1;
		}
	} else {
		//imgValue1 = TSensorBitViewImage::imgBitValue1;
		imgValue1 = TDefaultGraphicsBitmap::GetInstance()->imgBitValue1;
    }

	//viewControl = (TImage *)sysVCL::FindControl(parent, sensorBit->uuid);
	//String exeFile = Application->ExeName;
	//String exePath = ExtractFilePath(exeFile);
	//String imgBitValuePath = exePath + "img\\imgBitValue.bmp";

	//TImage *imgBitValue = (TImage *)viewControl;
    if (viewControl && !dynamic_cast<TImage *>(viewControl)) {
    	std::vector<TControl *>::iterator it = std::find(controls.begin(), controls.end(), viewControl);
        if (it != controls.end()) {
            controls.erase(it);
        }

		delete viewControl;
        viewControl = NULL;
    }

	if (!viewControl) {
		viewControl = new TImage(parent);
		viewControl->Parent = parent;
		viewControl->Name = record->uuid;
		viewControl->Height = 32;
		viewControl->Width = 32;
	}

    TImage *imgBitValue = dynamic_cast<TImage *>(viewControl);

	//imgBitValue->Picture->LoadFromFile(imgBitValuePath);
	//imgBitValue->Picture->Bitmap = TSensorBitViewImage::imgBitValue;
	imgBitValue->Picture->Bitmap = TDefaultGraphicsBitmap::GetInstance()->imgBitValue;

	/*
	Graphics::TBitmap* bm = new Graphics::TBitmap;
	bm->LoadFromFile(imgBitValuePath);

	TImage *imgBitValue = (TImage *)viewControl;
	imgBitValue->Stretch = true;
	imgBitValue->Picture->Graphic = bm;
	imgBitValue->Canvas->Draw(0, 0, bm);
	*/

	TLabel *leftLabel = (TLabel *)sysVCL::FindControl(parent, TSensorViewEdit::LABEL_LEFT_PREFIX + sensorBit->uuid);
	if (!leftLabel) {
		leftLabel = new TLabel(parent);
		leftLabel->Parent = parent;
		leftLabel->Name = TSensorViewEdit::LABEL_LEFT_PREFIX + sensorBit->uuid;
		leftLabel->Caption = sensorBit->desc;
		leftLabel->AutoSize = true;
		leftLabel->Transparent = true;
		leftLabel->Alignment = taRightJustify;
		leftLabel->Left = viewControl->Left - leftLabel->Width - 5;
		leftLabel->Top = viewControl->Top + (viewControl->Height - leftLabel->Height) / 2;
	}

	TLabel *rightLabel = (TLabel *)sysVCL::FindControl(parent, TSensorViewEdit::LABEL_RIGHT_PREFIX + sensorBit->uuid);
	if (!rightLabel) {
		rightLabel = new TLabel(parent);
		rightLabel->Parent = parent;
		rightLabel->Name = TSensorViewEdit::LABEL_RIGHT_PREFIX + sensorBit->uuid;
		rightLabel->Caption = "";
		rightLabel->AutoSize = true;
		rightLabel->Transparent = true;
		rightLabel->Alignment = taLeftJustify;
		rightLabel->Left = viewControl->Left + viewControl->Width + 5;
		rightLabel->Top = viewControl->Top + (viewControl->Height - rightLabel->Height) / 2;
	}

	if (std::find(controls.begin(), controls.end(), viewControl) == controls.end()) {
		controls.push_back(viewControl);
	}

	if (std::find(controls.begin(), controls.end(), leftLabel) == controls.end()) {
		controls.push_back(leftLabel);
	}

	if (std::find(controls.begin(), controls.end(), rightLabel) == controls.end()) {
		controls.push_back(rightLabel);
	}

    if (TLogger::logger->CheckLogLevel(LOG_LEVEL_TRACE)) {
	    TLogger::logger->Log("controls.size() : " + IntToStr((int)controls.size()) + " record->uuid : " + record->uuid, LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);
    }
}

//---------------------------------------------------------------------------
void TSensorBitViewImage::SetUserSetting(TUser *user, int status){
}

//---------------------------------------------------------------------------
void TSensorBitViewImage::DisplayAlarm(std::vector<TData *> *alarm) {
}

//---------------------------------------------------------------------------
TLabel * TSensorBitViewImage::GetLabelLeft() {
	return (TLabel *)GetControl(TSensorViewEdit::LABEL_LEFT_PREFIX + record->uuid);
}

//---------------------------------------------------------------------------
TLabel * TSensorBitViewImage::GetLabelRight() {
	return (TLabel *)GetControl(TSensorViewEdit::LABEL_RIGHT_PREFIX + record->uuid);
}


#pragma package(smart_init)
