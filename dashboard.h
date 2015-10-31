//---------------------------------------------------------------------------

#ifndef dashboardH
#define dashboardH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>

#include "mnemoshema_data_manager.h"
#include "http_data_observer.h"
#include "time_range_data_observer.h"
#include "device_file_data_observer.h"
#include "mnemoshema_data_history_observer.h"
#include "record_view.h"
#include "SensorView.h"
#include "SensorViewFactory.h"
#include <Vcl.ExtCtrls.hpp>
#include "dashboard_mouse_strategy.h"
#include "tree_view_observer.h"
#include "crane_mimic_panel.h"
#include "crane.h"
//---------------------------------------------------------------------------

//disable warning about all interface should be inherted from IUnknown
#pragma option push -w-8130

class TFormDashboard :
	public TForm,
	public ITreeViewEvents
{
__published:	// IDE-managed Components
	TPageControl *PageControlMP;
	TPanel *Panel1;
	TCheckBox *CheckBoxDataHistory;
	TDateTimePicker *DateDashboardHistory;
	TDateTimePicker *TimeDashboardHistory;
	TButton *ButtonRequestDataMnemoshemaHistory;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CheckBoxDataHistoryClick(TObject *Sender);
	void __fastcall ButtonRequestDataMnemoshemaHistoryClick(TObject *Sender);
	void __fastcall DateDashboardHistoryChange(TObject *Sender);
	void __fastcall TimeDashboardHistoryChange(TObject *Sender);
private:
	TCraneMimicPanel *craneMimicPanel;

	TCrane *crane1;

	TCrane *crane2;
public:		// User declarations
	sysObserver::IObserver *httpDataObserver;

	sysObserver::IObserver *timeRangeDataObserver;

	sysObserver::IObserver *mnemoshemaDataHistoryObserver;

	//sysObserver::IObserver *deviceFileDataObserver;

	//each record can have several record view control
	std::map<const TSensor *, std::list<IMnemoshemaView *> *> mshViews;

	TDashboardMouseStrategy dashboardMouseStrategy;

public:
	__fastcall TFormDashboard(TComponent* Owner);

	void __fastcall CreateMnemoshemsPages();

	void __fastcall LoadDashboardElements();

	void __fastcall AddSensorView(TWinControl *parent, TRecord *record, int x, int y);

	void __fastcall CreateHTTPDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager);

	void __fastcall CreateTimeRangeDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager);

	void __fastcall CreateMnemoshemaDataHistoryObserver(TMnemoshemaDataManager &mnemoshemaDataManager);

	//void __fastcall CreateFileDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager);

	//void __fastcall CreateDeviceFileDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager);

	void InitCreatedRecordView(TSensorView *createdRecordView);

	void __fastcall RestoreLastUserSettings();

	void __fastcall SaveLastUserSettings();

	void __fastcall SetMnemoshemaDataManager(TMnemoshemaDataManager &mnemoshemaDataManager);

	void OnTreeViewNodeChange(TTreeNode *node);

	void __fastcall InitializeCraneMimicPanel(TScrollBox *ScrollBoxImage, String &mimePanelBkgImgName);

	void __fastcall RequestDataMnemoshemaHistory();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDashboard *FormDashboard;
//---------------------------------------------------------------------------

#pragma option pop

#endif
