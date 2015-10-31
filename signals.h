//---------------------------------------------------------------------------

#ifndef signalsH
#define signalsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>

#include "mp.h"
#include "record_view.h"
#include "record_view_string_grid_container.h"
#include "tree_view_observer.h"

//disable warning about all interface should be inherted from IUnknown
#pragma option push -w-8130

//---------------------------------------------------------------------------
class TFormSignals :
	public TForm,
	public ITreeViewEvents
{
__published:	// IDE-managed Components
	TStringGrid *StringGridSignals;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormDestroy(TObject *Sender);
private:
	sysObserver::IObserver *httpDataObserver;
	sysObserver::IObserver *timeRangeDataObserver;
	sysObserver::IObserver *mnemoshemaDataHistoryObserver;

	void __fastcall CreateHTTPDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager);
	void __fastcall CreateTimeRangeDataObserver(TMnemoshemaDataManager &mnemoshemaDataManager);
	void __fastcall CreateMnemoshemaDataHistoryObserver(TMnemoshemaDataManager &mnemoshemaDataManager);
public:
	//record and string grid signals row
	std::map<const TSensor *, std::list<IMnemoshemaView *> *> mshViews;
public:
	__fastcall TFormSignals(TComponent* Owner);
	void __fastcall CreateRecordViews();
	void __fastcall SaveLastUserSettings();
	void __fastcall RestoreLastUserSettings();
	void __fastcall SetMnemoshemaDataManager(TMnemoshemaDataManager &mnemoshemaDataManager);
	virtual void OnTreeViewNodeChange(TTreeNode *node);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSignals *FormSignals;
//---------------------------------------------------------------------------

#pragma option pop

#endif
