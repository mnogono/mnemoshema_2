//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)

const int POWER_E = 6;
const int POWER_P = 5;
const int POWER_T = 4;
const int POWER_G = 3;
const int POWER_M = 2;
const int POWER_k = 1;
const int POWER_0 = 0;
const int POWER_m = -1;
const int POWER_mk = -2;
const int POWER_n = -3;
const int POWER_p = -4;
const int POWER_f = -5;
const int POWER_a = -6;

String GetPowerPrefix(int powerType){
	if (powerType == POWER_E) {
		return "�";
	} else if (powerType == POWER_P) {
		return "�";
	} else if (powerType == POWER_T) {
		return "�";
	} else if (powerType == POWER_G) {
		return "�";
	} else if (powerType == POWER_M) {
		return "�";
	} else if (powerType == POWER_k) {
		return "�";
	} else if (powerType == POWER_0) {
		return "";
	} else if (powerType == POWER_m) {
		return "�";
	} else if (powerType == POWER_mk) {
		return "��";
	} else if (powerType == POWER_n) {
		return "�";
	} else if (powerType == POWER_p) {
		return "�";
	} else if (powerType == POWER_f) {
		return "�";
	} else if (powerType == POWER_a) {
		return "�";
	}

	return "";
}

String GetPowerLongPrefix(int powerType){
	if (powerType == POWER_E) {
		return "�";
	} else if (powerType == POWER_P) {
		return "�";
	} else if (powerType == POWER_T) {
		return "� (����)";
	} else if (powerType == POWER_G) {
		return "� (����)";
	} else if (powerType == POWER_M) {
		return "� (����)";
	} else if (powerType == POWER_k) {
		return "� (����)";
	} else if (powerType == POWER_0) {
		return "";
	} else if (powerType == POWER_m) {
		return "� (����)";
	} else if (powerType == POWER_mk) {
		return "�� (�����)";
	} else if (powerType == POWER_n) {
		return "� (����)";
	} else if (powerType == POWER_p) {
		return "� (����)";
	} else if (powerType == POWER_f) {
		return "�";
	} else if (powerType == POWER_a) {
		return "�";
	}

	return "";
}

double GetPowerValue(int powerType){
	if (powerType == POWER_E) {
		return 1e-18;
	} else if (powerType == POWER_P) {
		return 1e-15;
	} else if (powerType == POWER_T) {
		return 1e-12;
	} else if (powerType == POWER_G) {
		return 1e-9;
	} else if (powerType == POWER_M) {
		return 1e-6;
	} else if (powerType == POWER_k) {
		return 1e-3;
	} else if (powerType == POWER_0) {
		return 1.0;
	} else if (powerType == POWER_m) {
		return 1e3;
	} else if (powerType == POWER_mk) {
		return 1e6;
	} else if (powerType == POWER_n) {
		return 1e9;
	} else if (powerType == POWER_p) {
		return 1e12;
	} else if (powerType == POWER_f) {
		return 1e15;
	} else if (powerType == POWER_a) {
		return 1e18;
	}

	return 1.0;
}
