//---------------------------------------------------------------------------

#ifndef debugH
#define debugH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "mnemoshema_data_manager.h"

//---------------------------------------------------------------------------
class TFormDebug : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *LabelSensorDataPoolUsed;
	TLabel *LabelSensorDataPoolAvailable;
	TTimer *TimerSensorDataPoolUpdate;
	TLabel *Label3;
	TLabel *LabelThreadPoolTasks;
	TLabel *Label4;
	TLabel *LabelHTTPRequestPoolUsed;
	TLabel *Label5;
	TLabel *LabelHTTPRequestPoolAvailable;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *LabelFileRequestPoolUsed;
	TLabel *LabelFileRequestPoolAvailable;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *LabelDeviceFileRequestPoolUsed;
	TLabel *LabelDeviceFileRequestPoolAvailable;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *LabelRequestPoolUsed;
	TLabel *LabelRequestPoolAvailable;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *LabelSensorEventDataPoolUsed;
	TLabel *LabelSensorEventDataPoolAvailable;
	void __fastcall TimerSensorDataPoolUpdateTimer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormDebug(TComponent* Owner);
	void __fastcall SaveLastUserSettings();
	void __fastcall RestoreLastUserSettings();

};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebug *FormDebug;
//---------------------------------------------------------------------------
#endif
