//---------------------------------------------------------------------------
#ifndef SensorViewEditH
#define SensorViewEditH

#include "record_view_edit.h"
//---------------------------------------------------------------------------

class TSensorViewEdit : public TRecordViewEdit {
	protected:
		TColor clHighlight;

		TColor clNormal;

		TSensor *sensor;

	public:
		TSensorViewEdit(TWinControl *parent, TSensor *sensor);

		virtual void DisplayData(TSensorData *data = NULL);

		virtual void DisplayData(std::list<TSensorData *> *data = NULL);

		virtual void SetUserSetting(TUser *user, int status);

		//virtual void DisplayAlarm(std::vector<TData *> *alarm);

		virtual void Highlight();

		virtual void UndoHighlight();

		virtual bool IsHighlighted();

		virtual TLabel * GetLabelLeft();

		virtual TLabel * GetLabelRight();

		virtual TLabel * GetLabelUnit();

		virtual void CreateControls();

		virtual void Resize(int width, int height);

	protected:
		//update controls position, after change width or height of viewControl
		void UpdateControlsPosition();
};

#endif
