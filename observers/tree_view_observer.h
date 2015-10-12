//---------------------------------------------------------------------------
#ifndef tree_view_observerH
#define tree_view_observerH
//---------------------------------------------------------------------------

class ITreeViewEvents {
	virtual void OnTreeViewNodeChange(TTreeNode *node) = 0;
};

class TTreeViewDeviceObserver : public sysObserver::IObserver {
	private:
		//notify object
		ITreeViewEvents *treeViewEvents;

		//observerable model object
		TTreeView *treeView;
	public:
		TTreeViewDeviceObserver(TTreeView *treeView, ITreeViewEvents *treeViewEvents);

		virtual void UpdateData();
};

#endif
