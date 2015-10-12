//---------------------------------------------------------------------------
#ifndef sensor_bit_view_text_chartH
#define sensor_bit_view_text_chartH

#include "record_view_chart.h"
//---------------------------------------------------------------------------
class TSensorBitViewTextChart : public TRecordViewChart {
	protected:
		const TSensorBit *sensorBit;

		//main signal series
		Vcltee::Series::TLineSeries *signal;

	public:
		TSensorBitViewTextChart(TWinControl *owner, const TSensorBit *sensorBit, TChartTimeAxis *timeAxis);

		virtual void DisplayData(TSensorData *data = NULL);

		virtual void DisplayData(std::list<TSensorData *> *data = NULL);

		virtual void ChangeMinMaxLeftAxis(double coefficient);

		virtual double GetTimeAxisMinValue();

		virtual double GetTimeAxisMaxValue();

		virtual double GetValueAxisMaxValue();

		virtual double GetValueAxisMinValue();

		virtual void SetValueAxisMinMaxValue(double min, double max);

		virtual std::map<TChartSeries *, double> GetSeriesValues(double localTime);
};

#endif
