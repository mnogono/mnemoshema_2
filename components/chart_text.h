//---------------------------------------------------------------------------
#ifndef chart_textH
#define chart_textH

#include "chart_trackable.h"
//---------------------------------------------------------------------------

//disable warning about all interface should be inherted from IUnknown
#pragma option push -w-8130

class TChartText :
	/*public TMemo,*/
	public TChartTrackable {

	public:
		double trackLocalTime;

		double trackValue;

	protected:
    	TMemo *memo;

		TChart *chart;

		TShape *box;

	public:
		TChartText(TChart *chart);

		inline __fastcall virtual ~TChartText();

		//return local time of the chart object
		virtual double GetLocalTime() const;

		//set trackable local time
		virtual void SetLocalTime(double localTime);

		//set trackable value
		virtual void SetValue(double value);

		virtual bool TrackValue();

		virtual double GetValue() const;

		void Highlight(bool highlight);

		void UpdateBox();

		//move trackable object to certain time
		virtual void Move(int x, int y);

		//show trackable element
		virtual void SetVisible(bool visible);

		//hide trackable element
		virtual bool IsVisible() const;

		virtual bool IsEqual(TChartTrackable *chartTrackable);

		virtual int GetType();
};

#pragma option pop

#endif
