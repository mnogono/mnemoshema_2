//---------------------------------------------------------------------------


#pragma hdrstop

#include "RecordViewChart.h"

//---------------------------------------------------------------------------
__fastcall TLabelBitValue::TLabelBitValue(Classes::TComponent *AOwner) : TLabel(AOwner){
}

//---------------------------------------------------------------------------
TLabelBitValue * TLabelBitValue::Create(TWinControl *AOwner, double startTime, double endTime, String text, int val){
	TLabelBitValue *labelBitValue = new TLabelBitValue(AOwner);
	labelBitValue->Parent = AOwner;
	labelBitValue->startTime = startTime;
	labelBitValue->endTime = endTime;
	labelBitValue->Text = "";
	labelBitValue->val = val;
	labelBitValue->Height = AOwner->Height;
	//labelBitValue->Color = clYellow;
	labelBitValue->Hint = text;
	labelBitValue->ShowHint = true;
	labelBitValue->Top = 0;
	labelBitValue->Transparent = true;
  

	/*
	labelBitValue->OnMouseDown = ((TChart *)AOwner)->OnMouseDown;
	labelBitValue->OnMouseUp = ((TChart *)AOwner)->OnMouseUp;
	labelBitValue->OnMouseMove = ((TChart *)AOwner)->OnMouseMove;
	*/

	return labelBitValue;
}

//---------------------------------------------------------------------------
/*
int CHART_SERIES_DATA_INDEX = 0;
int CHART_SERIES_ALARM_MIN_INDEX = 1;
int CHART_SERIES_ALARM_MAX_INDEX = 2;
int CHART_SERIES_VIRTUAL_ALARM_MIN_INDEX = 3;
int CHART_SERIES_VIRTUAL_ALARM_MAX_INDEX = 4;
*/

//---------------------------------------------------------------------------
inline __fastcall TRecordViewChart::~TRecordViewChart(void){
//	delete progressBar;
};

// ---------------------------------------------------------------------------
void __fastcall TRecordViewChart::CreateProgressBar(){
/*
	progressBar = new TButton(this);
	progressBar->Parent = this;

	progressBar->Visible = false;
*/
	isProgressBarVisible = false;
}

// ---------------------------------------------------------------------------
__fastcall TRecordViewChart::TRecordViewChart(Classes::TComponent *AOwner, TRecord *record, TUser *user, void (*SetChartSeriesSetings)(TLineSeries *, TUser *, String)):
	TChart(AOwner)
	,TActiveAlarmObserver() {

    progressBarStep = 0;

	this->maxYScrollValue = 0;
	this->minYScrollValue = 0;

	this->SetChartSeriesSetings = SetChartSeriesSetings;

	this->user = user;

	this->record = record;

	if (record->record_type == RECORD_TYPE_SENSOR) {
		this->sensor = static_cast<TSensor *>(record);
	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		this->sensor = user->GetSensor(static_cast<TSensorBit *>(record)->sensor_id);
	}

	//followBySignal = false;
	enableTrack = false;

	this->View3D = false;
	this->Legend->Visible = false;

	this->Title->Alignment = taLeftJustify;
	this->Title->VertMargin = 0;
	this->Title->Left = 75;
	this->Title->Top = 0;
	this->Title->Font->Color = clBlack;

	this->MarginUnits = TTeeUnits::muPixels;// 1;
	this->MarginLeft = 0;
	this->MarginRight = 5;
	this->MarginTop = 0;
	this->MarginBottom = -22;

	this->BevelInner = TBevelCut::bvNone;// 0;
	this->BevelOuter = TBevelCut::bvNone;//0;

	this->Border->Visible = false;

	this->LeftAxis->LabelsSize = 50;
	this->LeftAxis->Axis->Color = clBlack;
	this->LeftAxis->EndPosition = 97;

	//this->RightAxis->Axis->Color = clWhite;

	/*
	this->MarginUnits = 1;
	this->MarginLeft = 0;
	this->MarginRight = 20;
	*/
	this->Parent = (TWinControl *)AOwner;
	this->Align = alTop;
	/*
	this->View3D = false;
	this->Legend->Visible = false;
	*/
	//this->BottomAxis->GridCentered = false;
	//this->BottomAxis->Grid->DrawEvery = 1;
	this->BottomAxis->DateTimeFormat = "hh:nn:ss";
	this->BottomAxis->LabelsMultiLine = true;
	this->BottomAxis->Visible = true;
	this->BottomAxis->Grid->Visible = true;
	this->BottomAxis->Axis->Color = clWhite;

	this->Walls->Visible = true;
	this->Walls->Size = 3;
	//this->Walls->Bottom->Color = (TColor)RGB(155, 155, 155);
	//this->Walls->Right->Color = (TColor)RGB(155, 155, 155);

	TScrollBox *scrollBox = dynamic_cast<TScrollBox *>(AOwner);
	scrollBox->VertScrollBar->Increment = 20;
	/*
	if (scrollBox) {
		this->OnMouseWheel = scrollBox->OnMouseWheel;
	}
	*/
	//TMouseWheelEvent
	//typedef void __fastcall (__closure *TMouseWheelEvent)(System::TObject* Sender, Classes::TShiftState Shift, int WheelDelta, const Types::TPoint &MousePos, bool &Handled);
	this->TabStop = true;
	//this->OnGetAxisLabel = FormGEdit->SensorChartGetAxisLabel;

	//data series CHART_SERIES_DATA_INDEX = 0;
	//int iLineWidth = StrToInt(EmptyTo(GetUserSetting(user->id, "chart_line_width"), "2"));

	series = new TLineSeries(this);
	//series->Pen->Width = iLineWidth;
	series->XValues->DateTime = true;
	this->AddSeries(series);


	if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
	    SetChartSeriesSetings(series, user, "line_bit");
    } else {
	    SetChartSeriesSetings(series, user, "line");
    }

	//min allow signal series CHART_SERIES_ALARM_MIN_INDEX = 1
	min_series = new TLineSeries(this);
	//min_series->Pen->Width=2;
	//min_series->Pen->Color = clGreen;
	min_series->LinePen->Style = psDash;
	if (record->record_type == RECORD_TYPE_SENSOR) {
		TColor color = (TColor)StrToInt(EmptyTo(GetUserSetting(user->id, "chart_line_mark_color"), "0"));
		min_series->AddXY(0, sensor->min_value * sensor->powerValue, "", color);
		min_series->AddXY(0, sensor->min_value * sensor->powerValue, "", color);

	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		TColor color = (TColor)StrToInt(EmptyTo(GetUserSetting(user->id, "chart_line_mark_color"), "0"));
		min_series->AddXY(0, sensor->min_value, "", color);
		min_series->AddXY(0, sensor->min_value, "", color);
	}
	if (sensor->min_value == NULL_VALUE) {
		min_series->Visible = false;
	}
	SetChartSeriesSetings(min_series, user, "line_mark");
	this->AddSeries(min_series);

	//max allow signal series CHART_SERIES_ALARM_MAX_INDEX = 2
	max_series = new TLineSeries(this);
	//max_series->Pen->Width=2;
	//max_series->Pen->Color = clGreen;
	max_series->LinePen->Style = psDash;
	if (record->record_type == RECORD_TYPE_SENSOR) {
		TColor color = (TColor)StrToInt(EmptyTo(GetUserSetting(user->id, "chart_line_mark_color"), "0"));
		max_series->AddXY(0, sensor->max_value * sensor->powerValue, "", color);
		max_series->AddXY(0, sensor->max_value * sensor->powerValue, "", color);

	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		TColor color = (TColor)StrToInt(EmptyTo(GetUserSetting(user->id, "chart_line_mark_color"), "0"));
		max_series->AddXY(0, sensor->max_value, "", color);
		max_series->AddXY(0, sensor->max_value, "", color);
	}
	if (sensor->min_value == NULL_VALUE) {
		min_series->Visible = false;
	}
	SetChartSeriesSetings(max_series, user, "line_mark");
	this->AddSeries(max_series);

	//virtual min allow signal series CHART_SERIES_VIRTUAL_ALARM_MIN_INDEX = 3
	vmin_series = new TLineSeries(this);
	vmin_series->Visible = false;
	//vmin_series->Pen->Width=1;
	//vmin_series->Pen->Color = clBlue;
	if (record->record_type == RECORD_TYPE_SENSOR) {
		double vmin = StrToFloat(EmptyTo(GetUserSetting(user->id, sensor->uuid + "_min"), IntToStr(NULL_VALUE)));
		TColor color = (TColor)StrToInt(EmptyTo(GetUserSetting(user->id, "chart_line_virt_mark_color"), "0"));
		vmin_series->AddXY(0, vmin * sensor->powerValue, "", color);
		vmin_series->AddXY(0, vmin * sensor->powerValue, "", color);
		bool displayVirtualLimits = StrToBool(EmptyTo(GetUserSetting(user->id, sensor->uuid + "_displayVirtualLimits"), "0"));
		vmin_series->Visible = displayVirtualLimits;

	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		double vmin = StrToFloat(EmptyTo(GetUserSetting(user->id, sensor->uuid + "_min"), IntToStr(NULL_VALUE)));
		TColor color = (TColor)StrToInt(EmptyTo(GetUserSetting(user->id, "chart_line_virt_mark_color"), "0"));
		vmin_series->AddXY(0, vmin, "", color);
		vmin_series->AddXY(0, vmin, "", color);
		bool displayVirtualLimits = StrToBool(EmptyTo(GetUserSetting(user->id, sensor->uuid + "_displayVirtualLimits"), "0"));
		vmin_series->Visible = displayVirtualLimits;
	}
	SetChartSeriesSetings(vmin_series, user, "line_virt_mark");
	this->AddSeries(vmin_series);

	//virtual max allow signal series CHART_SERIES_VIRTUAL_ALARM_MAX_INDEX = 4
	vmax_series = new TLineSeries(this);
	vmax_series->Visible = false;
	vmax_series->Pen->Width=1;
	vmax_series->Pen->Color = clBlue;
	if (record->record_type == RECORD_TYPE_SENSOR) {
		double vmax = StrToFloat(EmptyTo(GetUserSetting(user->id, sensor->uuid + "_max"), IntToStr(NULL_VALUE)));
		TColor color = (TColor)StrToInt(EmptyTo(GetUserSetting(user->id, "chart_line_virt_mark_color"), "0"));
		vmax_series->AddXY(0, vmax * sensor->powerValue, "", color);
		vmax_series->AddXY(0, vmax * sensor->powerValue, "", color);
		bool displayVirtualLimits = StrToBool(EmptyTo(GetUserSetting(user->id, sensor->uuid + "_displayVirtualLimits"), "0"));
		vmax_series->Visible = displayVirtualLimits;

	} else if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		double vmax = StrToFloat(EmptyTo(GetUserSetting(user->id, sensor->uuid + "_max"), IntToStr(NULL_VALUE)));
		TColor color = (TColor)StrToInt(EmptyTo(GetUserSetting(user->id, "chart_line_virt_mark_color"), "0"));
		vmax_series->AddXY(0, vmax, "", color);
		vmax_series->AddXY(0, vmax, "", color);
		bool displayVirtualLimits = StrToBool(EmptyTo(GetUserSetting(user->id, sensor->uuid + "_displayVirtualLimits"), "0"));
		vmax_series->Visible = displayVirtualLimits;
	}
	SetChartSeriesSetings(vmax_series, user, "line_virt_mark");
	this->AddSeries(vmax_series);

	//this->PopupMenu = PopupMenuChart;
	this->track = new TLabel(this);
	this->track->Parent = this;
	this->track->Visible = false;
	this->track->Font->Size = 10;
	this->track->Font->Style = TFontStyles() << fsBold;

	this->trackMarker = new TShape(this);
	this->trackMarker->Parent = this;

	this->SetTrekSettings(user);

	this->trackMarker->Height = this->Height;
	this->trackMarker->Visible = false;

	this->vline = new TLabel(this);
	this->vline->Parent = this;
	this->vline->Visible = false;
	this->vline->Font->Size = 8;

	//this->SetMouseStrategy(mouseStrategy);
}

// ---------------------------------------------------------------------------
void TRecordViewChart::SetUser(TUser *user){
	this->user = user;
}

// ---------------------------------------------------------------------------
void TRecordViewChart::SetTrekSettings(TUser *User){
	String chartTrackWidth = EmptyTo(GetUserSetting(User->id, "chart_track_width"), "1");
	trackMarker->Width = StrToInt(chartTrackWidth);

	String chartTrackColor = EmptyTo(GetUserSetting(User->id, "chart_track_color"), "0");
	trackMarker->Pen->Color = (TColor)StrToInt(chartTrackColor);
	trackMarker->Brush->Color = (TColor)StrToInt(chartTrackColor);

	/*
	String settingsVal = "";
	settingsVal = User->GetViewSettings("settings", "chart_track_width");
	if (settingsVal=="") settingsVal = "1";
	this->trackMarker->Width = StrToInt(settingsVal);

	settingsVal = User->GetViewSettings("settings", "chart_track_color");
	if (settingsVal=="") settingsVal = "0";
	this->trackMarker->Pen->Color = StrToInt(settingsVal);
	this->trackMarker->Brush->Color = StrToInt(settingsVal);
	*/
}

// ---------------------------------------------------------------------------
void TRecordViewChart::DisplayData(std::vector<TData *> *data, bool bClearSeries){
	if (!sensor) throw TExceptionRecordViewChartDoesNotHaveSensor();

	if (!data) throw TExceptionRecordViewChartDataIsNull(sensor->name);

	if (!this->SeriesCount()) throw TExceptionRecordViewChartDoesNotHaveSeries(sensor->name);

	if (!series || !max_series || !min_series) throw TExceptionRecordViewChartDoesNotHaveRequiredSeries(sensor->name);
	/*
	iMillisecondsGMT1 = ConvertToMilliseconds(dtGMT1);
	iMillisecondsGMT2 = ConvertToMilliseconds(dtGMT2);
	iSecondsGMT1 = iMillisecondsGMT1 / 1000;
	iSecondsGMT2 = iMillisecondsGMT2 / 1000;
	*/
	if (bClearSeries) {
		series->Clear();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRecordViewChart::RestoreChartZoom(double fGMT1, double fGMT2){
	static double bias = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;// / 86400.0;
	//double XMin = threadEvent->dtGMT1.Val - bias;
	//double XMax = threadEvent->dtGMT2.Val - bias;
	//double XMin = dtGMT1->Val - bias;
	//double XMax = dtGMT2->Val - bias;
	double XMin = fGMT1 - bias;
	double XMax = fGMT2 - bias;

	BottomAxis->SetMinMax(XMin, XMax);

	//followBySignal = true;
	Zoomed = false;

	//уставки
	SetLimits(XMin, XMax, user);

	double dMinValue = series->GetYValueList("")->MinValue * 0.9;
	double dMaxValue = series->GetYValueList("")->MaxValue * 1.1;

    //change min and max value scale for discrete type sensor
    if (record->record_type == RECORD_TYPE_SENSOR_BIT) {
		dMinValue = 0.0;
        dMaxValue = 1.1;
    }

	LeftAxis->SetMinMax(dMinValue, dMaxValue);

	UpdateChartControls();
}

// ---------------------------------------------------------------------------
void TRecordViewChart::SetLimits(double xStart, double xEnd, TUser *User){
	if (max_series->Visible) {
		max_series->XValues->Value[0] = xStart;
		max_series->XValues->Value[1] = xEnd;
	}
	if (min_series->Visible) {
		min_series->XValues->Value[0] = xStart;
		min_series->XValues->Value[1] = xEnd;
	}
	if (vmin_series->Visible) {
		vmin_series->XValues->Value[0] = xStart;
		vmin_series->XValues->Value[1] = xEnd;
	}
	if (vmax_series->Visible) {
		vmax_series->XValues->Value[0] = xStart;
		vmax_series->XValues->Value[1] = xEnd;
	}

	return;
	//if (this->SeriesCount()>=5) {
	//	min_series = (TLineSeries *)this->Series[CHART_SERIES_ALARM_MIN_INDEX];
	//	max_series = (TLineSeries *)this->Series[CHART_SERIES_ALARM_MAX_INDEX];
	//	vmin_series = (TLineSeries *)this->Series[CHART_SERIES_VIRTUAL_ALARM_MIN_INDEX];
	//	vmax_series = (TLineSeries *)this->Series[CHART_SERIES_VIRTUAL_ALARM_MAX_INDEX];
	//} else return;


	//уставки
	max_series->Clear();
	if (sensor->max_value != NULL_VALUE) {
		max_series->AddXY(xStart, sensor->max_value);
		max_series->AddXY(xEnd, sensor->max_value);
	}

	min_series->Clear();
	if (sensor->min_value != NULL_VALUE) {
		min_series->AddXY(xStart, sensor->min_value);
		min_series->AddXY(xEnd, sensor->min_value);
	}

	//виртуальные уставки
	//String settingsVal = User->GetViewSettings("sensors", sensor->uuid+"_max");
	//if (settingsVal=="") settingsVal = "0";
	//double vmax = StrToFloat(settingsVal);
	double vmax = StrToFloat(EmptyTo(GetUserSetting(User->id, sensor->uuid + "_max"), IntToStr(NULL_VALUE)));
	vmax_series->Clear();
	if (vmax != NULL_VALUE) {
		vmax_series->AddXY(xStart, vmax);
		vmax_series->AddXY(xEnd, vmax);
	}

	//settingsVal = User->GetViewSettings("sensors", sensor->uuid+"_min");
	//if (settingsVal=="") settingsVal = "0";
	//double vmin = StrToFloat(settingsVal);
	double vmin = StrToFloat(EmptyTo(GetUserSetting(User->id, sensor->uuid + "_min"), IntToStr(NULL_VALUE)));
	vmin_series->Clear();
	if (vmin != NULL_VALUE) {
		vmin_series->AddXY(xStart, vmin);
		vmin_series->AddXY(xEnd, vmin);
	}

	bool displayVirtualLimits = StrToBool(EmptyTo(GetUserSetting(User->id, sensor->uuid + "_displayVirtualLimits"), "0"));
	//settingsVal = User->GetViewSettings("sensors", sensor->uuid+"_show");
	//if (settingsVal=="") settingsVal = "false";
	//bool isVisible = StrToBool(settingsVal);
	vmax_series->Visible = displayVirtualLimits;
	vmin_series->Visible = displayVirtualLimits;
}

// ---------------------------------------------------------------------------
void __fastcall TRecordViewChart::OnMouseMoveTrack(TObject *Sender, TShiftState Shift, int X, int Y) {
	static double xVal, yVal;
	static int len;
	static int iPos;
	static double msecToDay = 1.1574074074074074074074074074074e-8;
	static double searchErr;

	searchErr = sensor->rate * msecToDay;
	Series[0]->GetCursorValues(xVal, yVal);
	len = Series[0]->XValues->Count;
	track->Top = 5;
	//TODO ошибка при поиске значений, в многотопочном режиме, при показе данных
	//в реальном времени, размер внутреннего массива данных диаграммы мен€етс€
	//происходит выход за пределы массива, Out of Range
	TDateTime dt(xVal);
	String trackTime = dt.FormatString("HH:nn:ss");

	iPos = BinSearchValueList(Series[0]->XValues, xVal, 0, len - 1, searchErr);
	if (iPos == -1 || iPos > len) {
		track->Caption = "";
	} else {
		track->Caption = FloatToStr(Series[0]->YValues->Value[iPos]);
		if (!sensor->powerPrefix.IsEmpty() || !sensor->unitName.IsEmpty()) {
			track->Caption = track->Caption + " " + sensor->powerPrefix + sensor->unitName;
		}
		int textWidthCaption = Canvas->TextWidth(track->Caption);
		int textWidthTrackTime = Canvas->TextWidth(trackTime);
		int textWidth = Max(textWidthCaption, textWidthTrackTime);

		if ((X + 20 + textWidth) > Width) {
			track->Left = X - 20 - textWidth;
		} else {
			track->Left = X + 10;
		}

		track->Caption = track->Caption + "\n" + trackTime;

		/*
		int textHeight = Canvas->TextHeight(track->Caption);
		if (Y - 45 < 0) {
			track->Top = Y + textHeight + 15;
		} else {
			track->Top = Y - 45;
		}
		*/

	}
	trackMarker->Left = X + 1;
}

//---------------------------------------------------------------------------
int __fastcall TRecordViewChart::BinSearchValueList(TChartValueList *A,  double key,  int low, int high, double exp){
	if ( low > high ){
	  return -1;
	};

	int mid = ( low + high ) / 2;
	if (fabs(A->Value[mid] - key) <= exp) {
		return mid;
	}
	if ( A->Value[mid] < key) return BinSearchValueList(A, key, mid+1, high, exp);
	if ( A->Value[mid] > key ) return BinSearchValueList(A, key, low, mid-1, exp);

	return -1;
}

//---------------------------------------------------------------------------
void TRecordViewChart::UpdateChartControls(){
	double xValMin, xValMax, yValMin, yValMax;
	xValMin = this->BottomAxis->Minimum;
	xValMax = this->BottomAxis->Maximum;

	yValMin = this->LeftAxis->Minimum;
	yValMax = this->LeftAxis->Maximum;

	//TODO make interface ChartControls

	double time, value;
	for (int ctrl = 0; ctrl < this->ControlCount; ctrl++) {
		//проверка, нужно ли скрыть этот элемент
		if (dynamic_cast<TChartText *>(this->Controls[ctrl])){
			time = ((TChartText *)this->Controls[ctrl])->time;
			value = ((TChartText *)this->Controls[ctrl])->value;

			this->Controls[ctrl]->Visible = ((time>=xValMin && time<=xValMax) &&
											 (value>=yValMin && value<=yValMax));
		} else if (dynamic_cast<TVLShape *>(this->Controls[ctrl])){
			time = ((TVLShape *)this->Controls[ctrl])->time;
			this->Controls[ctrl]->Visible = (time>=xValMin && time<=xValMax);
			((TVLShape *)this->Controls[ctrl])->timeLabel->Visible = this->Controls[ctrl]->Visible;

		} else if (dynamic_cast<TLabelBitValue *>(Controls[ctrl])) {
			time = ((TLabelBitValue *)Controls[ctrl])->startTime;
			Controls[ctrl]->Visible = (time >= xValMin && time <= xValMax);

		} else continue;

		if (!this->Controls[ctrl]->Visible) continue;

		//еще нужно Left и Top обновить
		int left, top;

		if (dynamic_cast<TChartText *>(this->Controls[ctrl])){
			//left = this->Series[CHART_SERIES_DATA_INDEX]->CalcXPosValue(time);
			//top = this->Series[CHART_SERIES_DATA_INDEX]->CalcYPosValue(value);
			top = series->CalcYPosValue(value);
			//left = (time - xValMin) * this->Width / (xValMax - xValMin);
			left = this->LeftAxis->PosAxis + (time - xValMin) * (this->Width - this->LeftAxis->PosAxis - this->MarginRight) / (xValMax - xValMin);

			this->Controls[ctrl]->Left = left;
			this->Controls[ctrl]->Top = top;
		} else if (dynamic_cast<TLabelBitValue *>(Controls[ctrl])) {
			TLabelBitValue *labelBitValue = dynamic_cast<TLabelBitValue *>(Controls[ctrl]);
			//int xStartTime = Series[CHART_SERIES_DATA_INDEX]->CalcXPosValue(labelBitValue->startTime);
			int xStartTime = series->CalcXPosValue(labelBitValue->startTime);
			//int xEndTime = Series[CHART_SERIES_DATA_INDEX]->CalcXPosValue(labelBitValue->endTime);
			int xEndTime = series->CalcXPosValue(labelBitValue->endTime);
			//top = Series[CHART_SERIES_DATA_INDEX]->CalcYPosValue(labelBitValue->val);
			left = LeftAxis->PosAxis + (time - xValMin) * (Width - LeftAxis->PosAxis - MarginRight) / (xValMax - xValMin);

			Controls[ctrl]->Left = xStartTime;
			Controls[ctrl]->Top = top;
			Controls[ctrl]->Width = xEndTime - xStartTime;

		} else {
			left = this->LeftAxis->PosAxis + (time - xValMin) * (this->Width - this->LeftAxis->PosAxis - this->MarginRight) / (xValMax - xValMin);
			//left = this->Series[CHART_SERIES_DATA_INDEX]->CalcXPosValue(time);

            ((TVLShape *)Controls[ctrl])->SetX(left);

			/*
			this->Controls[ctrl]->Left = left;
			((TVLShape *)this->Controls[ctrl])->time_label->Left = left+this->Controls[ctrl]->Width;
			*/

			if (dynamic_cast<TVLShape *>(this->Controls[ctrl])){
				this->Controls[ctrl]->Height = Height;
			}

		}
	}
}

//---------------------------------------------------------------------------
void TRecordViewChart::UpdateVLine(int x, int y){
	if (x + vline->Width + 3 < vline->Parent->Width) {
		vline->Left = x + 3;
	}
	else {
		vline->Left = x - vline->Width - 3;
	}

	vline->Top = y + 10;

	static double xVal, yVal;
	//this->Series[CHART_SERIES_DATA_INDEX]->GetCursorValues(xVal, yVal);
	series->GetCursorValues(xVal, yVal);
	vline->Caption = DateTimeToStr(xVal);
}

//---------------------------------------------------------------------------
void __fastcall TRecordViewChart::StartProgressBar(){
	Title->Text->Text = GetName() + " («агрузка данных ...)";
	Refresh();
	//Application->ProcessMessages();

	isProgressBarVisible = true;

#ifdef ENABLE_TRACE_LOG
	TLogger::logger->Log("StartProgressBar sensor:" + sensor->name, LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);
#endif
}

//---------------------------------------------------------------------------
void __fastcall TRecordViewChart::UpdateProgressBar(){
	if (!isProgressBarVisible) return;

	if (progressBarStep == 0) {
		Title->Text->Text = GetName() + " («агрузка данных / )";
	} else if (progressBarStep == 1) {
		Title->Text->Text = GetName() + " («агрузка данных \\ )";
	} else if (progressBarStep == 2) {
		Title->Text->Text = GetName() + " («агрузка данных | )";
	} else {
		Title->Text->Text = GetName() + " («агрузка данных | )";
	}
	if (++progressBarStep > 1) {
		progressBarStep = 0;
	}

	Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TRecordViewChart::StopProgressBar(){
	Title->Text->Text = GetName();
	Refresh();

	isProgressBarVisible = false;

#ifdef ENABLE_TRACE_LOG
	TLogger::logger->Log("StopProgressBar sensor:" + sensor->name, LOG_LEVEL_TRACE, __FILE__, __FUNCTION__, __LINE__);
#endif

}

//---------------------------------------------------------------------------
void TRecordViewChart::UpdateActiveAlarms(TSensor *sensor, std::vector<TData *> *activeAlarms){
	for (int ctrl = ControlCount - 1; ctrl >= 0; --ctrl) {
		if (dynamic_cast<TAlarmVLShape *>(Controls[ctrl])){
			delete Controls[ctrl];
        }
	}

	for (std::vector<TData *>::iterator itAlarm = activeAlarms->begin(), itEnd = activeAlarms->end(); itAlarm != itEnd; ++itAlarm) {
		TAlarmVLShape *alarmVLShape = new TAlarmVLShape(this, *itAlarm);
		//alarmVLShape->Parent = this;
		alarmVLShape->time = sysTime::ConvertToDaysLocalTime((*itAlarm)->msecGMT * sysTime::MSEC2SEC);
		alarmVLShape->Width = 2;
		alarmVLShape->Height = Height;
		alarmVLShape->Pen->Color = clBlack;
		alarmVLShape->Brush->Color = clBlack;
		alarmVLShape->Hint = GetAlarmText(sensor, (TDataBaseAlarm *)*itAlarm);
		alarmVLShape->ShowHint = true;

		alarmVLShape->timeLabel = new TLabel(this);
		alarmVLShape->timeLabel->Parent = this;
		alarmVLShape->timeLabel->Transparent = false;
		alarmVLShape->timeLabel->Top = 2;
		alarmVLShape->timeLabel->Caption = FormatDateTime("dd.mm.yy hh:nn:ss", alarmVLShape->time);
		alarmVLShape->timeLabel->Font->Color = clBlack;
		alarmVLShape->timeLabel->Hint = GetAlarmText(sensor, (TDataBaseAlarm *)*itAlarm);
		alarmVLShape->timeLabel->ShowHint = true;
	}

	if (activeAlarms->size()) {
		UpdateChartControls();
	}
}

//---------------------------------------------------------------------------
void TRecordViewChart::Highlight() {
	Color = ChangeColor(Color, TColorChanges::makeDarknes);
}

//---------------------------------------------------------------------------
void TRecordViewChart::UndoHighlight() {
	Color = ChangeColor(Color, TColorChanges::makeLighter);
}

//---------------------------------------------------------------------------
TColor TRecordViewChart::ChangeColor(TColor color, TColorChanges colorChanges) {
	unsigned int r = abs(color) & 0xFF,
		g = (abs(color) & (0xFF << 8)) >> 8,
		b = (abs(color) & (0xFF << 16)) >> 16;

	if (colorChanges == TColorChanges::makeLighter) {
		unsigned int inc = 25;
		r += inc; g += inc; b += inc;
		r = (r > 0xFF)?0xFF:r;
		g = (g > 0xFF)?0xFF:g;
		b = (b > 0xFF)?0xFF:b;

	} else if (colorChanges == TColorChanges::makeDarknes) {
		unsigned int inc = 25;
		r = ((r < inc)?0:(r - inc));
		g = ((g < inc)?0:(g - inc));
		b = ((b < inc)?0:(b - inc));
	}

	return (TColor)(r + (g << 8) + (b << 16));
}
#pragma package(smart_init)
