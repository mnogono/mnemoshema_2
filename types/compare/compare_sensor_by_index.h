//---------------------------------------------------------------------------
#ifndef compare_sensor_by_indexH
#define compare_sensor_by_indexH
//---------------------------------------------------------------------------

struct TCmpSensorByIndex {
	bool operator()(const TSensor *s1, const TSensor *s2) const;
};

#endif
