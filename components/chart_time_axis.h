//---------------------------------------------------------------------------
#ifndef chart_time_axisH
#define chart_time_axisH

#include "observerable_history_date_time.h"
//---------------------------------------------------------------------------

//disable warning about all interface should be inherted from IUnknown
#pragma option push -w-8130

class TChartTimeAxis : public sysObserverable::TObserverable {
	private:
		TChart *timeAxis;

		TLineSeries *timeSeries;
	public:
		//when user drag the chart, sender object equals current dragged chart
		TObject *sender;
	public:
		TChartTimeAxis(TWinControl *owner);

		~TChartTimeAxis();

		//return time axis chart height
		int GetHeight();

		//return time axis chart width
		int GetWidth();

		//paint chart time axis to hdc
		void __fastcall PaintTo(HDC DC, int X, int Y);

		void SetTimeAxisBounds(double dt1LocalTime, double dt2LocalTime);

		void GetTimeAxisBounds(double &dt1LocalTime, double &dt2LocalTime);
	};

#pragma option pop

#endif
