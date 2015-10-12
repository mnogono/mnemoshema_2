//---------------------------------------------------------------------------

#ifndef unitH
#define unitH
//---------------------------------------------------------------------------

extern const int UNIT_UNKNOWN;
extern const int UNIT_V;
extern const int UNIT_A;
extern const int UNIT_VT;
extern const int UNIT_VAR;
extern const int UNIT_HZ;

// return unit name rus
String GetUnitName(int unit);

// return unit name eng
String GetUnitNameEn(int unit);

String GetUnitLongName(int unit);

String GetPowerPrefix(int powerType);

String GetPowerLongPrefix(int powerType);

double GetPowerValue(int powerType);

String GetUnitModeName(TBrizAnalogMode unit_mode);

#endif
