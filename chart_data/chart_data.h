//---------------------------------------------------------------------------
#ifndef chart_dataH
#define chart_dataH
//---------------------------------------------------------------------------

struct TChartData {
	//real size of data
	size_t dataSize;

	//signal data y values
	//capacity of valaray container will be increase to prevent to offten changing the size of the container
	std::valarray<double> dataY;

	//signal data x values
	//capacity of valaray container will be increase to prevent to offten changing the size of the container
	std::valarray<double> dataX;

	TChartData();

	TChartData(const TChartData &src);

	void SetInitialSize(size_t initialCapacity);

	void SetSensorData(const TSensor *sensor, std::list<TSensorData *> *data);

	void AppendSensorData(const TSensor *sensor, TSensorData *data);
};

#endif
