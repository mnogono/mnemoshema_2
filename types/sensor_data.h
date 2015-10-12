//---------------------------------------------------------------------------

#ifndef sensor_dataH
#define sensor_dataH
//---------------------------------------------------------------------------

class TSensorData : public sysPatterns::IPooledObject{
	public:
		//string value size
		size_t size;

		//date-time in msec since 1970
		__int64 timeGMT;

		union {
			char cValue;

			unsigned char ucValue;

			short sValue;

			unsigned short usValue;

			int iValue;

			unsigned int uiValue;

			long lValue;

			double dValue;

			float fValue;

			__int64  i64Value;

			unsigned __int64 ui64Value;

			//strin value
			char *strValue;
		};
	public:
		TSensorData();

		virtual void CleanUp();
};

#endif
