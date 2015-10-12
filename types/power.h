//---------------------------------------------------------------------------
#ifndef powerH
#define powerH
//---------------------------------------------------------------------------

extern const int POWER_E;
extern const int POWER_P;
extern const int POWER_T;
extern const int POWER_G;
extern const int POWER_M;
extern const int POWER_k;
extern const int POWER_0;
extern const int POWER_m;
extern const int POWER_mk;
extern const int POWER_n;
extern const int POWER_p;
extern const int POWER_f;
extern const int POWER_a;

String GetPowerPrefix(int powerType);

String GetPowerLongPrefix(int powerType);

double GetPowerValue(int powerType);

#endif
