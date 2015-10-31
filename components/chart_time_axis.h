//---------------------------------------------------------------------------
#ifndef chart_time_axisH
#define chart_time_axisH

#include "observerable_history_date_time.h"
#include "mnemoshema_data_manager.h"
#include "observerable_change_time_axis_by_user.h"
//---------------------------------------------------------------------------

//disable warning about all interface should be inherted from IUnknown
#pragma option push -w-8130

class TChartTimeAxis : public sysObserverable::TObserverable {
	private:
		TChart *timeAxis;

		TLineSeries *timeSeries;

		TMnemoshemaDataManager *mnemoshemaDataManager;
	public:
		//when user drag the chart, sender object equals current dragged chart
		TObject *sender;
	public:
		TChartTimeAxis(TWinControl *owner, TMnemoshemaDataManager &mnemoshemaDataManager);

		~TChartTimeAxis();

		//return time axis chart height
		int GetHeight();

		//return time axis chart width
		int GetWidth();

		//paint chart time axis to hdc
		void __fastcall PaintTo(HDC DC, int X, int Y);

		//time axis bounds was changed, do need to request new data? yes when user move chart and release mouse button
		//or when user zoon in or zoom out time axis
		void SetTimeAxisBounds(double dt1LocalTime, double dt2LocalTime, bool requestData = false);

		void GetTimeAxisBounds(double &dt1LocalTime, double &dt2LocalTime);
	};

#pragma option pop

#endif
