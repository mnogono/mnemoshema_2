//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)

const int UNIT_UNKNOWN = 0;
const int UNIT_V = 1;
const int UNIT_A = 2;
const int UNIT_VT = 3;
const int UNIT_VAR = 4;
const int UNIT_HZ = 5;

String GetUnitName(int unit){
	if (unit == UNIT_UNKNOWN) {
		return "";
	} else if (unit == UNIT_V) {
		return "В";
	} else if (unit == UNIT_A) {
		return "А";
	} else if (unit == UNIT_VT) {
		return "Вт";
	} else if (unit == UNIT_VAR) {
		return "Вар";
	} else if (unit == UNIT_HZ) {
		return "Гц";
	}

	return "";
}

String GetUnitNameEn(int unit){
	if (unit == UNIT_UNKNOWN) {
		return "";
	} else if (unit == UNIT_V) {
		return "U";
	} else if (unit == UNIT_A) {
		return "I";
	} else if (unit == UNIT_VT) {
		return "P";
	} else if (unit == UNIT_VAR) {
		return "Q";
	} else if (unit == UNIT_HZ) {
		return "F";
	}

	return "";
}

String GetUnitLongName(int unit){
	if (unit == UNIT_UNKNOWN) {
		return "";
	} else if (unit == UNIT_V) {
		return "В (Вольт)";
	} else if (unit == UNIT_A) {
		return "А (Ампер)";
	} else if (unit == UNIT_VT) {
		return "Вт (Ватт)";
	} else if (unit == UNIT_VAR) {
		return "ВАр (Вольт-Ампер реактивный)";
	} else if (unit == UNIT_HZ) {
		return "Гц (Герц)";
	}

	return "";
}

String GetUnitModeName(TBrizAnalogMode unit_mode){
	String unitModeName = "";

	if (unit_mode == amdNone) {
		unitModeName = "";
	} else if (unit_mode == amdDirect) {
		unitModeName = "";
	} else if (unit_mode == amdPhaseA) {
		unitModeName = "a";
	} else if (unit_mode == amdPhaseB) {
		unitModeName = "b";
	} else if (unit_mode == amdPhaseC) {
		unitModeName = "c";
	} else if (unit_mode == amdLineAB) {
		unitModeName = "ab";
	} else if (unit_mode == amdLineBC) {
		unitModeName = "bc";
	} else if (unit_mode == amdLineCA) {
		unitModeName = "ca";
	} else if (unit_mode == amdSequence0) {
		unitModeName = "0";
	} else if (unit_mode == amdSequence1) {
		unitModeName = "1";
	} else if (unit_mode == amdSequence2) {
		unitModeName = "2";
	} else if (unit_mode == amdPowerFull) {
		unitModeName = "";
	}

	return unitModeName;
}

