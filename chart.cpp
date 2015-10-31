//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "chart.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormChart *FormChart;

//TODO make commend why need to have this object here?
double lastDblClickTime;

/*
class TMnemoshemaTimeRangeObserver : public sysObserver::IObserver {
	public:
		TMnemoshemaDataManager *mnemoshemaDataManager;

		TMnemoshemaTimeRangeObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
			this->mnemoshemaDataManager = &mnemoshemaDataManager;
		}
		virtual ~TMnemoshemaTimeRangeObserver() {}
		virtual void UpdateData(sysObserverable::IObserverable *data) {
			//update the time axis only once when user change the date-time range and click apply
			//the sensors update data is ignored
			TDateTime *dt1GMT, *dt2GMT;
			FormChart->mnemoshemaDataManager->GetTimeRange(&dt1GMT, &dt2GMT);
			if (dt1GMT == NULL || dt2GMT == NULL) {
				return;
			}

			TSensorsTabSheet *sensorsTabSheet = dynamic_cast<TSensorsTabSheet *>(FormChart->PageControl->ActivePage);

			double bias = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;
			double dt1LocalTime = dt1GMT->Val - bias;
			double dt2LocalTime = dt2GMT->Val - bias;

			//sensorsTabSheet->UpdateTimeAxis(dt1LocalTime, dt2LocalTime);
			//sensorsTabSheet->SetTimeAxisBounds(dt1LocalTime, dt2LocalTime);
			sensorsTabSheet->timeAxis->SetTimeAxisBounds(dt1LocalTime, dt2LocalTime);
		}
};
*/

//---------------------------------------------------------------------------
void __fastcall TFormChart::InitializeTimeRange() {
	ComboBoxTimeRange->AddItem("30 минут",  reinterpret_cast<TObject *>(new String("30")));
	ComboBoxTimeRange->AddItem("1 час",     reinterpret_cast<TObject *>(new String("60")));
	ComboBoxTimeRange->AddItem("2 часа",    reinterpret_cast<TObject *>(new String("120")));
	ComboBoxTimeRange->AddItem("4 часа",    reinterpret_cast<TObject *>(new String("240")));
	ComboBoxTimeRange->AddItem("8 часов",   reinterpret_cast<TObject *>(new String("480")));
}

//---------------------------------------------------------------------------
__fastcall TFormChart::TFormChart(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::SaveOpenedPages() {
	try {

		TJSONObject *jsonOpenedPages = new TJSONObject();
		TJSONArray *jsonPages = new TJSONArray();

		for (int page = 0; page < PageControl->PageCount; ++page) {
			TSensorsTabSheet *tabSheet = static_cast<TSensorsTabSheet *>(PageControl->Pages[page]);

			TJSONObject *jsonPage = new TJSONObject();
			jsonPage->AddPair("PageTitle", tabSheet->Caption);

			TJSONArray *jsonUuids = new TJSONArray();

			//iterate over all msh view
			for (std::map<const TSensor *, IMnemoshemaView *>::iterator i = tabSheet->mshViews.begin(), iEnd = tabSheet->mshViews.end(); i != iEnd; ++i) {
				const TSensor *sensor = i->first;
				TRecordViewChart *mshView = static_cast<TRecordViewChart *>(i->second);

				TJSONObject *jsonMshView = new TJSONObject();

				jsonMshView->AddPair("LeftAxisMinimum", mshView->LeftAxis->Minimum);
				jsonMshView->AddPair("LeftAxisMaximum", mshView->LeftAxis->Maximum);

				jsonUuids->Add(sensor->uuid);

				jsonOpenedPages->AddPair(sensor->uuid, jsonMshView);
			}

			//iterate over all msh bit sensot view
			for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator i = tabSheet->mshSensorBitViews.begin(), iEnd = tabSheet->mshSensorBitViews.end(); i != iEnd; ++i) {
				const TSensorBit *sensorBit = i->first;
				TRecordViewChart *mshView = static_cast<TRecordViewChart *>(i->second);

				TJSONObject *jsonMshViewBit = new TJSONObject();

				jsonMshViewBit->AddPair("LeftAxisMinimum", mshView->LeftAxis->Minimum);
				jsonMshViewBit->AddPair("LeftAxisMaximum", mshView->LeftAxis->Maximum);

				jsonUuids->Add(sensorBit->uuid);

				jsonOpenedPages->AddPair(sensorBit->uuid, jsonMshViewBit);
			}

			jsonPage->AddPair("Uuids", jsonUuids);

			jsonPages->Add(jsonPage);
		}

		jsonOpenedPages->AddPair("Pages", jsonPages);

		//save json string into user settings project file
		sysApp::SetSetting(L"ChartPages", jsonOpenedPages);

		delete jsonOpenedPages;

	} catch (Exception &e) {
		sysLogger::ERR_W(e.ToString().c_str());
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::SaveLastUserSettings() {
	//save user settings before close the application
	sysApp::SaveFormSettings(this);

	sysApp::SetSetting(L"ComboBoxTimeRange", (double)ComboBoxTimeRange->ItemIndex);

	sysApp::SetSetting(L"RadioButtonOnline", RadioButtonOnline->Checked);
	sysApp::SetSetting(L"RadioButtonOffline", RadioButtonOffline->Checked);
	sysApp::SetSetting(L"Date1", Date1->Date.Val);
	sysApp::SetSetting(L"Time1", Time1->Time.Val);
	sysApp::SetSetting(L"Date2", Date2->Date.Val);
	sysApp::SetSetting(L"Time2", Time2->Time.Val);

	SaveOpenedPages();
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::RestoreOpenedPages() {
	TRecordViewChart *firstRecordViewChart = NULL;

	try {
		TJSONValue *v = sysApp::GetSetting(L"ChartPages");
		if (v == NULL) {
			return;
		}

		TJSONObject *jsonChartPages = dynamic_cast<TJSONObject *>(v);
		if (jsonChartPages == NULL) {
			return;
		}

		TJSONArray *jsonPages = dynamic_cast<TJSONArray *>(jsonChartPages->GetValue("Pages"));
		if (!jsonPages) {
			delete jsonChartPages;
			return;
		}

		for (int page = 0; page < jsonPages->Count; ++page) {
			TJSONObject *jsonPage = dynamic_cast<TJSONObject *>(jsonPages->Items[page]);

			TSensorsTabSheet *sensorsTabSheet = new TSensorsTabSheet(PageControl, *mnemoshemaDataManager);
			sensorsTabSheet->Caption = jsonPage->Values["PageTitle"]->Value();

			TJSONArray *jsonUuids = dynamic_cast<TJSONArray *>(jsonPage->Values["Uuids"]);
			for (int i = 0; i < jsonUuids->Count; ++i) {
				String uuid = jsonUuids->Items[i]->Value();
				TRecord *record = GetRecord(uuid);
				if (record == NULL) {
					continue;
				}

				TRecordViewChart *recordViewChart = sensorsTabSheet->AddChart(record, activeMouseStrategy);
				if (recordViewChart == NULL) {
					continue;
				}

				if (firstRecordViewChart == NULL) {
                	firstRecordViewChart = recordViewChart;
				}

				TJSONObject *jsonMshView = dynamic_cast<TJSONObject *>(jsonChartPages->Values[uuid]);
				if (jsonMshView == NULL) {
					continue;
				}


				TJSONString *jsonMin = dynamic_cast<TJSONString *>(jsonMshView->Values["LeftAxisMinimum"]);
				TJSONString *jsonMax = dynamic_cast<TJSONString *>(jsonMshView->Values["LeftAxisMaximum"]);

				if (jsonMin != NULL && jsonMax != NULL) {
					double min = StrToFloatDef(jsonMin->Value(), 0);
					double max = StrToFloatDef(jsonMax->Value(), 0);

					if (min != max) {
						recordViewChart->SetValueAxisMinMaxValue(min, max);
						recordViewChart->SetValueAxisTracking(false);
					}
				}
			}
		}

		delete jsonChartPages;

	} catch (Exception &e) {
		sysLogger::ERR_W(e.ToString().c_str());
	}

	//highlight first record view chart in tab
	if (firstRecordViewChart != NULL) {
		firstRecordViewChart->Highlight();
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::RestoreLastUserSettings() {
	sysApp::RestoreFormSettings(this);

	ComboBoxTimeRange->ItemIndex = sysApp::GetSettingInt(L"ComboBoxTimeRange", 0);

	RadioButtonOnline->Checked = sysApp::GetSettingBool(L"RadioButtonOnline", false);
	RadioButtonOffline->Checked = sysApp::GetSettingBool(L"RadioButtonOffline", false);

	Date1->Date = sysApp::GetSettingDouble(L"Date1", Now().Val);
	Time1->Time = sysApp::GetSettingDouble(L"Time1", Now().Val);
	Date2->Date = sysApp::GetSettingDouble(L"Date2", Now().Val);
	Time2->Time = sysApp::GetSettingDouble(L"Time2", Now().Val);
}

//---------------------------------------------------------------------------
/*
void __fastcall TFormChart::CreateHTTPDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	httpDataObserver = new THTTPDataObserver(mnemoshemaDataManager, recordViews);
	mnemoshemaDataManager.AddObserver(httpDataObserver);
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::CreateFileDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager) {
	fileDataObserver = new TFileDataObserver(mnemoshemaDataManager, recordViews);
	mnemoshemaDataManager.AddObserver(fileDataObserver);
}
*/

//---------------------------------------------------------------------------
void __fastcall TFormChart::CreateMouseStrategy() {
	mouseStrategyHand = new TMouseStrategyHand(PageControl, PopupMenuChart);

	mouseStrategyZoom = new TMouseStrategyZoom(PageControl, PopupMenuChart);

	mouseStrategyText = new TMouseStrategyText(PageControl, PopupMenuChart);
	mouseStrategyText->AddObserver(this);

	mouseStrategyVerticalLine = new TMouseStrategyVerticalLine(PageControl, PopupMenuChart);
	mouseStrategyVerticalLine->AddObserver(this);

	mouseStrategySuperVerticalLine = new TMouseStrategySuperVerticalLine(PageControl, PopupMenuChart);
	mouseStrategySuperVerticalLine->AddObserver(this);
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::FormCreate(TObject *Sender) {
	RestoreLastUserSettings();

	CreateMouseStrategy();

	activeMouseStrategy = mouseStrategyHand;

	InitializeTimeRange();

	ComboBoxTimeRange->ItemIndex = 0;

	InitChartFilterButtons();
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::InitChartFilterButtons() {
	CheckBoxGaussianFilter->Checked = GAUSSIAN_FILTER_ENABLED;
	CheckBoxMedianFilter->Checked = MEDIAN_FILTER_ENABLED;
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::CreateDefaultTabSheet(TMnemoshemaDataManager &mnemoshemaDataManager) {
	TSensorsTabSheet *sensorsTabSheet = new TSensorsTabSheet(PageControl, mnemoshemaDataManager);
	sensorsTabSheet->Caption = "Графики";

	//register sensors tab sheet as observer of the time axis
}

//---------------------------------------------------------------------------
TRecordViewChart * __fastcall TFormChart::AddChart(const TRecord *record) {
	TSensorsTabSheet *sensorsTabSheet = dynamic_cast<TSensorsTabSheet *>(PageControl->ActivePage);
	return sensorsTabSheet->AddChart(record, activeMouseStrategy/*, PopupMenuChart*/);
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::AddCharts(const TRecord *record) {
	std::list<TSensor *> sensors;
	if (record->record_type == RECORD_TYPE_DEVICE) {
		sensors = GetSensors(static_cast<const TDevice *>(record));
	}

	for (std::list<TSensor *>::iterator i = sensors.begin(), iEnd = sensors.end(); i != iEnd; ++i) {
		AddChart(*i);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::SetMnemoshemaDataManager(TMnemoshemaDataManager &mnemoshemaDataManager) {
	this->mnemoshemaDataManager = &mnemoshemaDataManager;
	/*
	CreateHTTPDataObserver(mnemoshemaDataManager);
	CreateFileDataObserver(mnemoshemaDataManager);
	*/

	/*
	mnemoshemaTimeRangeObserver = new TMnemoshemaTimeRangeObserver(mnemoshemaDataManager);
	mnemoshemaDataManager.AddObserver(mnemoshemaTimeRangeObserver);
	*/
	mnemoshemaDataManager.AddObserver(this);

	RestoreOpenedPages();

	RequestData();

	if (PageControl->PageCount == 0) {
		CreateDefaultTabSheet(mnemoshemaDataManager);
	}
}

//---------------------------------------------------------------------------
std::set<const TSensor *> __fastcall TFormChart::GetVisibleSensors() {
	TSensorsTabSheet *sensorsTabSheet = dynamic_cast<TSensorsTabSheet *>(PageControl->ActivePage);
	if (sensorsTabSheet != NULL) {
		return sensorsTabSheet->GetSensors();
	}

	return std::set<const TSensor *>();
}

// ---------------------------------------------------------------------------
/*
void __fastcall TFormChart::OnChartMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled) {
	TChart *chart = dynamic_cast<TChart *>(Sender);
	if (!chart){
		return;
	}

	TScrollBox *scrollBox = dynamic_cast<TScrollBox *>(chart->Parent);
	if (!scrollBox){
		return;
	}

	scrollBox->VertScrollBar->Position -= Sign(WheelDelta) * 50;

	Handled = true;
}
*/
// ---------------------------------------------------------------------------
void __fastcall TFormChart::OnChartClick(TObject *Sender) {
	TWinControl *winControl = dynamic_cast<TWinControl *>(Sender);
	if (!winControl){
		return;
	}

	winControl->SetFocus();

	/*
	if (CurrentChart != NULL){
		CurrentChart->UndoHighlight();
	}

	CurrentChart = (TRecordViewChart *)Sender;
	CurrentChart->Highlight();
	*/
}

// ---------------------------------------------------------------------------
// TODO here problem for discrete data display 1 and 0...
void __fastcall TFormChart::OnChartScrollHand(TObject *Sender) {
	/*
	return;

	TRecordViewChart *chart = dynamic_cast<TRecordViewChart *>(Sender);
	if (!chart){
		return;
	}

	if (chart->Zoomed){
		return;
	}

	if (chart->LeftAxis->Minimum > chart->minYScrollValue * 0.9){
		chart->LeftAxis->SetMinMax(chart->minYScrollValue *0.9, chart->maxYScrollValue *1.1);
	} else if (chart->LeftAxis->Maximum < chart->maxYScrollValue * 1.1){
		chart->LeftAxis->SetMinMax(chart->minYScrollValue *0.9, chart->maxYScrollValue *1.1);
	}
	*/
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::OnChartDblClickHand(TObject *Sender) {
	//last dbl click time introduce for prevent dbl click
	lastDblClickTime = Now().Val;

	TSensorsTabSheet *activeTab = static_cast<TSensorsTabSheet *>(PageControl->ActivePage);

	/*
	double fLocalTimeBiasInDays = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;
	if (RadioButtonRealTime->Checked) {
		TDateTime now = Now();

		axisTimeLocalTimeMaxValue = Now().Val;
		axisTimeLocalTimeMinValue = axisTimeLocalTimeMaxValue + floor(DateTimePickerRealTime->DateTime.Val) - DateTimePickerRealTime->DateTime.Val;
	} else if (RadioButtonDateTime->Checked){
		axisTimeLocalTimeMinValue = floor(DateTimePickerD1->DateTime.Val) + DateTimePickerT1->DateTime.Val;
		axisTimeLocalTimeMaxValue = floor(DateTimePickerD2->DateTime.Val) + DateTimePickerT2->DateTime.Val;
	}
	*/

	/*
	TDateTime *dt1GMT, *dt2GMT;
	mnemoshemaDataManager->GetTimeRange(dt1GMT, dt2GMT);

	activeTab->UpdateTimeAxis(axisTimeLocalTimeMinValue, axisTimeLocalTimeMaxValue);
	*/

	//activeTab->followBySignal = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::OnChartMouseDownHand(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	/*
	if (ShapeText->Visible){
		ShapeText->Visible = false;
	}

	mouseZoom.button = Button;
	mouseZoom.point.x = X;
	mouseZoom.point.y = Y;
	mouseZoom.pointClick.x = X;
	mouseZoom.pointClick.y = Y;
	*/
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::OnChartMouseUpHand(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
	//check if event ChartMouseUpHand invoke for dbl click event
	//prevent dbl click event
	if ((Now().Val - lastDblClickTime) < sysTime::SEC2DAY) {
		return;
	}

	/*
	TRecordViewChart *sensorChart = dynamic_cast<TRecordViewChart *>(Sender);
	if (!sensorChart) {
		return;
	}

	TSensorsTabSheet *activePage = dynamic_cast<TSensorsTabSheet *>(PageControlCharts->ActivePage);
	if (!activePage) {
        return;
    }

	if (mouseZoom.button != mbLeft) {
		return;
	}

	//minimum mouse x difference between left button down and left button up
	static const int MOUSE_OFFSET_SENSIVITY = 5;

	if (abs(mouseZoom.point.x - X) < MOUSE_OFFSET_SENSIVITY) {
		return;
	}

	double minXLocalTime = sensorChart->BottomAxis->Minimum;
	double maxXLocalTime = sensorChart->BottomAxis->Maximum;

	//disable follow by signal
	activePage->followBySignal = false;

	//update bottom time axis and update all active axis charts
	SetTimeScaleChartsActiveTab(minXLocalTime, maxXLocalTime);

	mouseZoom.button = mbMiddle;
	*/
}

// ---------------------------------------------------------------------------
void __fastcall TFormChart::OnChartContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled) {
	/*
	if (mouseZoom.button == mbRight){
		if (mouseZoom.pointClick.PtInCircle(MousePos, 2)){
			Handled = false;
			return;
		}
	}
	*/
	Handled = true;
}

// ---------------------------------------------------------------------------
/*
void __fastcall TFormChart::ChangeMouseStrategy(TRecordViewChart *recordViewChart, TChartMouseStrategy mouseStrategy) {
	//TODO move this code into the chart
	//need develop a interface for hand, arrow, zoom e.t.c interactive with implementation of the scroll, dbClik, mouse down e.t.c. function
	//and set mouse strategy with the ptr of such class

	//recordViewChart->vline->Visible = false;
	TChart *chart = recordViewChart->GetChart();

	//chart->OnMouseWheel = OnChartMouseWheel;
	chart->OnClick = OnChartClick;

	if (mouseStrategy == HAND){
		chart->OriginalCursor = crArrow;

		chart->BottomAxis->Automatic = false;
		chart->BottomAxis->AutomaticMinimum = false;
		chart->BottomAxis->AutomaticMaximum = false;

		chart->LeftAxis->Automatic = false;
		chart->LeftAxis->AutomaticMinimum = false;
		chart->LeftAxis->AutomaticMaximum = false;

		chart->Zoom->Allow = false;
		chart->ScrollMouseButton = mbLeft;
		chart->Zoom->Direction = tzdBoth;

		chart->OnScroll = OnChartScrollHand;
		chart->OnDblClick = OnChartDblClickHand;
		chart->OnMouseDown = OnChartMouseDownHand;
		chart->OnMouseUp = OnChartMouseUpHand;
		chart->OnContextPopup = OnChartContextPopup;

	} else if (mouseStrategy == ZOOM){
		recordViewChart->OriginalCursor = crCross;

		recordViewChart->Zoom->Allow = true;
		recordViewChart->Zoom->MouseButton = mbLeft;
		recordViewChart->Zoom->Pen->Color = clBlack;
		recordViewChart->Zoom->Pen->Style = psDash;
		recordViewChart->Zoom->Pen->Width = 2;
		recordViewChart->Zoom->UpLeftZooms = true;
		recordViewChart->Zoom->History = true;
		recordViewChart->Zoom->Animated = false;
		recordViewChart->Zoom->AnimatedSteps = 1;
		recordViewChart->Zoom->Direction = tzdHorizontal;
		recordViewChart->ScrollMouseButton = NULL;

		// recordViewChart->OnScroll = FormGEdit->ChartScrollZoom;
		recordViewChart->OnZoom = FormGEdit->ChartZoom;
		recordViewChart->OnDblClick = FormGEdit->ChartDblClickZoom;

		recordViewChart->OnMouseDown = NULL;
		// recordViewChart->OnMouseUp = FormGEdit->ChartMouseUpZoom;
		recordViewChart->OnMouseUp = NULL;
		recordViewChart->OnMouseMove = NULL;
	} else if (mouseStrategy == TEXT){
		recordViewChart->OriginalCursor = crIBeam;

		recordViewChart->Zoom->Allow = false;

		recordViewChart->OnMouseDown = FormGEdit->ChartMouseDownText;
		recordViewChart->OnMouseUp = FormGEdit->ChartMouseUpText;
		recordViewChart->OnMouseMove = FormGEdit->ChartMouseMoveText;
	} else if (mouseStrategy == VLINE){
		recordViewChart->OriginalCursor = crHSplit;

		recordViewChart->OnMouseMove = FormGEdit->ChartMouseMoveVLine;
		recordViewChart->OnMouseDown = FormGEdit->ChartMouseDownVLine;
	} else if (mouseStrategy == VLINES){
		recordViewChart->OriginalCursor = crHSplit;

		recordViewChart->OnMouseMove = FormGEdit->ChartMouseMoveVLines;
		recordViewChart->OnMouseDown = FormGEdit->ChartMouseDownVLines;
	}
}
*/

// ---------------------------------------------------------------------------
void __fastcall TFormChart::SetChartsMouseStrategy(TMouseStrategy *mouseStrategy, bool disableTracker){
	int pageCount = PageControl->PageCount;
	for (int page = 0; page < pageCount; ++page) {
		TSensorsTabSheet *tab = (TSensorsTabSheet *)PageControl->Pages[page];

		//iterate over sensors mnemoshema view
		for (std::map<const TSensor *, IMnemoshemaView *>::iterator i = tab->mshViews.begin(), iEnd = tab->mshViews.end(); i != iEnd; ++i) {
			TRecordViewChart *recordViewChart = static_cast<TRecordViewChart *>(i->second);
			if (activeMouseStrategy != NULL) {
				activeMouseStrategy->Deactivate(recordViewChart);
			}
			if (disableTracker == true) {
				recordViewChart->chartTracker->SetVisible(false);
			}
			mouseStrategy->Apply(recordViewChart);
		}

		//iterate over sensor bit mnemoshema viewer
		//std::map<const TSensorBit *, IMnemoshemaView *> mshSensorBitViews;
		for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator i = tab->mshSensorBitViews.begin(), iEnd = tab->mshSensorBitViews.end(); i != iEnd; ++i) {
			TRecordViewChart *recordViewChart = static_cast<TRecordViewChart *>(i->second);
			if (activeMouseStrategy != NULL) {
				activeMouseStrategy->Deactivate(recordViewChart);
			}
			if (disableTracker == true) {
				recordViewChart->chartTracker->SetVisible(false);
			}
			mouseStrategy->Apply(recordViewChart);
		}
	}

	activeMouseStrategy = mouseStrategy;
}

// ---------------------------------------------------------------------------
/*
void __fastcall TFormChart::SetChartsMouseStrategy(TChartMouseStrategy strategy){
	int pageCount = PageControl->PageCount;
	for (int page = 0; page < pageCount; ++page){
		TSensorsTabSheet *tab = (TSensorsTabSheet *)PageControl->Pages[page];

		for (std::map<const TRecord *, TRecordView *>::iterator i = tab->recordViews.begin(), iEnd = tab->recordViews.end(); i != iEnd; ++i) {
			TRecordViewChart *recordViewChart = static_cast<TRecordViewChart *>(i->second);
			ChangeMouseStrategy(recordViewChart, strategy);
		}
	}
}
*/

//---------------------------------------------------------------------------
void __fastcall TFormChart::btnHandClick(TObject *Sender) {
	SetChartsMouseStrategy(mouseStrategyHand);
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::btnZoomClick(TObject *Sender) {
	SetChartsMouseStrategy(mouseStrategyZoom, true);
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::btnZoomOutClick(TObject *Sender) {
	btnHand->Down = true;
	SetChartsMouseStrategy(mouseStrategyHand);

	//return time axis as describe in bottom chart setting
	double dt1LocalTime = 0;
	double dt2LocalTime = 0;

	if (RadioButtonOnline->Checked == true) {
		String *sMinutes = reinterpret_cast<String *>(ComboBoxTimeRange->Items->Objects[ComboBoxTimeRange->ItemIndex]);
		int minutes = StrToIntDef(*sMinutes, 30);

		dt2LocalTime = Now().Val;
		dt1LocalTime = dt2LocalTime - sysTime::MIN2DAY * minutes;
	} else if (RadioButtonOffline->Checked == true) {
		dt1LocalTime = floor(Date1->Date.Val) + Time1->Time.Val - floor(Time1->Time.Val);
		dt2LocalTime = floor(Date2->Date.Val) + Time2->Time.Val - floor(Time2->Time.Val);
	}

	if (dt1LocalTime != 0 && dt2LocalTime != 0) {
		TSensorsTabSheet *activePage = static_cast<TSensorsTabSheet *>(PageControl->ActivePage);
		activePage->timeAxis->SetTimeAxisBounds(dt1LocalTime, dt2LocalTime, true);
	}


	/*
	double dt1LocalTime = -1, dt2LocalTime = -1;
	//find min and max date-time value
	for (std::map<const TSensor *, IMnemoshemaView *>::iterator i = activePage->mshViews.begin(), iEnd = activePage->mshViews.end(); i != iEnd; ++i) {
		TSensorViewChart *chart = static_cast<TSensorViewChart *>(i->second);
		TLineSeries *signal = chart->GetSignal();
		if (dt1LocalTime == -1 || dt1LocalTime > signal->MinXValue()) {
			dt1LocalTime = signal->MinXValue();
		}
		if (dt2LocalTime == -1 || dt2LocalTime < signal->MaxXValue()) {
			dt2LocalTime = signal->MaxXValue();
		}
	}
	*/
}
//---------------------------------------------------------------------------

void __fastcall TFormChart::PMSaveAsImageClick(TObject *Sender) {
	if (!PopupMenuChart->PopupComponent){
		return;
	}

	TChart *chart = dynamic_cast<TChart *>(PopupMenuChart->PopupComponent);
	if (!chart){
		return;
	}

	if (!SaveDialog->Execute(NULL)){
		return;
	}

	String fileName = SaveDialog->FileName;

	TSensorsTabSheet *activePage = static_cast<TSensorsTabSheet *>(PageControl->ActivePage);

	Graphics::TBitmap *bm = new Graphics::TBitmap;
	bm->PixelFormat = pf32bit;
	bm->Width  = chart->ClientWidth;
	bm->Height = chart->ClientHeight;
	bm->Height += activePage->timeAxis->GetHeight();

	chart->PaintTo(bm->Canvas->Handle, 0, 0);
	activePage->timeAxis->PaintTo(bm->Canvas->Handle, 0, chart->ClientHeight);

	bm->SaveToFile(fileName);

	delete bm;
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::PMSaveAllAsImageClick(TObject *Sender) {
	if (!PopupMenuChart->PopupComponent){
		return;
	}

	TSensorsTabSheet *activePage = static_cast<TSensorsTabSheet *>(PageControl->ActivePage);
	if (activePage->mshViews.size() == 0) {
    	return;
	}

	if (!SaveDialog->Execute(NULL)){
		return;
	}

	String fileName = SaveDialog->FileName;

	Graphics::TBitmap *bm = new Graphics::TBitmap;
	bm->PixelFormat = pf32bit;
	bm->Width  = activePage->timeAxis->GetWidth();
	bm->Height = 0;

	//calculate the final height of the image
	for (std::map<const TSensor *, IMnemoshemaView *>::iterator i = activePage->mshViews.begin(), iEnd = activePage->mshViews.end(); i != iEnd; ++i) {
		TRecordViewChart *chart = static_cast<TRecordViewChart *>(i->second);
		bm->Height += chart->ClientHeight;
		bm->Height += activePage->timeAxis->GetHeight();
	}

	//build the image
	int heightPosition = 0;
	for (std::map<const TSensor *, IMnemoshemaView *>::iterator i = activePage->mshViews.begin(), iEnd = activePage->mshViews.end(); i != iEnd; ++i) {
		TRecordViewChart *chart = static_cast<TRecordViewChart *>(i->second);
		chart->PaintTo(bm->Canvas->Handle, 0, heightPosition);
		heightPosition += chart->Height;

		activePage->timeAxis->PaintTo(bm->Canvas->Handle, 0, heightPosition);
		heightPosition += activePage->timeAxis->GetHeight();
	}

	bm->SaveToFile(fileName);

	delete bm;
}
//---------------------------------------------------------------------------
/*
void TFormChart::OnTimeAxisChanged(double dt1LocalTime, double dt2LocalTime) {
	TDateTime dt;
	dt.Val = dt1LocalTime;
	String s1 = dt.FormatString("YYYY.mm.dd hh:nn:ss");
	dt.Val = dt2LocalTime;
	String s2 = dt.FormatString("YYYY.mm.dd hh:nn:ss");
	StatusBar->Panels->Items[1]->Text = "с " + s1 + " по " + s2;
}
*/
//---------------------------------------------------------------------------

void TFormChart::UpdateData(sysObserverable::IObserverable *data) {
	if (data == NULL) {
		return;
	}

	if (data->GetObserverableType() == TObserverableTypes::HISTORY_DATE_TIME) {
		TSensorsTabSheet *sensorsTabSheet = static_cast<TSensorsTabSheet *>(PageControl->ActivePage);
		TObserverableHistoryDateTime *historyDateTime = static_cast<TObserverableHistoryDateTime *>(data);

		sensorsTabSheet->timeAxis->SetTimeAxisBounds(historyDateTime->dt1LocalTime, historyDateTime->dt2LocalTime);
	} else if (data->GetObserverableType() == TObserverableTypes::NEW_VERTICAL_LINE) {
		SetChartsMouseStrategy(mouseStrategyHand);
		btnHand->Down = true;

	} else if (data->GetObserverableType() == TObserverableTypes::NEW_SUPER_VERTICAL_LINE) {
		SetChartsMouseStrategy(mouseStrategyHand);
		btnHand->Down = true;

	} else if (data->GetObserverableType() == TObserverableTypes::NEW_TEXT_MARKER) {
		SetChartsMouseStrategy(mouseStrategyHand);
		btnHand->Down = true;

	} else if (data->GetObserverableType() == TObserverableTypes::TIME_AXIS_CHANGE_BY_USER) {
		//TSensorsTabSheet *sensorsTabSheet = static_cast<TSensorsTabSheet *>(PageControl->ActivePage);
		TObserverableChangeTimeAxisByUser *changeTimeAxisByUser = static_cast<TObserverableChangeTimeAxisByUser *>(data);

		std::set<const TSensor *> sensors = GetVisibleSensors();
		if (sensors.size() == 0) {
			return;
		}

		//sensorsTabSheet->timeAxis->SetTimeAxisBounds(historyDateTime->dt1LocalTime, historyDateTime->dt2LocalTime);
		RequestDataByTimeRange(sensors, changeTimeAxisByUser->GetDateTime1AsGMT(), changeTimeAxisByUser->GetDateTime2AsGMT());
	}
}

//---------------------------------------------------------------------------
void TFormChart::ChangeTimeAxisScale(double coefficient) {
	double dt1LocalTime, dt2LocalTime;

	TSensorsTabSheet *activePage = static_cast<TSensorsTabSheet *>(PageControl->ActivePage);
	activePage->timeAxis->GetTimeAxisBounds(dt1LocalTime, dt2LocalTime);

	double newTimeRange = coefficient * (dt2LocalTime - dt1LocalTime);
	double middleTime = 0.5 * (dt1LocalTime + dt2LocalTime);
	dt1LocalTime = middleTime - 0.5 * newTimeRange;
	dt2LocalTime = middleTime + 0.5 * newTimeRange;

	activePage->timeAxis->SetTimeAxisBounds(dt1LocalTime, dt2LocalTime);
}

//---------------------------------------------------------------------------
void TFormChart::ChangeSignalAxisScale(double coefficient) {
	TSensorsTabSheet *activePage = static_cast<TSensorsTabSheet *>(PageControl->ActivePage);
	for (std::map<const TSensor *, IMnemoshemaView *>::iterator i = activePage->mshViews.begin(), iEnd = activePage->mshViews.end(); i != iEnd; ++i) {
		IMnemoshemaView *mshView = i->second;
		if (mshView->IsHighlighted() == true) {
			TRecordViewChart *recordViewChart = static_cast<TRecordViewChart *>(mshView->GetControl());
			recordViewChart->ChangeMinMaxLeftAxis(coefficient);
			recordViewChart->SetValueAxisTracking(false);
		}

		//TODO update interface function for IMnemoshemaView for change the y scale
		//TRecordViewChart *recordViewChart = static_cast<TRecordViewChart *>(i->second);
		/*
		double newSignalRange = coefficient * (recordViewChart->LeftAxis->Maximum - recordViewChart->LeftAxis->Minimum);
		double middleSignal = 0.5 * (recordViewChart->LeftAxis->Maximum + recordViewChart->LeftAxis->Minimum);
		recordViewChart->LeftAxis->SetMinMax(middleSignal - 0.5 * newSignalRange, middleSignal + 0.5 * newSignalRange);
		*/
		//recordViewChart->ChangeMinMaxLeftAxis(coefficient);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::btnScaleXIncClick(TObject *Sender) {
	SetChartsMouseStrategy(mouseStrategyHand);
	ChangeTimeAxisScale(0.5);
	btnHand->Down = true;
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::btnScaleXDecClick(TObject *Sender) {
	SetChartsMouseStrategy(mouseStrategyHand);
	ChangeTimeAxisScale(2.0);
	btnHand->Down = true;
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::btnScaleYIncClick(TObject *Sender) {
	SetChartsMouseStrategy(mouseStrategyHand);
	ChangeSignalAxisScale(0.5);
	btnHand->Down = true;
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::btnScaleYDecClick(TObject *Sender) {
	SetChartsMouseStrategy(mouseStrategyHand);
	ChangeSignalAxisScale(2.0);
	btnHand->Down = true;
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::PMNewPageClick(TObject *Sender) {
	if (mnemoshemaDataManager == NULL) {
		return;
	}

	String pageTitle = "";
	if (InputQuery(L"Введите имя закладки", L"Имя закладки:", pageTitle)) {
		TSensorsTabSheet *sensorsTabSheet = new TSensorsTabSheet(PageControl, *mnemoshemaDataManager);
		sensorsTabSheet->Caption = pageTitle;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::PMChangePageTitleClick(TObject *Sender) {
	String pageTitle = PageControl->ActivePage->Caption;
	if (InputQuery(L"Введите имя закладки", L"Имя закладки:", pageTitle)) {
		PageControl->ActivePage->Caption = pageTitle;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::FormCloseQuery(TObject *Sender, bool &CanClose) {
	WindowState = wsMinimized;
	CanClose = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormChart::PMDeleteChartClick(TObject *Sender) {
	TSensorsTabSheet *sensorsTabSheet = dynamic_cast<TSensorsTabSheet *>(PageControl->ActivePage);
	sensorsTabSheet->DeleteChart(static_cast<TRecordViewChart *>(PopupMenuChart->PopupComponent));
}
//---------------------------------------------------------------------------

void __fastcall TFormChart::PMDeletePageClick(TObject *Sender) {
	TSensorsTabSheet *sensorsTabSheet = dynamic_cast<TSensorsTabSheet *>(PageControl->ActivePage);
	sensorsTabSheet->DeleteAllCharts();

	PageControl->RemoveComponent(sensorsTabSheet);

	delete sensorsTabSheet;

	if (PageControl->PageCount == 0) {
		//insert default tab if remove the last one
		TSensorsTabSheet *sensorsTabSheet = new TSensorsTabSheet(PageControl, *mnemoshemaDataManager);
		sensorsTabSheet->Caption = "График";
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::RadioButtonOnlineClick(TObject *Sender) {
	RequestData();
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::RadioButtonOfflineClick(TObject *Sender) {
	RequestData();
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::RequestData() {
	static double fLocalTimeBiasInDays = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;

	std::set<const TSensor *> sensors = GetVisibleSensors();
	if (sensors.size() == 0) {
		return;
	}

	double dt1GMT = 0;
	double dt2GMT = 0;

	if (RadioButtonOnline->Checked == true) {
		String *sMinutes = reinterpret_cast<String *>(ComboBoxTimeRange->Items->Objects[ComboBoxTimeRange->ItemIndex]);
		int minutes = StrToIntDef(*sMinutes, 30);

		dt2GMT = fLocalTimeBiasInDays + Now().Val;
		dt1GMT = dt2GMT - sysTime::MIN2DAY * minutes;
	} else if (RadioButtonOffline->Checked == true) {
		dt1GMT = fLocalTimeBiasInDays + floor(Date1->Date.Val) + Time1->Time.Val - floor(Time1->Time.Val);
		dt2GMT = fLocalTimeBiasInDays + floor(Date2->Date.Val) + Time2->Time.Val - floor(Time2->Time.Val);
	}

	RequestDataByTimeRange(sensors, dt1GMT, dt2GMT);
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::RequestDataByTimeRange(std::set<const TSensor *> &sensors, double dt1GMT, double dt2GMT) {
	if (sensors.size() == 0) {
		return;
	}

	static double fLocalTimeBiasInDays = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;

	if (dt1GMT != 0 && dt2GMT != 0) {
		TSensorsTabSheet *sensorsTabSheet = dynamic_cast<TSensorsTabSheet *>(PageControl->ActivePage);
		int timeAxisWidth = sensorsTabSheet->timeAxis->GetWidth();
		double resolution = (dt2GMT - dt1GMT) / (double) timeAxisWidth;

		REQUEST_TIME_RANGE_DATA(sensors, dt1GMT, dt2GMT, resolution);

		TObserverableHistoryDateTime observerableHistorydateTime(dt1GMT - fLocalTimeBiasInDays, dt2GMT - fLocalTimeBiasInDays);
		mnemoshemaDataManager->NotifyObservers(&observerableHistorydateTime);
	}

}

//---------------------------------------------------------------------------
void __fastcall TFormChart::ComboBoxTimeRangeChange(TObject *Sender) {
	RequestData();
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::ButtonRequestDataClick(TObject *Sender) {
	RequestData();
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::PMTrackClick(TObject *Sender) {
	TSensorsTabSheet *sensorsTabSheet = dynamic_cast<TSensorsTabSheet *>(PageControl->ActivePage);

	for (std::map<const TSensor *, IMnemoshemaView *>::iterator itView = sensorsTabSheet->mshViews.begin(), itEnd = sensorsTabSheet->mshViews.end(); itView != itEnd; ++itView) {
		IMnemoshemaView *view = itView->second;
		TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(view->GetControl());
		if (recordViewChart != NULL) {
			recordViewChart->chartTracker->SetVisible(!recordViewChart->chartTracker->IsVisible());
		}
	}

	for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator itView = sensorsTabSheet->mshSensorBitViews.begin(), itEnd = sensorsTabSheet->mshSensorBitViews.end(); itView != itEnd; ++itView) {
		IMnemoshemaView *view = itView->second;
		TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(view->GetControl());
		if (recordViewChart != NULL) {
			recordViewChart->chartTracker->SetVisible(!recordViewChart->chartTracker->IsVisible());
		}
	}

	PMTrack->Checked = !PMTrack->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::btnTextClick(TObject *Sender) {
	SetChartsMouseStrategy(mouseStrategyText);
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::PMDeleteClick(TObject *Sender) {
	/*
	TChartText *chartText = dynamic_cast<TChartText *>(PopupMenuChartText->PopupComponent);
	if (chartText != NULL) {
		TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(chartText->Parent);
		recordViewChart->RemoveTrackableElement(chartText);
		delete chartText;
	}
	*/
}

//---------------------------------------------------------------------------

void __fastcall TFormChart::btnVLineClick(TObject *Sender) {
	SetChartsMouseStrategy(mouseStrategyVerticalLine);
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::PMDeleteVerticalLineClick(TObject *Sender)
{
;//TODO delete vertical line
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::btnVLinesClick(TObject *Sender) {
	SetChartsMouseStrategy(mouseStrategySuperVerticalLine);
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::FormDestroy(TObject *Sender) {
	SaveLastUserSettings();
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::UpdateCharts() {
	if (PageControl->PageCount == 0) {
    	return;
	}

	TSensorsTabSheet *sensorsTabSheet = dynamic_cast<TSensorsTabSheet *>(PageControl->ActivePage);
	if (sensorsTabSheet == NULL) {
    	return;
	}

	for (std::map<const TSensor *, IMnemoshemaView *>::iterator itView = sensorsTabSheet->mshViews.begin(), itEnd = sensorsTabSheet->mshViews.end(); itView != itEnd; ++itView) {
		IMnemoshemaView *view = itView->second;
		if (view == NULL) {
			continue;
		}

		view->UpdateValue();
	}

	for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator itView = sensorsTabSheet->mshSensorBitViews.begin(), itEnd = sensorsTabSheet->mshSensorBitViews.end(); itView != itEnd; ++itView) {
		IMnemoshemaView *view = itView->second;
		if (view == NULL) {
			continue;
		}

		view->UpdateValue();
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::CheckBoxMedianFilterClick(TObject *Sender) {
	MEDIAN_FILTER_ENABLED = CheckBoxMedianFilter->Checked;

	UpdateCharts();
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::CheckBoxGaussianFilterClick(TObject *Sender) {
	GAUSSIAN_FILTER_ENABLED = CheckBoxGaussianFilter->Checked;

	UpdateCharts();
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::PMDeleteMarkerClick(TObject *Sender) {
	if (activeMouseStrategy->mouseInfo.trackable == NULL) {
		return;
	}

	TRecordViewChart *recordViewChart = dynamic_cast<TRecordViewChart *>(PopupMenuChart->PopupComponent);
	if (recordViewChart == NULL) {
		return;
	}

	TChartTrackable *trackableElement = activeMouseStrategy->mouseInfo.trackable;
	if (trackableElement->GetType() == trackable::VERTICAL_LINE) {
		TChartVerticalLine *currentVerticalLine = static_cast<TChartVerticalLine *>(trackableElement);
		if (currentVerticalLine->IsSuperline()) {
			//remove all superline with the same local time

			TSensorsTabSheet *tabSheet;
			if ((tabSheet = dynamic_cast<TSensorsTabSheet *>(PageControl->ActivePage)) != NULL) {
				TChartVerticalLine *verticalLine;
				for (std::map<const TSensor *, IMnemoshemaView *>::iterator it = tabSheet->mshViews.begin(); it != tabSheet->mshViews.end(); ++it) {
					IMnemoshemaView *mshView = it->second;
					TRecordViewChart *viewChart;
					if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
						//find vertical line with the same time
						std::list<TChartTrackable *> *trackableElements = viewChart->GetTrackableElements();

						for (std::list<TChartTrackable *>::iterator t = trackableElements->begin(); t != trackableElements->end(); ++t) {
							if ((*t)->GetType() != trackable::TTrackableType::VERTICAL_LINE) {
								continue;
							}

							verticalLine = static_cast<TChartVerticalLine *>(*t);
							if (verticalLine == currentVerticalLine) {
								continue;
							}

							if (fabs(verticalLine->GetLocalTime() - currentVerticalLine->GetLocalTime()) <= sysTime::SEC2DAY) {
								++t;
								viewChart->RemoveTrackableElement(verticalLine);
								delete verticalLine;
							}
						}
					}
				}

				for (std::map<const TSensorBit *, IMnemoshemaView *>::iterator it = tabSheet->mshSensorBitViews.begin(); it != tabSheet->mshSensorBitViews.end(); ++it) {
					IMnemoshemaView *mshView = it->second;
					TRecordViewChart *viewChart;
					if ((viewChart = dynamic_cast<TRecordViewChart *>(mshView->GetControl())) != NULL) {
						//find vertical line with the same time
						std::list<TChartTrackable *> *trackableElements = viewChart->GetTrackableElements();

						for (std::list<TChartTrackable *>::iterator t = trackableElements->begin(); t != trackableElements->end(); ++t) {
							if ((*t)->GetType() != trackable::TTrackableType::VERTICAL_LINE) {
								continue;
							}

							verticalLine = static_cast<TChartVerticalLine *>(*t);
							if (verticalLine == currentVerticalLine) {
								continue;
							}

							if (fabs(verticalLine->GetLocalTime() - currentVerticalLine->GetLocalTime()) <= sysTime::SEC2DAY) {
								++t;
								viewChart->RemoveTrackableElement(verticalLine);
								delete verticalLine;
							}
						}
					}
				}
			}
		}
	}

	recordViewChart->RemoveTrackableElement(trackableElement);
	delete activeMouseStrategy->mouseInfo.trackable;
	activeMouseStrategy->mouseInfo.trackable = NULL;
}

//---------------------------------------------------------------------------
void __fastcall TFormChart::PopupMenuChartPopup(TObject *Sender) {
	PMDeleteMarker->Enabled = activeMouseStrategy->mouseInfo.trackable != NULL;
}
//---------------------------------------------------------------------------

