//---------------------------------------------------------------------------
#ifndef exceptionsH
#define exceptionsH
//---------------------------------------------------------------------------
class EProfileDoesNotHaveMimicPanel : public Sysutils::Exception{
	public:
		EProfileDoesNotHaveMimicPanel() : Sysutils::Exception("Профилю не назначена мнемосхема. Обратитесь к администратору."){};
};

//---------------------------------------------------------------------------
class ETypeCast : public Sysutils::Exception{
	public:
		ETypeCast(String msg) : Sysutils::Exception("Ошибка преобразования типа данных: " + msg){};
};

//---------------------------------------------------------------------------
class EWrongDisplayType : public Sysutils::Exception{
	public:
		EWrongDisplayType(String msg) : Sysutils::Exception("Ошибка отображения данных: " + msg){};
};

//---------------------------------------------------------------------------
class EUpdateDataManagerDataSourceNotAvailable : public Sysutils::Exception{
	public:
		EUpdateDataManagerDataSourceNotAvailable(String msg) : Sysutils::Exception("Ошибка получения данных: " + msg){};
};

//---------------------------------------------------------------------------
class EFileDataManagerDataSourceNotAvailable : public EUpdateDataManagerDataSourceNotAvailable{
	public:
		EFileDataManagerDataSourceNotAvailable(String msg) : EUpdateDataManagerDataSourceNotAvailable("Неверный путь до файловой базы данных или удаленный путь не доступен: " + msg){};
};

//---------------------------------------------------------------------------
class EHTTPDataNotRecive : public EUpdateDataManagerDataSourceNotAvailable{
	public:
		EHTTPDataNotRecive(String msg) : EUpdateDataManagerDataSourceNotAvailable("Отсутствует данные по HTTP протоколу: " + msg){};
};

//---------------------------------------------------------------------------
class EHTTPVersionNotRecive : public EUpdateDataManagerDataSourceNotAvailable{
	public:
		EHTTPVersionNotRecive(String msg) : EUpdateDataManagerDataSourceNotAvailable("Отсутствует номер версии в HTTP ответе: " + msg){};
};

//---------------------------------------------------------------------------
class EUnknownAlarmDataType : public Sysutils::Exception{
	public:
		EUnknownAlarmDataType(String msg) : Sysutils::Exception("Неизвестный тип событий " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionColumnDoesNotHaveSortedFunction : public Sysutils::Exception{
	public:
		TExceptionColumnDoesNotHaveSortedFunction(int colIndex) : Sysutils::Exception("Для колонки не назначена функция сортировки, колонка: " + IntToStr(colIndex)){};
};

//---------------------------------------------------------------------------
class TExceptionRecordViewChartDoesNotHaveSeries : public Sysutils::Exception{
	public:
	  TExceptionRecordViewChartDoesNotHaveSeries(String msg) : Sysutils::Exception("Не заданы серии линий на графике " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionRecordViewChartDoesNotHaveRequiredSeries : public Sysutils::Exception{
	public:
	  TExceptionRecordViewChartDoesNotHaveRequiredSeries(String msg) : Sysutils::Exception("Не заданы необходимые серии линий на графике " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionRecordViewChartDoesNotHaveSensor : public Sysutils::Exception{
	public:
	  TExceptionRecordViewChartDoesNotHaveSensor(String msg) : Sysutils::Exception("Отсутствует ссылка на датчик, необходимый для отображения графика " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionRecordViewChartDataIsNull : public Sysutils::Exception{
	public:
	  TExceptionRecordViewChartDataIsNull(String msg) : Sysutils::Exception("Не создан контейнер хранения данных для отображения их на графике " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionWrongAlarmEventType : public Sysutils::Exception {
	public:
	  TExceptionWrongAlarmEventType(String msg) : Sysutils::Exception("Неверный тип события " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionEventsJsonFileNotFound : public Sysutils::Exception {
	public:
		TExceptionEventsJsonFileNotFound(String msg) : Sysutils::Exception("Файл events.json не найден " + msg){};
};

//---------------------------------------------------------------------------
class TExceptionOffsetNotFoundInEventsJson : public Sysutils::Exception {
	public:
		TExceptionOffsetNotFoundInEventsJson(String msg) : Sysutils::Exception("Не найден ключ события в файле events.json " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionUnknownDeviceType : public Sysutils::Exception {
	public:
	  TExceptionUnknownDeviceType(String msg) : Sysutils::Exception("Неизвестный тип устройства " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionCantParseEventsJsonFile : public Sysutils::Exception {
	public:
	  TExceptionCantParseEventsJsonFile(String msg) : Sysutils::Exception("Не удалось распарсить json файл событий " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionInvalidRecordType : public Sysutils::Exception {
	public:
	  TExceptionInvalidRecordType(String msg) : Sysutils::Exception("недопустимый тип записи " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionNoDBConnection : public Sysutils::Exception {
	public:
	  TExceptionNoDBConnection(String msg) : Sysutils::Exception("отсутствует подключение к БД " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionUndefineSystemSetting : public Sysutils::Exception {
	public:
	  TExceptionUndefineSystemSetting(String msg) : Sysutils::Exception("отсутствует системный параметр " + msg){}
};

//---------------------------------------------------------------------------
class TExceptionFileNotFound : public Sysutils::Exception {
	public:
	  TExceptionFileNotFound(String msg) : Sysutils::Exception("файл не найден " + msg){}
};
#endif
