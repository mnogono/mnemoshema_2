//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "login.h"
#include "mp.h"
//---------------------------------------------------------------------------
USEFORM("select_report.cpp", FormReportList);
USEFORM("signals.cpp", FormSignals);
USEFORM("reports.cpp", FormReports);
USEFORM("login.cpp", FormLogin);
USEFORM("db_module.cpp", DataModuleMP); /* TDataModule: File Type */
USEFORM("debug.cpp", FormDebug);
USEFORM("chart.cpp", FormChart);
USEFORM("dashboard.cpp", FormDashboard);
USEFORM("recordInfo.cpp", FormRecordInfo);
USEFORM("mp.cpp", FormMnemoshemaMain);
//---------------------------------------------------------------------------
#include "sensor.h"
#include "sensor_data.h"

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TDataModuleMP), &DataModuleMP);

		TForm *login = new TFormLogin(Application);
		int result = login->ShowModal();
		delete login;

		if (result == mrOk) {
			Application->CreateForm(__classid(TFormMnemoshemaMain), &FormMnemoshemaMain);
			Application->CreateForm(__classid(TFormDashboard), &FormDashboard);
			Application->CreateForm(__classid(TFormChart), &FormChart);
			Application->CreateForm(__classid(TFormSignals), &FormSignals);
//			Application->CreateForm(__classid(TFormDebug), &FormDebug);
			Application->CreateForm(__classid(TFormRecordInfo), &FormRecordInfo);
			Application->CreateForm(__classid(TFormReports), &FormReports);
			Application->CreateForm(__classid(TFormReportList), &FormReportList);

			FormMnemoshemaMain->ApplicationRun();

			Application->Run();
		}
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

