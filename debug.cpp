//---------------------------------------------------------------------------
#include "pch.h"
#include <vcl.h>
#pragma hdrstop

#include "debug.h"

class TThreadHTTPDataUISynchronize;

//thread pool, each device http data requests by thread from this pool
//extern sysThread::TThreadPoolBorland<TThreadHTTPDataUISynchronize> *threadPool;

class TTaskRequestHTTPData;

extern sysPatterns::TPool<TTaskRequestHTTPData> *httpRequestPool;

class TTaskRequestFileData;

extern sysPatterns::TPool<TTaskRequestFileData> *fileRequestPool;

class TTaskRequestDeviceFileData;

extern sysPatterns::TPool<TTaskRequestDeviceFileData> *deviceFileRequestPool;

//extern sysPatterns::TPool<TTaskRequestMnemoshemaData> *requestPool;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDebug *FormDebug;
//---------------------------------------------------------------------------
__fastcall TFormDebug::TFormDebug(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormDebug::TimerSensorDataPoolUpdateTimer(TObject *Sender)
{
	LabelSensorDataPoolUsed->Caption = IntToStr((int)SENSOR_DATA_POOL.GetUsedSize());
	LabelSensorDataPoolAvailable->Caption = IntToStr((int)SENSOR_DATA_POOL.GetAvailableSize());

	LabelThreadPoolTasks->Caption = IntToStr((int)THREAD_POOL->GetTaskQueueCount());


	LabelHTTPRequestPoolUsed->Caption = IntToStr((int)HTTP_REQUEST_POOL->GetUsedSize());

	LabelHTTPRequestPoolAvailable->Caption = IntToStr((int)HTTP_REQUEST_POOL->GetAvailableSize());

	//LabelFileRequestPoolUsed->Caption = IntToStr((int)FILE_REQUEST_POOL->GetUsedSize());

	//LabelFileRequestPoolAvailable->Caption = IntToStr((int)FILE_REQUEST_POOL->GetAvailableSize());

	LabelSensorEventDataPoolUsed->Caption = IntToStr((int)SENSOR_EVENT_DATA_POOL.GetUsedSize());
	LabelSensorEventDataPoolAvailable->Caption = IntToStr((int)SENSOR_EVENT_DATA_POOL.GetAvailableSize());
}
//---------------------------------------------------------------------------
void __fastcall TFormDebug::FormCreate(TObject *Sender)
{
	RestoreLastUserSettings();
}

//---------------------------------------------------------------------------
void __fastcall TFormDebug::SaveLastUserSettings() {
	//save user settings before close the application
	sysApp::SaveFormSettings(this);
}

//---------------------------------------------------------------------------
void __fastcall TFormDebug::RestoreLastUserSettings() {
	sysApp::RestoreFormSettings(this);
}

//---------------------------------------------------------------------------
void __fastcall TFormDebug::FormDestroy(TObject *Sender)
{
	SaveLastUserSettings();
}
//---------------------------------------------------------------------------


