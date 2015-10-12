//---------------------------------------------------------------------------

#ifndef RecordSensorBitViewChartH
#define RecordSensorBitViewChartH

#include "RecordViewChart.h"
#include <bitset>

//---------------------------------------------------------------------------

class TRecordSensorBitViewChart : public TRecordViewChart {
	private:
	    int count_of_period_before_brake;

		map<int, String> sensorBitValues;

		void DisplayMiddleDataAsNumeric(std::vector<TData *> *data, bool bClearSeries = true);

		void DisplayMiddleDataAsString(std::vector<TData *> *data, bool bClearSeries = true);

        void DisplayMiddleDataAs01(std::vector<TData *> *data, bool bClearSeries = true);

		void __fastcall SeriesGetMarkText(TChartSeries *Sender, int ValueIndex, String &MarkText);

	public:
		TRecordSensorBitViewChart(TWinControl *owner, TRecord *record, TUser *user, void (*SetChartSeriesSetings)(TLineSeries *, TUser *, String));

		virtual inline __fastcall ~TRecordSensorBitViewChart(void);

		virtual void DisplayData(std::vector<TData *> *data, bool bClearSeries = true);

		virtual String __fastcall GetName();
};

#endif
