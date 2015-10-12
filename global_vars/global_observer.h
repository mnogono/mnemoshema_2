//---------------------------------------------------------------------------
#ifndef global_observerH
#define global_observerH
//---------------------------------------------------------------------------

class TGlobalObserver : public sysObserver::IObserver {
	virtual void UpdateData(sysObserverable::IObserverable *data = NULL) = 0;
};

#endif
