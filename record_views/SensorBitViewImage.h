//---------------------------------------------------------------------------

#ifndef SensorBitViewImageH
#define SensorBitViewImageH

#include "SensorView.h"
#include "sysFile.h"
#include "SensorViewEdit.h"
#include "Images.h"

#include <Graphics.hpp>
#include <bitset>

extern TImages IMAGES;
//---------------------------------------------------------------------------

struct TDefaultGraphicsBitmap {
	private:
		static TDefaultGraphicsBitmap *instance;

		TDefaultGraphicsBitmap(){
			imgBitValue  = new Graphics::TBitmap;
			imgBitValue0 = new Graphics::TBitmap;
			imgBitValue1 = new Graphics::TBitmap;

			imgBitValue->LoadFromResourceName(NULL,  L"ID_BMP_IMG_BIT_VALUE");
			imgBitValue0->LoadFromResourceName(NULL, L"ID_BMP_IMG_BIT_VALUE_0");
			imgBitValue1->LoadFromResourceName(NULL, L"ID_BMP_IMG_BIT_VALUE_1");
		};
	public:
		Graphics::TBitmap *imgBitValue;  //no data
		Graphics::TBitmap *imgBitValue0; //bit value 0
		Graphics::TBitmap *imgBitValue1; //bit value 1

		static TDefaultGraphicsBitmap * GetInstance(){
			if (!instance) {
				instance = new TDefaultGraphicsBitmap();
			}
			return instance;
		}
		~TDefaultGraphicsBitmap(){
			delete imgBitValue;
			delete imgBitValue0;
			delete imgBitValue1;
		}
};

class TSensorBitViewImage : public TSensorView {
	protected:
		TSensor *sensor;
		TSensorBit *sensorBit;

	public:
		//bitmap of 3 default images for 3 states
		/*
		static Graphics::TBitmap *imgBitValue;  //no data
		static Graphics::TBitmap *imgBitValue0; //bit value 0
		static Graphics::TBitmap *imgBitValue1; //bit value 1
		*/
		TDefaultGraphicsBitmap *defaultGraphicBitmap;

        Graphics::TBitmap *imgValue0;
        Graphics::TBitmap *imgValue1;

		TSensorBitViewImage(TWinControl *parent, TSensorBit *sensorBit);

		virtual void DisplayData(TData *data);

		virtual void CreateControls();

		virtual void SetUserSetting(TUser *user, int status);

		virtual void DisplayAlarm(std::vector<TData *> *alarm);

		virtual TLabel * TSensorBitViewImage::GetLabelLeft();

		virtual TLabel * TSensorBitViewImage::GetLabelRight();
};

//load default bitmap from resource file
void CreateBitmapSensorBitView(){
	/*
	TSensorBitViewImage::imgBitValue->LoadFromResourceName(NULL,  L"ID_BMP_IMG_BIT_VALUE");
	TSensorBitViewImage::imgBitValue0->LoadFromResourceName(NULL, L"ID_BMP_IMG_BIT_VALUE_0");
	TSensorBitViewImage::imgBitValue1->LoadFromResourceName(NULL, L"ID_BMP_IMG_BIT_VALUE_1");
	*/
}

void DeleteBitmapSensorBitView(){
	/*
	TSensorBitViewImage::imgBitValue->FreeImage();
	TSensorBitViewImage::imgBitValue0->FreeImage();
	TSensorBitViewImage::imgBitValue1->FreeImage();
	*/
}

//load bitmap in memmory after start application
#pragma startup CreateBitmapSensorBitView 64

#pragma exit DeleteBitmapSensorBitView 64

#endif
