//---------------------------------------------------------------------------

#ifndef reportsH
#define reportsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TFormReports : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TButton *ButtonSelectTemplateFile;
	TEdit *EditTemplateTitle;
	TLabel *Label6;
	TEdit *EditReportTemplateFile;
	TButton *ButtonSaveTempateFile;
	TDBGrid *DBGridReports;
	TDataSource *DataSource;
	TADOQuery *ADOQuery;
	TButton *ButtonOpenReportTemplate;
	TButton *ButtonDeleteReportTemplate;
	void __fastcall ButtonSelectTemplateFileClick(TObject *Sender);
	void __fastcall ButtonSaveTempateFileClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ButtonDeleteReportTemplateClick(TObject *Sender);
	void __fastcall ButtonOpenReportTemplateClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormReports(TComponent* Owner);

	void __fastcall RestoreLastUserSettings();

	void __fastcall SaveLastUserSettings();

	void __fastcall DeleteSelectedReports();

	void __fastcall UpdateReportsGrid();

	void __fastcall SaveReportToFile(int reportId, wchar_t *file);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReports *FormReports;
//---------------------------------------------------------------------------
#endif
