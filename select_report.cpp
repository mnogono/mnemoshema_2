//---------------------------------------------------------------------------

#include "pch.h"
#pragma hdrstop

#include "select_report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormReportList *FormReportList;
//---------------------------------------------------------------------------
__fastcall TFormReportList::TFormReportList(TComponent* Owner) : TForm(Owner) {
	id = -1;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportList::ButtonCanceClick(TObject *Sender) {
	ModalResult = mrCancel;
}

//---------------------------------------------------------------------------
int TFormReportList::GetReportId() {
	return id;
}

//---------------------------------------------------------------------------
void __fastcall TFormReportList::ButtonOKClick(TObject *Sender) {
	if (ComboBoxReports->ItemIndex >= 0) {
		id = StrToInt(*reinterpret_cast<String *>(ComboBoxReports->Items->Objects[ComboBoxReports->ItemIndex]));
	} else {
        id = -1;
    }

	ModalResult = mrOk;
}

//---------------------------------------------------------------------------
//update combo box list with available template reports
void __fastcall TFormReportList::FormShow(TObject *Sender) {
	for (int i = 0; i < ComboBoxReports->Items->Count; ++i) {
		delete reinterpret_cast<String *>(ComboBoxReports->Items->Objects[i]);
	}
	ComboBoxReports->Clear();

	try {
		DataModuleMP->ADOQuery->SQL->Text = "SELECT `id`, `title` FROM reports";

		DataModuleMP->ADOQuery->Open();

		for (int i = 0; i < DataModuleMP->ADOQuery->RecordCount; i++, DataModuleMP->ADOQuery->Next()) {
			String title = DataModuleMP->ADOQuery->FieldByName("title")->AsString;
			int id = DataModuleMP->ADOQuery->FieldByName("id")->AsInteger;

			ComboBoxReports->Items->AddObject(title, reinterpret_cast<TObject *>(new String(id)));
		}
	} catch (Exception &e) {
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.ToString().c_str());
	}
	DataModuleMP->ADOQuery->Close();

}
//---------------------------------------------------------------------------
