//---------------------------------------------------------------------------

#ifndef select_reportH
#define select_reportH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormReportList : public TForm
{
__published:	// IDE-managed Components
	TComboBox *ComboBoxReports;
	TLabel *Label1;
	TButton *ButtonCance;
	TButton *ButtonOK;
	void __fastcall ButtonCanceClick(TObject *Sender);
	void __fastcall ButtonOKClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	int id;
public:		// User declarations
	__fastcall TFormReportList(TComponent* Owner);

	int GetReportId();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportList *FormReportList;
//---------------------------------------------------------------------------
#endif
