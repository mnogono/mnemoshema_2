//---------------------------------------------------------------------------

#include "pch.h"
#pragma hdrstop

#include "reports.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormReports *FormReports;
//---------------------------------------------------------------------------
__fastcall TFormReports::TFormReports(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::ButtonSelectTemplateFileClick(TObject *Sender) {
	if (OpenDialog->Execute(NULL)){
		EditReportTemplateFile->Text = OpenDialog->FileName;
		EditTemplateTitle->Text = ExtractFileName(OpenDialog->FileName);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::ButtonSaveTempateFileClick(TObject *Sender) {
	String templateFile = EditReportTemplateFile->Text;
	if (templateFile.IsEmpty()){
		MessageBox(NULL, L"Укажите файл шаблона", L"Информация", MB_OK);
		return;
	}

	if (EditTemplateTitle->Text.IsEmpty()){
		MessageBox(NULL, L"Укажите имя шаблона", L"Информация", MB_OK);
	}

	if (!sysFile::IsFileExist(templateFile.c_str())){
		String msg;
		msg.sprintf(L"Файл не найден '%s'", templateFile.c_str());
		MessageBox(NULL, msg.c_str(), L"Информация", MB_OK);
		return;
	}

	char *sTemplateBuffer = sysFile::BinaryToString(templateFile.c_str());
	wchar_t *wcTemplateBuffer;
	size_t templateBufferSize;
	sysStr::MBToW(sTemplateBuffer, wcTemplateBuffer, templateBufferSize);
	delete[]sTemplateBuffer;

	bool isSuccess = false;
	try {
		String sSQL;
		sSQL = sSQL.sprintf(L"insert into reports (`name`, `title`,`template`) values ('%s', '%s', x'%s')",
			EditTemplateTitle->Text.c_str(), EditTemplateTitle->Text.c_str(), wcTemplateBuffer);

		DataModuleMP->ADOQuery->SQL->Text = sSQL;

		delete[]wcTemplateBuffer;

		DataModuleMP->ADOQuery->ExecSQL();

		isSuccess = true;
	} catch (Exception &e) {
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.ToString().c_str());
	}
	DataModuleMP->ADOQuery->Close();

	if (isSuccess == true) {
		MessageBox(NULL, L"Шаблон отчета успешно сохранен", L"Информация", MB_OK);
	} else {
		MessageBox(NULL, L"Не удалось сохранить шаблон отчета", L"Информация", MB_OK);
	}

	UpdateReportsGrid();
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::FormCloseQuery(TObject *Sender, bool &CanClose) {
	WindowState = wsMinimized;
	CanClose = false;
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::FormCreate(TObject *Sender) {
	RestoreLastUserSettings();

	ADOQuery->Connection = DataModuleMP->ADOConnection;
	ADOQuery->Active = true;
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::RestoreLastUserSettings() {
	sysApp::RestoreFormSettings(this);
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::FormDestroy(TObject *Sender) {
	SaveLastUserSettings();
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::SaveLastUserSettings() {
	sysApp::SaveFormSettings(this);
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::UpdateReportsGrid() {
	/*
	try {
		ADOQuery->ExecSQL();
	} catch (Exception &e) {
		sysLogger::ERR_W(e.ToString().c_str());
	}
	*/
	//ADOQuery->Refresh();
	ADOQuery->DisableControls();
	try {
		ADOQuery->Close();
		ADOQuery->Open();
	} catch (Exception &e) {
		sysLogger::ERR_W(e.ToString().c_str());
	}
	ADOQuery->EnableControls();
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::DeleteSelectedReports() {
	if(DBGridReports->SelectedRows->Count == 0) {
		MessageBox(NULL, L"Не выделены ни одна строка для удаления", L"Информация", MB_OK);
		return;
	}

	ADOQuery->DisableControls();
	int item = ADOQuery->RecNo;
	if (DBGridReports->SelectedRows->Count == 1) {
		String msg;
		msg = msg.sprintf(L"Удалить выбранный шаблон отчета (%s)?", ADOQuery->FieldByName("title")->AsString.c_str());
		if (MessageBox(NULL, msg.c_str(), L"Удаление отчета", MB_YESNO) == ID_NO) {
			DBGridReports->SelectedRows->Clear();
			ADOQuery->EnableControls();
			return;
		}
	} else {
		String msg;
		msg = msg.sprintf(L"Удалить выбранные шаблоны отчетов?");
		if (MessageBox(NULL, msg.c_str(), L"Удаление отчета", MB_YESNO) == ID_NO) {
			DBGridReports->SelectedRows->Clear();
			ADOQuery->EnableControls();
			return;
		}
    }

	std::vector<std::wstring> ids;
	wchar_t wId[33];

	ADOQuery->First();
	while(!ADOQuery->Eof) {
		if(DBGridReports->SelectedRows->CurrentRowSelected) {
			_ltow(ADOQuery->FieldByName("id")->AsInteger, wId, 10);
			ids.push_back(std::wstring(wId));
		}
		ADOQuery->Next();
	}

	if (ids.size() == 0) {
		return;
	}

	try {
		std::wstring wSQL = std::wstring(L"DELETE FROM reports WHERE id IN (")
			+ sysStr::JoinStrings(ids, L",")
			+ std::wstring(L")");

		DataModuleMP->ADOQuery->SQL->Text =  String(wSQL.c_str());
		DataModuleMP->ADOQuery->ExecSQL();

		DBGridReports->SelectedRows->Clear();
	} catch (Exception &e) {
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.ToString().c_str());
	}

	if(ADOQuery->RecordCount > 0) {
		if(item > ADOQuery->RecordCount) {
			ADOQuery->Last();
		} else {
			ADOQuery->RecNo = item;
		}
	}

	ADOQuery->EnableControls();

	UpdateReportsGrid();
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::ButtonDeleteReportTemplateClick(TObject *Sender) {
	DeleteSelectedReports();
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::SaveReportToFile(int reportId, wchar_t *file) {
	try {
		DataModuleMP->ADOQuery->SQL->Text = "SELECT `template` FROM reports WHERE id = " + IntToStr(reportId);

		DataModuleMP->ADOQuery->Open();

		if (DataModuleMP->ADOQuery->RecordCount > 0) {
			System::DynamicArray<System::Byte> reportTemplate = DataModuleMP->ADOQuery->FieldByName("template")->AsBytes;

			sysFile::WriteToFile(file, reportTemplate);
		}
	} catch (Exception &e) {
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.ToString().c_str());
	}
	DataModuleMP->ADOQuery->Close();
}

//---------------------------------------------------------------------------
void __fastcall TFormReports::ButtonOpenReportTemplateClick(TObject *Sender) {
	String sOfficeFile;
	if (sysOO::IsOpenOfficeInstalled("4.1.1", sOfficeFile) == false) {
		MessageBox(NULL, L"Не установленно программное обеспечение Open Office 4.1.1.", L"Информация", MB_OK);
		return;

	}

	if (DBGridReports->SelectedRows->Count == 0) {
		MessageBox(NULL, L"Выберите шаблон для редактирования", L"Информация", MB_OK);
		return;
	}
	if (DBGridReports->SelectedRows->Count != 1) {
		MessageBox(NULL, L"Выберите один шаблон для редактирования", L"Информация", MB_OK);
		return;
	}

	if (SaveDialog->Execute(NULL) == false) {
		return;
	}
	String saveTo = SaveDialog->FileName;

	int id = ADOQuery->FieldByName("id")->AsInteger;

	SaveReportToFile(id, saveTo.c_str());
	/*
	try {
		DataModuleMP->ADOQuery->SQL->Text = "SELECT `template` FROM reports WHERE id = " + IntToStr(id);

		DataModuleMP->ADOQuery->Open();

		if (DataModuleMP->ADOQuery->RecordCount > 0) {
			System::DynamicArray<System::Byte> reportTemplate = DataModuleMP->ADOQuery->FieldByName("template")->AsBytes;

			sysFile::WriteToFile(saveTo.c_str(), reportTemplate);
		}
	} catch (Exception &e) {
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.ToString().c_str());
	}
	DataModuleMP->ADOQuery->Close();
	*/

	sysOO::OpenFile(sOfficeFile, saveTo.c_str());

	//sysProcess::ExecuteCmd(sOfficeFile.c_str(), L"", 5000);
}
//---------------------------------------------------------------------------

