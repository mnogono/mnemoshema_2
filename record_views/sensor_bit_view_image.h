//---------------------------------------------------------------------------
#ifndef sensor_bit_view_imageH
#define sensor_bit_view_imageH

#include "record_view.h"

//---------------------------------------------------------------------------
class TDefaultSensorBitImageBitmap {
	public:
		Graphics::TBitmap *imgBitValue;  //no data
		Graphics::TBitmap *imgBitValue0; //bit value 0
		Graphics::TBitmap *imgBitValue1; //bit value 1

		static TDefaultSensorBitImageBitmap * GetInstance();

		~TDefaultSensorBitImageBitmap();
	private:
		static TDefaultSensorBitImageBitmap *instance;

		TDefaultSensorBitImageBitmap();
};

//---------------------------------------------------------------------------
class TSensorBitViewImage : public TRecordView{
	protected:
		TSensorBit *sensorBit;

		Graphics::TBitmap *imgValue0;

		Graphics::TBitmap *imgValue1;

		TLabel *leftLabel;

		TLabel *rightLabel;

	public:
		TSensorBitViewImage(TWinControl *parent, TSensorBit *sensorBit);

		virtual void DisplayData(TSensorData *data = NULL);

		virtual void DisplayData(std::list<TSensorData *> *data = NULL);

		virtual void MoveTo(int x, int y);

		virtual void CreateControls();

		virtual void Highlight();

		virtual void UndoHighlight();

		virtual bool IsHighlighted();

		virtual void SetPopupMenu(TPopupMenu *menu);
};

#endif
