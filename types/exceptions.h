//---------------------------------------------------------------------------
#ifndef exceptionsH
#define exceptionsH
//---------------------------------------------------------------------------
class EProfileDoesNotHaveMimicPanel : public Sysutils::Exception{
	public:
		EProfileDoesNotHaveMimicPanel() : Sysutils::Exception("������� �� ��������� ����������. ���������� � ��������������."){};
};

//---------------------------------------------------------------------------
class ETypeCast : public Sysutils::Exception{
	public:
		ETypeCast(String msg) : Sysutils::Exception("������ �������������� ���� ������: " + msg){};
};

//---------------------------------------------------------------------------
class EWrongDisplayType : public Sysutils::Exception{
	public:
		EWrongDisplayType(String msg) : Sysutils::Exception("������ ����������� ������: " + msg){};
};

//---------------------------------------------------------------------------
class EUpdateDataManagerDataSourceNotAvailable : public Sysutils::Exception{
	public:
		EUpdateDataManagerDataSourceNotAvailable(String msg) : Sysutils::Exception("������ ��������� ������: " + msg){};
};

//---------------------------------------------------------------------------
class EFileDataManagerDataSourceNotAvailable : public EUpdateDataManagerDataSourceNotAvailable{
	public:
		EFileDataManagerDataSourceNotAvailable(String msg) : EUpdateDataManagerDataSourceNotAvailable("�������� ���� �� �������� ���� ������ ��� ��������� ���� �� ��������: " + msg){};
};

//---------------------------------------------------------------------------
class EHTTPDataNotRecive : public EUpdateDataManagerDataSourceNotAvailable{
	public:
		EHTTPDataNotRecive(String msg) : EUpdateDataManagerDataSourceNotAvailable("����������� ������ �� HTTP ���������: " + msg){};
};

//---------------------------------------------------------------------------
class EHTTPVersionNotRecive : public EUpdateDataManagerDataSourceNotAvailable{
	public:
		EHTTPVersionNotRecive(String msg) : EUpdateDataManagerDataSourceNotAvailable("����������� ����� ������ � HTTP ������: " + msg){};
};

//---------------------------------------------------------------------------
class EUnknownAlarmDataType : public Sysutils::Exception{
	public:
		EUnknownAlarmDataType(String msg) : Sysutils::Exception("����������� ��� ������� " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionColumnDoesNotHaveSortedFunction : public Sysutils::Exception{
	public:
		TExceptionColumnDoesNotHaveSortedFunction(int colIndex) : Sysutils::Exception("��� ������� �� ��������� ������� ����������, �������: " + IntToStr(colIndex)){};
};

//---------------------------------------------------------------------------
class TExceptionRecordViewChartDoesNotHaveSeries : public Sysutils::Exception{
	public:
	  TExceptionRecordViewChartDoesNotHaveSeries(String msg) : Sysutils::Exception("�� ������ ����� ����� �� ������� " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionRecordViewChartDoesNotHaveRequiredSeries : public Sysutils::Exception{
	public:
	  TExceptionRecordViewChartDoesNotHaveRequiredSeries(String msg) : Sysutils::Exception("�� ������ ����������� ����� ����� �� ������� " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionRecordViewChartDoesNotHaveSensor : public Sysutils::Exception{
	public:
	  TExceptionRecordViewChartDoesNotHaveSensor(String msg) : Sysutils::Exception("����������� ������ �� ������, ����������� ��� ����������� ������� " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionRecordViewChartDataIsNull : public Sysutils::Exception{
	public:
	  TExceptionRecordViewChartDataIsNull(String msg) : Sysutils::Exception("�� ������ ��������� �������� ������ ��� ����������� �� �� ������� " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionWrongAlarmEventType : public Sysutils::Exception {
	public:
	  TExceptionWrongAlarmEventType(String msg) : Sysutils::Exception("�������� ��� ������� " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionEventsJsonFileNotFound : public Sysutils::Exception {
	public:
		TExceptionEventsJsonFileNotFound(String msg) : Sysutils::Exception("���� events.json �� ������ " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionOffsetNotFoundInEventsJson : public Sysutils::Exception {
	public:
		TExceptionOffsetNotFoundInEventsJson(String msg) : Sysutils::Exception("�� ������ ���� ������� � ����� events.json " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionUnknownDeviceType : public Sysutils::Exception {
	public:
	  TExceptionUnknownDeviceType(String msg) : Sysutils::Exception("����������� ��� ���������� " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionCantParseEventsJsonFile : public Sysutils::Exception {
	public:
	  TExceptionCantParseEventsJsonFile(String msg) : Sysutils::Exception("�� ������� ���������� json ���� ������� " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionInvalidRecordType : public Sysutils::Exception {
	public:
	  TExceptionInvalidRecordType(String msg) : Sysutils::Exception("������������ ��� ������ " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionNoDBConnection : public Sysutils::Exception {
	public:
	  TExceptionNoDBConnection(String msg) : Sysutils::Exception("����������� ����������� � �� " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionUndefineSystemSetting : public Sysutils::Exception {
	public:
	  TExceptionUndefineSystemSetting(String msg) : Sysutils::Exception("����������� ��������� �������� " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionFileNotFound : public Sysutils::Exception {
	public:
	  TExceptionFileNotFound(String msg) : Sysutils::Exception("���� �� ������ " + msg){}
};
#endif
