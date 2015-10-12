//---------------------------------------------------------------------------
#ifndef record_view_chartH
#define record_view_chartH

//disable warning about all interface should be inherted from IUnknown
#pragma option push -w-8130

#include "record_view.h"
#include "chart_time_axis.h"
#include "time_axis_observer.h"
#include "observerable_types.h"
#include "observerable_history_date_time.h"
#include "chart_tracker.h"
#include "chart_trackable.h"
#include "chart_vertical_line.h"
//---------------------------------------------------------------------------
class TRecordViewChart :
	public TChart,
	public IMnemoshemaView,
	public sysObserver::IObserver {

	public:
		TChartTracker *chartTracker;

		TChartVerticalLine *chartVerticalLine;
	protected:
		const TRecord * record;

		//vcl owner control
		TWinControl *owner;

		//autotraking time axis
		bool autoTrackTimeAxis;

		//autotraking value axis
		bool autoTrackValueAxis;

		TChartTimeAxis *timeAxis;

		std::list<TChartTrackable *> chartTrackableElements;
	private:
		//auto adjust left axis value
		bool autoAdjust;

	public:
		TRecordViewChart(TWinControl *owner, const TRecord *record, TChartTimeAxis *timeAxis);

		inline __fastcall virtual ~TRecordViewChart();

		virtual void MoveTo(int x, int y);

		virtual void CreateControls();

		virtual void DisplayData(TSensorData *data = NULL) = 0;

		virtual void DisplayData(std::list<TSensorData *> *data = NULL) = 0;

		virtual void Highlight();

		virtual void UndoHighlight();

		virtual bool IsHighlighted();

		virtual void OnTimeAxisChanged(double dt1LocalTime, double dt2LocalTime);

		virtual void SetDefaultTitle();

		virtual const TRecord * GetRecord();

		virtual void UpdateData(sysObserverable::IObserverable *data = NULL);

		virtual void SetPopupMenu(TPopupMenu *menu);

		virtual void ChangeMinMaxLeftAxis(double coefficient) = 0;

		virtual void SetTimeAxisTracking(bool enable);

		virtual void SetValueAxisTracking(bool enable);

		virtual void TrackTimeAxis();

		virtual void TrackValueAxis();

		virtual double GetTimeAxisMinValue() = 0;

		virtual double GetTimeAxisMaxValue() = 0;

		virtual double GetValueAxisMaxValue() = 0;

		virtual double GetValueAxisMinValue() = 0;

		virtual void SetValueAxisMinMaxValue(double min, double max) = 0;

		virtual void AddTrackableElement(TChartTrackable *trackable);

		virtual void RemoveTrackableElement(TChartTrackable *trackable);

		std::list<TChartTrackable *> * GetTrackableElements();

		bool IsExistTrackableElement(TChartTrackable *trackable);

		virtual TControl * GetControl();

		virtual TChartTracker * CreateChartTracker(TChart *chart, const TRecord *record);

		//increase and decrease the min and max value by 10% to better display data for user
		void AdjustValueAxis();

		virtual void UpdateValue();

		//return current cursor values for each series
		//x [out] bottom time axis values
		virtual std::map<TChartSeries *, double> GetSeriesValues(double localTime) = 0;

		//void SetAutoAdjust(bool autoAdjust);
};

#pragma option pop

#endif
