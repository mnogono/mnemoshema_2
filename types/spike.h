//---------------------------------------------------------------------------
#ifndef spikeH
#define spikeH
//---------------------------------------------------------------------------

struct TSpike {
	double time;
	bool isPositive;
	std::list<TSensorData *>::iterator itSensorData;
	int index;

	TSpike();

	TSpike(double time, bool isPositive, std::list<TSensorData *>::iterator i);

	TSpike(double time, bool isPositive, int index);

	static bool IsSpike(double y1, double y2);
};

#endif
