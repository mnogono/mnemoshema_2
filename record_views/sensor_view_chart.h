//---------------------------------------------------------------------------
#ifndef sensor_view_chartH
#define sensor_view_chartH

#include "record_view_chart.h"
#include "chart_data.h"
//---------------------------------------------------------------------------

class TSensorViewChart : public TRecordViewChart {
	protected:
		TChartData chartData;
		/*
		//real size of data
    	size_t dataSize;

		//signal data y values
		//capacity of valaray container will be increase to prevent to offten changing the size of the container
		std::valarray<double> dataY;

		//signal data x values
		//capacity of valaray container will be increase to prevent to offten changing the size of the container
		std::valarray<double> dataX;
		*/

		const TSensor *sensor;
		//main signal series
		Vcltee::Series::TLineSeries *signal;

		/*
		max time duration of sensor signals can be apply for one unbreak series
		if time range between 2 near signals exceed this value, chart will break series signal line
		*/
		double sensorPeriodDay;

		/*
		acceptable dispersion percentage
		if value in signal series is out of mean +- acceptable dispersion percentage
		chart will try to remove it value out of serie
		*/
		double acceptableDispersionPercentage;
	public:
		TSensorViewChart(TWinControl *parent, const TSensor *sensor, TChartTimeAxis *timeAxis);

		inline __fastcall virtual ~TSensorViewChart();

		virtual void CreateControls();

		virtual void DisplayData(TSensorData *data = NULL);

		virtual void DisplayData(std::list<TSensorData *> *data = NULL);

		void CreateSignalSeries();

		TLineSeries * GetSignal();

		virtual void ChangeMinMaxLeftAxis(double coefficient);

		virtual double GetTimeAxisMinValue();

		virtual double GetTimeAxisMaxValue();

		virtual double GetValueAxisMaxValue();

		virtual double GetValueAxisMinValue();

		virtual void SetValueAxisMinMaxValue(double min, double max);

		virtual std::map<TChartSeries *, double> GetSeriesValues(double localTime);
};

#endif
