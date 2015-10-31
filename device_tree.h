//---------------------------------------------------------------------------

#ifndef device_treeH
#define device_treeH

#include "record.h"
//---------------------------------------------------------------------------

struct TUpdateTreeViewOptions {
	unsigned display_sensor_bit: 1;
	unsigned display_empty_nodes: 1;
	String sFirespecGroups;
	String sDefaultPrecision;
};

extern TUpdateTreeViewOptions UPDATE_TREE_VIEW_OPTIONS;

void FilterTreeEx(
	TTreeView *TreeViewDevice,
	TTreeView *TreeViewDeviceFiltered,
	const String &filter);

void FilterTree(
	TTreeView *TreeViewDevice,
	TTreeView *TreeViewDeviceFiltered,
	const String &filter);

void BuildTreeViewDevice(TTreeView *TreeViewDevice);

//find record in tree view device node data, return index of node or -1 if not found
int FindRecordInTreeViewDevice(TTreeView *TreeViewDevice, const TRecord *record);


#endif
