//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)

TMnemoshema::TMnemoshema() {
	record_type = TRecordType::RECORD_TYPE_MNEMOSHEMA;
}

String TMnemoshema::GetName() const {
	return name;
}
