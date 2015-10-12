//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "mnemoshema_data_manager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TMnemoshemaDataManager::TMnemoshemaDataManager(){
	Clear();
}

void TMnemoshemaDataManager::Clear() {
	finishedTask = NULL;

	dt1GMT = NULL;
	dt2GMT = NULL;
}

//clear and set finished task
void TMnemoshemaDataManager::SetFinishedTask(TTaskRequestMnemoshemaData *finishedTask) {
	Clear();

	this->finishedTask = finishedTask;
}

//clear and set time range
void TMnemoshemaDataManager::SetTimeRange(TDateTime *dt1GMT, TDateTime *dt2GMT) {
	Clear();

	this->dt1GMT = dt1GMT;
	this->dt2GMT = dt2GMT;
}

TTaskRequestMnemoshemaData * TMnemoshemaDataManager::GetFinishedTask() {
	return finishedTask;
}

void TMnemoshemaDataManager::GetTimeRange(TDateTime **dt1GMT, TDateTime **dt2GMT) {
	*dt1GMT = this->dt1GMT;
	*dt2GMT = this->dt2GMT;
}


