//---------------------------------------------------------------------------
#ifndef sensor_bits_view_numeric_chartH
#define sensor_bits_view_numeric_chartH

#include "record_view_chart.h"
#include "chart_tracker_sensor_bits_view.h"
//---------------------------------------------------------------------------

class TSensorBitsViewNumericChart : public TRecordViewChart {
	protected:
		const TSensor *sensor;

		std::vector<Vcltee::Series::TLineSeries *> bitSignals;

		double sensorPeriodDay;

	public:
		TSensorBitsViewNumericChart(TWinControl *owner, const TSensor *sensor, TChartTimeAxis *timeAxis);

		virtual void DisplayData(TSensorData *data = NULL);

		virtual void DisplayData(std::list<TSensorData *> *data = NULL);

		void CreateSignalSeries();

		virtual void ChangeMinMaxLeftAxis(double coefficient);

		virtual double GetTimeAxisMinValue();

		virtual double GetTimeAxisMaxValue();

		virtual double GetValueAxisMinValue();

		virtual double GetValueAxisMaxValue();

		virtual void SetValueAxisMinMaxValue(double min, double max);

		virtual TChartTracker * CreateChartTracker(TChart *chart, const TRecord *record);

		virtual std::map<TChartSeries *, double> GetSeriesValues(double localTime);
};

#endif
