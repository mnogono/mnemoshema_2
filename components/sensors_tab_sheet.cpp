//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "sensors_tab_sheet.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TSensorsTabSheet::TSensorsTabSheet(
	Classes::TComponent * AOwner,
	TMnemoshemaDataManager &mnemoshemaDataManager/*,
	ITimeAxisEvents *timeAxisHandler*/) : TTabSheet(AOwner) {

	//assert(timeAxisHandler != NULL);

	this->mnemoshemaDataManager = &mnemoshemaDataManager;

	TPageControl *pageControl = dynamic_cast<TPageControl *>(AOwner);

    assert(pageControl != NULL);

	Parent = pageControl;

	PageControl = pageControl;

	timeAxis = NULL;

	autoTracking = true;
	//followBySignal = true;

	CreateTimeAxis();

	CreateScrollBox();

	RegisterDataObservers(mnemoshemaDataManager);

	//CreateTimeAxisObserver(timeAxis, timeAxisHandler);
};

//---------------------------------------------------------------------------
void __fastcall TSensorsTabSheet::RegisterDataObservers(TMnemoshemaDataManager &mnemoshemaDataManager) {
	mnemoshemaDataManager.AddObserver(this);

	CreateHTTPDataObserver(mnemoshemaDataManager);

	CreateFileDataObserver(mnemoshemaDataManager);
}

//---------------------------------------------------------------------------
TRecordViewChart * __fastcall TSensorsTabSheet::AddChart(const TRecord *record, TMouseStrategy *mouseStrategy/*, TPopupMenu *popupMenu*/) {
	assert(scrollBox != NULL);

	IMnemoshemaView *view = NULL;

	const TSensor *sensor = NULL;
	if (record->record_type == RECORD_TYPE_SENSOR) {
		sensor = static_cast<const TSensor *>(record);

		std::map<const TSensor *, IMnemoshemaView *>::iterator itView = mshViews.find(sensor);
		if (itView != mshViews.end()) {
			//such record already have view on this tab sheet
			return static_cast<TRecordViewChart *>(itView->second);
		}

		//add chart component to parent (scrollBox)
		view = TRecordViewChartFactory::Build(scrollBox, record, timeAxis);
		if (view == NULL) {
			return NULL;
		}

		mshViews[sensor] = view;

	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		const TSensorBit *sensorBit = static_cast<const TSensorBit *>(record);
		std::map<const TSensorBit *, IMnemoshemaView *>::iterator itView = mshSensorBitViews.find(sensorBit);
		if (itView != mshSensorBitViews.end()) {
			//such record already have view on this tab sheet
			return static_cast<TRecordViewChart *>(itView->second);
		}

		view = TRecordViewChartFactory::Build(scrollBox, sensorBit, timeAxis);
		if (view == NULL) {
			return NULL;
		}

		mshSensorBitViews[sensorBit] = view;
	}

	TRecordViewChart *chart = static_cast<TRecordViewChart *>(view);

	mouseStrategy->Apply(chart);

	return chart;
}

//---------------------------------------------------------------------------
__fastcall TSensorsTabSheet::~TSensorsTabSheet() {
	if (timeAxis != NULL) {
		timeAxis->RemoveObserver(timeAxisObserver);
	}

	if (timeAxisObserver != NULL) {
		delete timeAxisObserver;
	}

	if (timeAxis != NULL) {
		delete timeAxis;
	}

	if (mnemoshemaDataManager != NULL) {
		mnemoshemaDataManager->RemoveObserver(httpDataObserver);
		mnemoshemaDataManager->RemoveObserver(timeRangeDataObserver);
	}

	if (httpDataObserver != NULL) {
		delete httpDataObserver;
	}

	if (timeRangeDataObserver != NULL) {
		delete timeRangeDataObserver;
	}

	//maybe do not need to free the memory because the owner component must free the resource
	/*
	if (timeAxis != NULL) {
		delete timeAxis;
		timeAxis = NULL;
	}

	if (timeSeries != NULL) {
		delete timeSeries;
		timeSeries = NULL;
	}

	if (scrollBox != NULL) {
		delete scrollBox;
		scrollBox = NULL;
	}
	*/

	//TODO unsubscribe from mnemoshemaDataManager

	/*
	TODO make it in future
	for (std::map<const TRecord *, std::list<TRecordView *> *>::iterator i = sensorViews.begin(), iEnd = sensorViews.end(); i != iEnd; ++i) {
		std::list<TRecordViewChart *> *recordViews = i->second;

		for (std::list<TRecordViewChart *>::iterator j = recordViews->begin(), jEnd = recordViews->end(); j != jEnd; ++j) {
			delete *j;
		}
	}
	*/
}

// ---------------------------------------------------------------------------
void __fastcall TSensorsTabSheet::CreateScrollBox() {
	scrollBox = new TScrollBox(this);
	scrollBox->Parent = this;
	scrollBox->Align = alClient;
	scrollBox->AutoScroll = true;
	scrollBox->HorzScrollBar->Tracking = false;
	scrollBox->VertScrollBar->Tracking = true;
	scrollBox->VertScrollBar->Visible = true;
	scrollBox->BorderStyle = bsNone;
	scrollBox->AutoSize = false;
	scrollBox->Height = Height - timeAxis->GetHeight() - 1;
};

// ---------------------------------------------------------------------------
void __fastcall TSensorsTabSheet::CreateTimeAxis() {
	timeAxis = new TChartTimeAxis(this, *mnemoshemaDataManager);
	/*
	timeAxis = new TChart(this);

	timeAxis->Parent = this;
	timeAxis->View3D = false;
	timeAxis->Legend->Visible = false;
	timeAxis->LeftAxis->Visible = false;
	timeAxis->Align = alBottom;
	timeAxis->Height = 45;

	timeAxis->LeftAxis->LabelsSize = 50;

	timeAxis->RightAxis->Visible = false;
	timeAxis->TopAxis->Visible = false;
	timeAxis->Border->Visible = false;

	timeAxis->BottomAxis->DateTimeFormat = "hh:nn:ss";
	timeAxis->BottomAxis->LabelsMultiLine = true;

	timeAxis->Title->Visible = false;

	timeAxis->MarginUnits = TTeeUnits::muPixels;
	timeAxis->MarginLeft = 56;
	timeAxis->MarginRight = 5;
	timeAxis->MarginTop = 0;
	timeAxis->MarginBottom = 15;

	timeAxis->BevelInner = TBevelCut::bvNone;
	timeAxis->BevelOuter = TBevelCut::bvNone;

	timeSeries = new TLineSeries(timeAxis);
	timeSeries->XValues->DateTime = true;
	timeAxis->AddSeries(timeSeries);
	*/
}

// ---------------------------------------------------------------------------
//void __fastcall TSensorsTabSheet::SetTimeAxisBounds(double dtLocalTime1, double dtLocalTime2) {

	/*
	//update bottom time axis min and max values
	timeAxis->timeAxis->BottomAxis->SetMinMax(dtLocalTime1, dtLocalTime2);

	assert(scrollBox != NULL);

	//update time axis margin
	if (scrollBox->VertScrollBar->IsScrollBarVisible()){
		int scrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		timeAxis->MarginRight = 5 + scrollBarWidth;
	} else {
		timeAxis->MarginRight = 5;
	}

	//update time series
	TLineSeries *timeSeries = static_cast<TLineSeries *>(timeAxis->Series[0]);

	//display 100 ticks
	double dt = (dtLocalTime2 - dtLocalTime1) / 100.0;

	timeSeries->Clear();

	for (int i = 0; i < 100; i++){
		timeSeries->AddXY(dtLocalTime1 + i *dt, 0.0, "", clBlack);
	}
	*/
//}

//---------------------------------------------------------------------------
void __fastcall TSensorsTabSheet::CreateHTTPDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	httpDataObserver = new THTTPDataObserver(mshViews);
	static_cast<THTTPDataObserver *>(httpDataObserver)->SetSensorBitViews(mshSensorBitViews);

	mnemoshemaDataManager.AddObserver(httpDataObserver);
}

//---------------------------------------------------------------------------
void __fastcall TSensorsTabSheet::CreateFileDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	timeRangeDataObserver = new TTimeRangeDataObserver(mshViews);
	static_cast<TTimeRangeDataObserver *>(timeRangeDataObserver)->SetSensorBitViews(mshSensorBitViews);

	mnemoshemaDataManager.AddObserver(timeRangeDataObserver);
}

//---------------------------------------------------------------------------
/*
void __fastcall TSensorsTabSheet::CreateTimeAxisObserver(TChartTimeAxis *timeAxis, ITimeAxisEvents *timeAxisHandler) {
	timeAxisObserver = new TTimeAxisObserver(timeAxis, timeAxisHandler);
	timeAxis->AddObserver(timeAxisObserver);
}
*/
//---------------------------------------------------------------------------
std::set<const TSensor *> TSensorsTabSheet::GetSensors() {
	std::set<const TSensor *> sensors;

	for (std::map<const TSensor *, IMnemoshemaView *>::iterator i = mshViews.begin(), iEnd = mshViews.end(); i != iEnd; ++i) {
		const TSensor *sensor = i->first;
		sensors.insert(sensor);
	}

	for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator i = mshSensorBitViews.begin(), iEnd = mshSensorBitViews.end(); i != iEnd; ++i) {
		const TSensorBit *sensorBit = i->first;
		sensors.insert(sensorBit->sensor);
	}

	return sensors;
}

//---------------------------------------------------------------------------
void TSensorsTabSheet::UpdateData(sysObserverable::IObserverable *data) {
	if (data->GetObserverableType() == TObserverableTypes::HISTORY_DATE_TIME) {
		TObserverableHistoryDateTime *observerableHistoryDateTime = static_cast<TObserverableHistoryDateTime *>(data);
		dt1LocalTime = observerableHistoryDateTime->dt1LocalTime;
		dt2LocalTime = observerableHistoryDateTime->dt2LocalTime;
	}
}

//---------------------------------------------------------------------------
void TSensorsTabSheet::OnTimeAxisChanged(double dt1LocalTime, double dt2LocalTime) {
	this->dt1LocalTime = dt1LocalTime;
	this->dt2LocalTime = dt2LocalTime;
}

//---------------------------------------------------------------------------
void TSensorsTabSheet::GetTimeAxisLocalTime(double &dt1LocalTime, double &dt2LocalTime) {
	dt1LocalTime = this->dt1LocalTime;
	dt2LocalTime = this->dt2LocalTime;
}

//---------------------------------------------------------------------------
void TSensorsTabSheet::SetAutoTracking(bool enable) {
	autoTracking = enable;

	for (std::map<const TSensor *, IMnemoshemaView *>::iterator i = mshViews.begin(), iEnd = mshViews.end(); i != iEnd; ++i) {
		IMnemoshemaView *view = i->second;
		TRecordViewChart *sensorViewChart = static_cast<TRecordViewChart *>(view);
		if (sensorViewChart != NULL) {
			sensorViewChart->SetTimeAxisTracking(enable);
			sensorViewChart->SetValueAxisTracking(enable);
		}
	}

	for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator i = mshSensorBitViews.begin(), iEnd = mshSensorBitViews.end(); i != iEnd; ++i) {
		IMnemoshemaView *view = i->second;
		TRecordViewChart *sensorViewChart = static_cast<TRecordViewChart *>(view);
		if (sensorViewChart != NULL) {
			sensorViewChart->SetTimeAxisTracking(enable);
			sensorViewChart->SetValueAxisTracking(enable);
		}
	}
}

//---------------------------------------------------------------------------
void TSensorsTabSheet::DeleteChart(TRecordViewChart *chart) {
	const TRecord *record = chart->GetRecord();
	if (record->record_type == RECORD_TYPE_SENSOR) {
		const TSensor *sensor = static_cast<const TSensor *>(record);
		mshViews.erase(sensor);

	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		const TSensorBit *sensorBit = static_cast<const TSensorBit *>(record);
		mshSensorBitViews.erase(sensorBit);
	}

	//unsubscribe from time axis
	timeAxis->RemoveObserver(chart);

	scrollBox->RemoveComponent(chart);

	delete chart;
}

//---------------------------------------------------------------------------
void  TSensorsTabSheet::DeleteAllCharts() {
	std::list<TRecordViewChart *> recordViewChartsForDelete;

	for (std::map<const TSensor *, IMnemoshemaView *>::iterator i = mshViews.begin(), iEnd = mshViews.end(); i != iEnd; ++i) {
		IMnemoshemaView *view = i->second;
		TRecordViewChart *recordViewChart = static_cast<TRecordViewChart *>(view);
		if (recordViewChart != NULL) {
			recordViewChartsForDelete.push_back(recordViewChart);
		}
	}

	for (std::list<TRecordViewChart *>::iterator i = recordViewChartsForDelete.begin(), iEnd = recordViewChartsForDelete.end(); i != iEnd; ++i) {
		DeleteChart(*i);
	}
}
