//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "login.h"
#include "db.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLogin *FormLogin;
//---------------------------------------------------------------------------
__fastcall TFormLogin::TFormLogin(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormLogin::ButtonOkClick(TObject *Sender)
{
	TUser user = LoadUser(EditLogin->Text, EditPassword->Text);
	if (user.id == -1) {
		ShowMessage("Не верный логин или пароль");
	} else {
		sysApp::SetSetting(L"last_login_user", user.login.c_str());

		LoadCollectionPoints(user);

		LoadSensors(user);

		LoadDevices(user);

		LoadSensorsBits(user);

		LoadDeviceTreeGroups(user);

		LoadMnemoshems(user);

		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormLogin::ButtonCancelClick(TObject *Sender) {
	ModalResult = mrCancel;
}

//---------------------------------------------------------------------------
void __fastcall TFormLogin::FormShow(TObject *Sender) {
	if (!EditLogin->Text.IsEmpty()){
		EditPassword->SetFocus();
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormLogin::FormCreate(TObject *Sender) {
	std::wstring fileVersion = sysFile::getExeVersion(Application->ExeName.c_str(), 1251);
	Caption = Caption + String(L" ") + String(fileVersion.c_str());

	if (!DataModuleMP->ADOConnection->Connected) {
		ButtonOk->Enabled = false;
	} else {
		UpdateSoftware();
	}

	// restore last login user
	EditLogin->Text = sysApp::GetSettingString(L"last_login_user", "");
}

//---------------------------------------------------------------------------
void __fastcall TFormLogin::UpdateSoftware() {
	if (DataModuleMP->GetSettingInt("enable_auto_update")) {
		String exeName = ExtractFileName(Application->ExeName);
		HANDLE hApplicationTerminate = CreateMutex(NULL, NULL, exeName.c_str());
		if (!hApplicationTerminate) {
			ShowMessage("Не могу создать Mutex. Приложение будет закрыто.");
			sysLogger::ERR_A("Can't create mutex");
			Application->Terminate();
		} else if (GetLastError() == ERROR_ALREADY_EXISTS) {
			ShowMessage("Приложение уже запущено, текущая копия будет закрыта.");
			sysLogger::ERR_A("Application already ran. Current instance will be terminated");
			Application->Terminate();
		}

		PROCESS_INFORMATION pi;
		STARTUPINFO si = {
			sizeof(STARTUPINFO)
		};

		wchar_t *cwd = sysFile::GetExecutableFolder();
		wchar_t wFileProjectAppUpdate[MAX_PATH];
		wsprintf(wFileProjectAppUpdate, L"%s\\ProjectAppUpdate.exe", cwd);
		if (sysFile::IsFileExist(wFileProjectAppUpdate)) {
			String cmd;
			cmd.sprintf(L"cmd /C ProjectAppUpdate.exe -u %s", exeName.c_str());

			::CreateProcess(NULL, cmd.c_str(), NULL, NULL, 0, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		} else {
			String msg;
			msg.sprintf(L"Не удалось запустить программу для обновления ПО '%s'", wFileProjectAppUpdate);
			MessageBox(NULL, msg.c_str(), L"Ошибка", MB_OK);

			sysLogger::ERR_A("Can't ran ProjectAppUpdate:");
			sysLogger::ERR_W(wFileProjectAppUpdate);
		}

        delete []cwd;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormLogin::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == 13){
		ButtonOkClick(Sender);
	} else if (Key == 27){
		ButtonCancelClick(Sender);
	}
}
//---------------------------------------------------------------------------

