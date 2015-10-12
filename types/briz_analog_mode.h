//---------------------------------------------------------------------------

#ifndef briz_analog_modeH
#define briz_analog_modeH
//---------------------------------------------------------------------------

enum TBrizAnalogMode {
	amdNone = 0, // нет
    amdDirect, // сигнал
    amdPhaseA, // фаза A
    amdPhaseB, // фаза B
    amdPhaseC, // фаза C
    amdLineAB, // линия AB
    amdLineBC, // линия BC
    amdLineCA, // линия CA
    amdSequence0, // нулевая последовательность
    amdSequence1, // прямая последовательность
    amdSequence2, // обратная последовательность
    amdPowerFull // полная мощность
};

#endif
