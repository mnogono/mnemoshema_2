//---------------------------------------------------------------------------
#ifndef sensors_tab_sheetH
#define sensors_tab_sheetH

#include "record_view_chart.h"
#include "record_view_chart_factory.h"
#include "mnemoshema_data_manager.h"
#include "http_data_observer.h"
#include "time_range_data_observer.h"
#include "mouse_strategy.h"
#include "chart_time_axis.h"
//---------------------------------------------------------------------------

//disable warning about all interface should be inherted from IUnknown
#pragma option push -w-8130

class TSensorsTabSheet :
	public TTabSheet,
	public sysObserver::IObserver,
	public ITimeAxisEvents {

	public:
		std::map<const TSensor *, IMnemoshemaView *> mshViews;

		std::map<const TSensorBit *, IMnemoshemaView *> mshSensorBitViews;

		// automatically change min/max  bottom axis and min/max left axis for all charts and timeAxis
		//bool followBySignal;

		TChartTimeAxis *timeAxis;
	private:
   		bool autoTracking;

        TMnemoshemaDataManager *mnemoshemaDataManager;

		TScrollBox *scrollBox;

		sysObserver::IObserver *httpDataObserver;

		sysObserver::IObserver *timeRangeDataObserver;

		TTimeAxisObserver *timeAxisObserver;

		double dt1LocalTime;

		double dt2LocalTime;

	public:
		__fastcall virtual TSensorsTabSheet(
			Classes::TComponent * AOwner,
			TMnemoshemaDataManager &mnemoshemaDataManager);

		__fastcall virtual ~TSensorsTabSheet();

		TRecordViewChart * __fastcall AddChart(const TRecord *record, TMouseStrategy *mouseStrategy/*, TPopupMenu *popupMenu*/);

		std::set<const TSensor *> GetSensors();

		void UpdateData(sysObserverable::IObserverable *data = NULL);

		virtual void OnTimeAxisChanged(double dt1LocalTime, double dt2LocalTime);

		virtual void SetAutoTracking(bool enable);

		void DeleteChart(TRecordViewChart *chart);

		void DeleteAllCharts();

		void GetTimeAxisLocalTime(double &dt1LocalTime, double &dt2LocalTime);

	private:
		void __fastcall CreateTimeAxis();

		void __fastcall CreateScrollBox();

		void __fastcall CreateHTTPDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager);

		void __fastcall CreateFileDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager);

		void __fastcall RegisterDataObservers(TMnemoshemaDataManager &mnemoshemaDataObserver);

		//void __fastcall CreateTimeAxisObserver(TChartTimeAxis *timeAxis, ITimeAxisEvents *timeAxisHandler);
};

#pragma option pop

#endif
