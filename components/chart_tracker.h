//---------------------------------------------------------------------------
#ifndef chart_trackerH
#define chart_trackerH

#include "chart_trackable.h"
#include "mnemoshema_view.h"
#include "chart_label.h"
//---------------------------------------------------------------------------

class TChartTracker : public TChartTrackable{
	public:
		double trackLocalTime;

		double trackValue;
	protected:
		TLabel *trackerLabel;

		TShape *tracker;

		TShape *trackerBackground;

		String unit;

		bool enabled;

		TChart *chart;

		const TRecord *record;

	public:
		TChartTracker(TChart *chart, const TRecord *record);

		virtual double GetLocalTime() const;

		virtual void SetLocalTime(double localTime);

		virtual void SetValue(double value);

		virtual double GetValue() const;

		virtual void SetVisible(bool visible);

		virtual bool TrackValue();

		virtual bool IsVisible() const;

		//virtual void Move(double localTime);

		virtual void Move(int x, int y);

		//virtual double GetLocalTime();

		virtual bool IsEnabled();

		virtual void SetEnabled(bool enabled);

		virtual void __fastcall OnMouseMove(TObject* Sender, TShiftState Shift, int X, int Y);

		void __fastcall OnMouseDown(TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		void __fastcall OnMouseUp(TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

		//static int BinSearchValueList(TChartValueList *A,  double key, int low, int high, double exp);

		virtual bool IsEqual(TChartTrackable *chartTrackable);

		virtual int GetType();
};

#endif
