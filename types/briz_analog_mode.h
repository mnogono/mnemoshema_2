//---------------------------------------------------------------------------

#ifndef briz_analog_modeH
#define briz_analog_modeH
//---------------------------------------------------------------------------

enum TBrizAnalogMode {
	amdNone = 0, // ���
    amdDirect, // ������
    amdPhaseA, // ���� A
    amdPhaseB, // ���� B
    amdPhaseC, // ���� C
    amdLineAB, // ����� AB
    amdLineBC, // ����� BC
    amdLineCA, // ����� CA
    amdSequence0, // ������� ������������������
    amdSequence1, // ������ ������������������
    amdSequence2, // �������� ������������������
    amdPowerFull // ������ ��������
};

#endif
