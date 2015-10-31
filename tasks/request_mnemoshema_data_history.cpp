//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "request_mnemoshema_data_history.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TTaskRequestMnemoshemaDataHistory::TTaskRequestMnemoshemaDataHistory() : TTaskRequestDeviceDataFileFormat() {
}

//---------------------------------------------------------------------------
TTaskRequestMnemoshemaDataHistory::~TTaskRequestMnemoshemaDataHistory() {}

//---------------------------------------------------------------------------
int TTaskRequestMnemoshemaDataHistory::GetObserverableType() {
	return TObserverableTypes::MNEMOSHEMA_DATA_HISTORY;
}

//---------------------------------------------------------------------------
void TTaskRequestMnemoshemaDataHistory::GetOffsets(FILE *f, __int64 fileSize, size_t dataSize, long ms1, long ms2, int &offset1, int &offset2) {
	offset1 = FindPositionByteOffsetInDataFile(f, fileSize, dataSize, ms1);
	offset2 = offset1 + dataSize;
}
