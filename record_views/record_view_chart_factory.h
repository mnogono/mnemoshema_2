//---------------------------------------------------------------------------
#ifndef record_view_chart_factoryH
#define record_view_chart_factoryH

#include "mnemoshema_view.h"
#include "record_view.h"
#include "sensor_view_chart.h"
#include "sensor_view_numeric_chart.h"
#include "chart_time_axis.h"
#include "sensor_bit_view_numeric_chart.h"
#include "sensor_bit_view_text_chart.h"
#include "sensor_bits_view_numeric_chart.h"

//---------------------------------------------------------------------------
class TRecordViewChartFactory {
	private:
		TRecordViewChartFactory();
	public:
		 static IMnemoshemaView * Build(TWinControl *parent, const TRecord *record, TChartTimeAxis *timeAxis);
};
#endif
