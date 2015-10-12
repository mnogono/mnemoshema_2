//---------------------------------------------------------------------------

#ifndef RecordViewChartH
#define RecordViewChartH

#include "Sensor.h"
#include "User.h"
#include "db.h"
#include "lib.h"
#include "ChartText.h"
#include "ActiveAlarmObserver.h"
#include "VLShape.h"
#include "VLShapeAlarm.h"

#if __BORLANDC__ > 0x660
	#include <VCLTee.Chart.hpp>
	#include <VclTee.TeeGDIPlus.hpp>
	#include <VCLTee.TeEngine.hpp>
	#include <VCLTee.TeeProcs.hpp>
	#include <VCLTee.Series.hpp>
#else
	#include "Chart.hpp"
	#include "Series.hpp"
	#include "TeEngine.hpp"
	#include "TeeProcs.hpp"
#endif

//---------------------------------------------------------------------------
class TLabelBitValue : public TLabel {
	public:
		double startTime;
		double endTime;
		int val;

		virtual __fastcall TLabelBitValue(Classes::TComponent *AOwner);

		static TLabelBitValue * Create(TWinControl *AOwner, double startTime, double endTime, String text, int val);
};

//---------------------------------------------------------------------------
/*
extern int CHART_SERIES_DATA_INDEX;
extern int CHART_SERIES_ALARM_MIN_INDEX;
extern int CHART_SERIES_ALARM_MAX_INDEX;
extern int CHART_SERIES_VIRTUAL_ALARM_MIN_INDEX;
extern int CHART_SERIES_VIRTUAL_ALARM_MAX_INDEX;
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TRecordViewChart :
	public TChart
	,public TActiveAlarmObserver {

	private:
		enum TColorChanges {makeLighter, makeDarknes};
		TColor ChangeColor(TColor color, TColorChanges colorChanges);
		char progressBarStep;

	protected:
		__int64 iMillisecondsGMT1;
		__int64 iMillisecondsGMT2;
		long iSecondsGMT1;
		long iSecondsGMT2;
		TUser *user;
		TWinControl *progressBar;

		//temp solution
		bool isProgressBarVisible;

		void __fastcall CreateProgressBar();

	public:
		TLineSeries *series;
		TLineSeries *max_series;
		TLineSeries *min_series;
		TLineSeries *vmax_series;
		TLineSeries *vmin_series;

		double maxYScrollValue;
		double minYScrollValue;

		TLabel *track;

		TLabel *vline;

		TShape *trackMarker;

		TSensor *sensor;

		TRecord *record;

		//TColor OriginalColor;

		std::vector<TMemo *> *memoTexts;

		bool enableTrack;

		//automatically change min/max  bottom axis and min/max left axis
		//bool followBySignal;

		//void __fastcall (*ChangeMouseStrategy)(TRecordViewChart *, TChartMouseStrategy);

		virtual __fastcall TRecordViewChart(Classes::TComponent *AOwner, TRecord *record, TUser *user, void (*SetChartSeriesSetings)(TLineSeries *, TUser *, String));

		void SetPopupMenuChart(TPopupMenu *PopupMenuChart);

		void SetUser(TUser *user);

		virtual inline __fastcall ~TRecordViewChart(void);

		void (*SetChartSeriesSetings)(TLineSeries *, TUser *, String);

		//virtual void SetMouseStrategy(TChartMouseStrategy mouseStrategy);

		//void SetTrekEnabled(bool enableTrek);

		void SetTrekSettings(TUser *User);

		void UpdateVLine(int x, int y);

    //Update visibility of chart controls elements
		void UpdateChartControls();

		void SetLimits(double dtLocalTime1, double dtLocalTime2, TUser *User);

		virtual void DisplayData(std::vector<TData *> *data, bool bClearSeries = true);

		//void __fastcall RestoreChartZoom(TDateTime *dtGMT1, TDateTime *dtGMT2);
		void __fastcall RestoreChartZoom(double fGMT1, double fGMT2);

		void __fastcall OnMouseMoveTrack(TObject *Sender, TShiftState Shift, int X, int Y);

		static int __fastcall BinSearchValueList(TChartValueList *A,  double key,  int low, int high, double exp);

		void __fastcall StartProgressBar();

		void __fastcall UpdateProgressBar();

		void __fastcall StopProgressBar();

		virtual String __fastcall GetName() = 0;

		void UpdateActiveAlarms(TSensor *sensor, std::vector<TData *> *activeAlarms);

		virtual void Highlight();

		virtual void UndoHighlight();
};

#endif
