//---------------------------------------------------------------------------

#ifndef mpH
#define mpH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.EngExt.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <Vcl.Bind.DBEngExt.hpp>
#include <Vcl.Bind.Editors.hpp>
#include <Vcl.CategoryButtons.hpp>

#include "dashboard.h"
#include "chart.h"
#include "signals.h"
#include "debug.h"
#include "reports.h"
#include "select_report.h"

#include "http_data_manager.h"
#include "file_data_manager.h"
#include "device_tree.h"
#include "request_http_data.h"
#include "request_file_data.h"
#include "request_device_file_data.h"
#include "request_device_data_file_format.h"
#include "request_device_event_data_file_format.h"
#include "request_mnemoshema_data_history.h"
#include "recordInfo.h"
#include <Vcl.Menus.hpp>
#include "observerable_history_date_time.h"
#include <Vcl.Grids.hpp>
#include "request_file_event_data.h"
#include "sensor_event_data.h"
#include "requested_time_range_data.h"
#include "request_device_event_data_file_format.h"
#include "string_grid_events_observer.h"
#include "db_module.h"
//---------------------------------------------------------------------------

#pragma option push -w-8130

class TFormMnemoshemaMain :
	public TForm,
	public sysObserver::IObserver
{
__published:	// IDE-managed Components
	TPanel *PanelMnemoshemaLeft;
	TPanel *PanelSensorSearch;
	TLabel *Label1;
	TEdit *EditSearch;
	TButton *ButtonClearFilterText;
	TSplitter *Splitter1;
	TTreeView *TreeViewDevice;
	TPanel *PanelEvents;
	TSplitter *Splitter2;
	TTimer *TimerUpdateHTTPData;
	TPopupMenu *PopupMenuTreeViewDevice;
	TMenuItem *PMBuildChart;
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *MMClose;
	TMenuItem *N2;
	TMenuItem *N8;
	TMenuItem *N10;
	TMenuItem *N11;
	TTimer *TimerUpdateEventsData;
	TPopupMenu *PopupMenuRecordView;
	TMenuItem *PMBuildChartForRecordView;
	TTreeView *TreeViewDeviceFiltered;
	TMenuItem *N4;
	TMenuItem *MMOpenOffice;
	TMenuItem *MMReportTemplateManager;
	TPopupMenu *PopupMenuStringGridEvent;
	TMenuItem *PMOpenEventOnChart;
	TPanel *PanelEventDateTimeSettings;
	TCheckBox *CheckBoxEventByDate;
	TDateTimePicker *DateTimePickerEventDate;
	TMenuItem *PMOpenEventFolder;
	TMenuItem *PMCreateEventReport;
	TMemo *MemoEventDescription;
	TSplitter *Splitter3;
	TMenuItem *FindSensor;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TimerUpdateHTTPDataTimer(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall PMBuildChartClick(TObject *Sender);
	void __fastcall MMCloseClick(TObject *Sender);
	void __fastcall N2Click(TObject *Sender);
	void __fastcall TreeViewDeviceChange(TObject *Sender, TTreeNode *Node);
	void __fastcall N8Click(TObject *Sender);
	void __fastcall N9Click(TObject *Sender);
	void __fastcall N10Click(TObject *Sender);
	void __fastcall N11Click(TObject *Sender);
	void __fastcall TimerUpdateEventsDataTimer(TObject *Sender);
	void __fastcall Updateblob1Click(TObject *Sender);
	void __fastcall PMBuildChartForRecordViewClick(TObject *Sender);
	void __fastcall EditSearchChange(TObject *Sender);
	void __fastcall ButtonClearFilterTextClick(TObject *Sender);
	void __fastcall MMOpenOfficeClick(TObject *Sender);
	void __fastcall MMReportTemplateManagerClick(TObject *Sender);
	void __fastcall PMOpenEventOnChartClick(TObject *Sender);
	void __fastcall DateTimePickerEventDateChange(TObject *Sender);
	void __fastcall CheckBoxEventByDateClick(TObject *Sender);
	void __fastcall PMOpenEventFolderClick(TObject *Sender);
	void __fastcall PMCreateEventReportClick(TObject *Sender);
	void __fastcall FindSensorClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	//http data manager, notify all observer for new sensor data
	//THTTPDataManager httpDataManager;
	TMnemoshemaDataManager mnemoshemaDataManager;

	//access control for httpDataManager
	CRITICAL_SECTION csHttpDataManagerAccess;
	//sysObserverable::TObserverable treeViewDevice;

	TStringGridEventsObserver stringGridEventsObserver;

	TStringGridDatasource<TDatasourceObjectEvent> *stringGridEvents;
public:
	__fastcall TFormMnemoshemaMain(TComponent* Owner);

	void __fastcall CreateThreadPool();

	void __fastcall RegistrerHTTPDataObservers();

	void __fastcall RestoreLastUserSettings();

	void __fastcall SaveLastUserSettings();

	//function invoked after all form is initialized
	void __fastcall ApplicationRun();

	void __fastcall InitializeStringGridEvents();

	void UpdateData(sysObserverable::IObserverable *data = NULL);

	//static void RequestTimeRangeDataFileFormatSensor(std::set<const TSensor *> &sensors, double dt1GMT, double dt2GMT, double resolution);

	static void RequestMnemoshemaData(std::set<const TSensor *> &sensors, double dtGMT);

	static void RequestTimeRangeDataFileFormatDevice(std::set<const TSensor *> &sensors, double dt1GMT, double dt2GMT, double resolution);

	void __fastcall BuildChart(const TRecord *record);

	void __fastcall InitializeDashboardRecordViewPopupMenu();

	bool __fastcall RegisterSkyReportLibrary(const String &sOfficeFile);

	void __fastcall UpdateStringGridEvents(TTaskRequestDeviceEventDataFileFormat *data);

	void __fastcall CreateStringGridEvents();

	void __fastcall RequestEventsByDate(double dtLocalTime);

	void __fastcall InitUpdateHTTPTimer();

	void __fastcall InitUpdateEventsData();

	void __fastcall InitChartFilters();

	void __fastcall StringGridEventClick(TObject *Sender);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMnemoshemaMain *FormMnemoshemaMain;
//---------------------------------------------------------------------------

#pragma option pop

#endif
