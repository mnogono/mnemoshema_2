//---------------------------------------------------------------------------
#ifndef datasource_objectH
#define datasource_objectH
//---------------------------------------------------------------------------

struct TDatasourceObject {
	virtual String & Value(size_t index) = 0;
};

#endif
