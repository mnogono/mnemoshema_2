//---------------------------------------------------------------------------

#ifndef chartH
#define chartH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.ComCtrls.hpp>

#include "mp.h"
#include "record_view.h"
#include "http_data_observer.h"
#include "record_view_chart_factory.h"
#include "sensors_tab_sheet.h"
#include "sensor_view_chart.h"
#include "mouse_strategy.h"
#include "mouse_strategy_hand.h"
#include "mouse_strategy_zoom.h"
#include "mouse_strategy_text.h"
#include "mouse_strategy_vertical_line.h"
#include "mouse_strategy_super_vertical_line.h"
#include "time_axis_observer.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>
//#include "observerable_types.h"
#include "observerable_history_date_time.h"
#include <VCLTee.Series.hpp>

class TMnemoshemaTimeRangeObserver;
//---------------------------------------------------------------------------
class TFormChart :
	public TForm,
	public sysObserver::IObserver
{
__published:	// IDE-managed Components
	TPageControl *PageControl;
	TPanel *Panel1;
	TSpeedButton *btnHand;
	TSpeedButton *btnZoom;
	TSpeedButton *btnZoomOut;
	TSpeedButton *btnText;
	TSpeedButton *btnVLine;
	TSpeedButton *btnVLines;
	TPanel *Panel6;
	TSpeedButton *btnScaleXInc;
	TSpeedButton *btnScaleXDec;
	TPanel *Panel8;
	TSpeedButton *btnScaleYInc;
	TSpeedButton *btnScaleYDec;
	TPopupMenu *PopupMenuChart;
	TMenuItem *PMTrack;
	TMenuItem *PMSaveAsImage;
	TMenuItem *PMSaveAllAsImage;
	TMenuItem *PMDeleteChart;
	TMenuItem *N10;
	TMenuItem *PMShowVirtMark;
	TMenuItem *PMSetVirtMark;
	TSaveDialog *SaveDialog;
	TPopupMenu *PopupMenuPageControl;
	TMenuItem *PMNewPage;
	TMenuItem *PMDeletePage;
	TMenuItem *PMChangePageTitle;
	TPanel *PanelBottom;
	TRadioButton *RadioButtonOnline;
	TComboBox *ComboBoxTimeRange;
	TShape *ShapeVericalSep1;
	TRadioButton *RadioButtonOffline;
	TDateTimePicker *Time2;
	TDateTimePicker *Time1;
	TDateTimePicker *Date2;
	TDateTimePicker *Date1;
	TLabel *Label1;
	TButton *ButtonRequestData;
	TPanel *v;
	TCheckBox *CheckBoxMedianFilter;
	TCheckBox *CheckBoxGaussianFilter;
	TMenuItem *PMDeleteMarker;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnHandClick(TObject *Sender);
	void __fastcall btnZoomClick(TObject *Sender);
	void __fastcall btnZoomOutClick(TObject *Sender);
	void __fastcall PMSaveAsImageClick(TObject *Sender);
	void __fastcall PMSaveAllAsImageClick(TObject *Sender);
	void __fastcall btnScaleXIncClick(TObject *Sender);
	void __fastcall btnScaleXDecClick(TObject *Sender);
	void __fastcall btnScaleYIncClick(TObject *Sender);
	void __fastcall btnScaleYDecClick(TObject *Sender);
	void __fastcall PMNewPageClick(TObject *Sender);
	void __fastcall PMChangePageTitleClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall PMDeleteChartClick(TObject *Sender);
	void __fastcall PMDeletePageClick(TObject *Sender);
	void __fastcall RadioButtonOnlineClick(TObject *Sender);
	void __fastcall RadioButtonOfflineClick(TObject *Sender);
	void __fastcall ComboBoxTimeRangeChange(TObject *Sender);
	void __fastcall ButtonRequestDataClick(TObject *Sender);
	void __fastcall PMTrackClick(TObject *Sender);
	void __fastcall btnTextClick(TObject *Sender);
	void __fastcall PMDeleteClick(TObject *Sender);
	void __fastcall btnVLineClick(TObject *Sender);
	void __fastcall PMDeleteVerticalLineClick(TObject *Sender);
	void __fastcall btnVLinesClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall CheckBoxMedianFilterClick(TObject *Sender);
	void __fastcall CheckBoxGaussianFilterClick(TObject *Sender);
	void __fastcall PMDeleteMarkerClick(TObject *Sender);
	void __fastcall PopupMenuChartPopup(TObject *Sender);
private:	// User declarations
	void __fastcall CreateDefaultTabSheet(TMnemoshemaDataManager &mnemoshemaDataManager);
public:		// User declarations
	__fastcall TFormChart(TComponent* Owner);

	void __fastcall SaveLastUserSettings();

	void __fastcall RestoreLastUserSettings();

	//create new chart into active tab sheet, return record view chart or NULL
	TRecordViewChart * __fastcall AddChart(const TRecord *record);

	//add several chart, if record is device or device tree view
	void __fastcall AddCharts(const TRecord *record);

	//request data for all visible charts using current settings of the date-time range
	void __fastcall RequestData();

	void __fastcall RequestDataByTimeRange(std::set<const TSensor *> &sensors, double dt1GMT, double dt2GMT);

	void __fastcall SetMnemoshemaDataManager(TMnemoshemaDataManager &mnemoshemaDataManager);

	//void __fastcall OnChartMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled);

	void __fastcall OnChartClick(TObject *Sender);

	void __fastcall OnChartScrollHand(TObject *Sender);

	void __fastcall OnChartDblClickHand(TObject *Sender);

	void __fastcall OnChartMouseDownHand(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

	void __fastcall OnChartMouseUpHand(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

	void __fastcall OnChartContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled);

	//void __fastcall ChangeMouseStrategy(TRecordViewChart *recordViewChart, TChartMouseStrategy mouseStrategy);

	//void __fastcall SetChartsMouseStrategy(TChartMouseStrategy strategy);

	void __fastcall SetChartsMouseStrategy(TMouseStrategy *mouseStrategy, bool disableTracker = false);

	void __fastcall CreateMouseStrategy();

	//return current visible sensors records on the chart form
	std::set<const TSensor *> __fastcall GetVisibleSensors();

	virtual void UpdateData(sysObserverable::IObserverable *data = NULL);

	void ChangeTimeAxisScale(double coefficient);

	//virtual void OnTimeAxisChanged(double dt1LocalTime, double dt2LocalTime);
public:
	TMnemoshemaDataManager *mnemoshemaDataManager;
private:
	TMnemoshemaTimeRangeObserver *mnemoshemaTimeRangeObserver;

    TMouseStrategy *activeMouseStrategy;

	TMouseStrategy *mouseStrategyHand;

	TMouseStrategy *mouseStrategyZoom;

	TMouseStrategy *mouseStrategyText;

	TMouseStrategy *mouseStrategyVerticalLine;

	TMouseStrategy *mouseStrategySuperVerticalLine;

	//observer for update the status bar text when tab time axis will be changed
	TTimeAxisObserver *timeAxisObserver;

	void ChangeSignalAxisScale(double coefficient);

	void __fastcall InitializeTimeRange();

	void __fastcall SaveOpenedPages();

	void __fastcall RestoreOpenedPages();

	void __fastcall InitChartFilterButtons();

	void __fastcall UpdateCharts();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormChart *FormChart;
//---------------------------------------------------------------------------
#endif
