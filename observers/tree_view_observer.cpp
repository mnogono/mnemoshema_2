//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "tree_view_observer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TTreeViewDeviceObserver::TTreeViewDeviceObserver(TTreeView *treeView, ITreeViewEvents *treeViewEvents) {
    this->treeView = treeView;

	this->treeViewEvents = treeViewEvents;
}

void TTreeViewDeviceObserver::UpdateData() {

}

