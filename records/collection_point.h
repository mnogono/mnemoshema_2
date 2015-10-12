//---------------------------------------------------------------------------
#ifndef collection_pointH
#define collection_pointH
//---------------------------------------------------------------------------

struct TCollectionPoint : public TRecord {
	String title;
	String addr;
	String description;

	TCollectionPoint();

	virtual String GetName() const;
};

#endif
