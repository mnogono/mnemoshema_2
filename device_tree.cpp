//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "device_tree.h"

TUpdateTreeViewOptions UPDATE_TREE_VIEW_OPTIONS;

// ------------------------------------------------------------------------------
static bool compare(TTreeNode *node, void *data) {
	TDeviceTreeGroup *deviceTreeGroup = (TDeviceTreeGroup *)data;
	TRecord *record = (TRecord *)node->Data;
	if (!record) {
		return false;
	}
	return deviceTreeGroup->parent_uuid == record->uuid;
}

// ------------------------------------------------------------------------------
static void BuildTreeViewFiresecDevice(
	TTreeView *TreeViewDevice,
	const TDevice *device,
	TTreeNode *dataPointNode){

	sysLogger::TRACE_A("Updating firespec device tree...");

	//device->parentNode = dataPointNode;
	TTreeNode *deviceNode = TreeViewDevice->Items->AddChild(dataPointNode, device->name);
	deviceNode->ImageIndex = -1;
	deviceNode->SelectedIndex = -1;

	deviceNode->Data = (void *)device;
	//device->node = deviceNode;
	//vector<TSensor *> &sensors = User->sensors;
	std::list<TSensor *> &sensors = SENSORS;
	String sensorName;
	String nodeName;

	//String defaultFirespecGroups = String(L"АМ-1П,АМ-1Т,ППУ,НПТ");
	String defaultFirespecGroups = String(L"");
	//String sFirespecGroups = EmptyTo(DataModuleMP->GetSetting("firesec_groups"), defaultFirespecGroups);
	String sFirespecGroups = sysBaseTypes::EmptyTo(UPDATE_TREE_VIEW_OPTIONS.sFirespecGroups, defaultFirespecGroups);

	std::vector<std::wstring> firespecGroups;
	sysStr::Split(sFirespecGroups.c_str(), L",", firespecGroups);

	//create all firespec group into tree view device
	std::map<std::wstring, TTreeNode *> groupNode;

	for (std::vector<std::wstring>::iterator itGroup = firespecGroups.begin(); itGroup != firespecGroups.end(); ++itGroup) {
		TTreeNode *node = TreeViewDevice->Items->AddChild(deviceNode, itGroup->c_str());
		groupNode[itGroup->c_str()] = node;
	}

	for (std::list<TSensor *>::iterator si = sensors.begin(); si != sensors.end(); si++){
		if ((*si)->device_id != device->id){
			continue;
		}

		//check if sensor name can be attached into one of firespec group
		TTreeNode *node = NULL;
		TTreeNode *sensorNode = NULL;

		for (std::vector<std::wstring>::iterator itGroup = firespecGroups.begin(); itGroup != firespecGroups.end(); ++itGroup) {
			if (wcsstr((*si)->name.c_str(), itGroup->c_str()) != NULL) {
				node = groupNode[*itGroup];
				break;
			}
		}

		if (node) {
			sensorNode = TreeViewDevice->Items->AddChild(node, (*si)->name);
		} else {
			sensorNode = TreeViewDevice->Items->AddChild(deviceNode, (*si)->name);
		}

		sensorNode->Data = *si;
	}
}

// ------------------------------------------------------------------------------
static void BuildTreeViewRSLinxDevice(
	TTreeView *TreeViewDevice,
	const TDevice *device,
	TTreeNode *dataPointNode){

	sysLogger::TRACE_A("Updating rslinx device tree view...");

	TTreeNode *deviceNode = TreeViewDevice->Items->AddChild(dataPointNode, device->name);
	deviceNode->ImageIndex = -1;
	deviceNode->SelectedIndex = -1;

	deviceNode->Data = (void *)device;
	std::list<TSensor *> &sensors = SENSORS;
	String sensorName;
	std::vector<std::wstring> groups;
	String nodeName;

	for (std::list<TSensor *>::iterator si = sensors.begin(); si != sensors.end(); si++){
		if ((*si)->device_id == device->id){
			sensorName = (*si)->name;

			groups.clear();
			sysStr::Split(sensorName.c_str(), L".", groups);

			TTreeNode *parent = deviceNode;
			TTreeNode *child;

			for (std::size_t itGroup = 0; itGroup < groups.size() - 1; ++itGroup){
				String groupName = groups[itGroup].c_str();

				child = sysVCL::FindChildTreeNodeByNodeName(TreeViewDevice, groupName, parent);
				if (!child){
					parent = TreeViewDevice->Items->AddChild(parent, groupName);
				} else {
					parent = child;
				}
			}

			nodeName = groups[groups.size() - 1].c_str();

			TTreeNode *sensorNode = TreeViewDevice->Items->AddChild(parent, nodeName);
			sensorNode->ImageIndex = -1;
			sensorNode->SelectedIndex = -1;

			sensorNode->Data = *si;
		}
	}
}

//---------------------------------------------------------------------------
void FilterTreeEx(TTreeView *TreeViewDevice,
	TTreeView *TreeViewDeviceFiltered,
	const String &filter){

	if (filter.IsEmpty()) {
		TreeViewDevice->Visible = true;
		TreeViewDeviceFiltered->Visible = false;
		return;
	}

	TreeViewDeviceFiltered->Items->Clear();

	String search = filter.LowerCase();
    String checkName;
	for (int itItem = 0; itItem < TreeViewDevice->Items->Count; ++itItem) {
    	checkName = TreeViewDevice->Items->Item[itItem]->Text;
        if (checkName.LowerCase().Pos(search) <= 0) continue;

		TTreeNode *node = TreeViewDeviceFiltered->Items->AddChild(TreeViewDevice->Items->Item[itItem]->Parent, checkName);
		node->Data = TreeViewDevice->Items->Item[itItem]->Data;
	}

	if (!TreeViewDeviceFiltered->Items->Count) {
    	TreeViewDeviceFiltered->Items->Add(NULL, "Датчик не найден");
	} else {
	    TreeViewDeviceFiltered->Items->Item[0]->Expand(true);
	}

	TreeViewDeviceFiltered->Visible = true;
	TreeViewDevice->Visible = false;
}

//---------------------------------------------------------------------------
void FilterTree(TTreeView *TreeViewDevice,
	TTreeView *TreeViewDeviceFiltered,
	const String &filter) {

	if (filter.IsEmpty()) {
		TreeViewDevice->Visible = true;
		TreeViewDeviceFiltered->Visible = false;
		return;
	}

	TreeViewDeviceFiltered->Items->Clear();

	/* key - record, value added node */
    std::map<TRecord *, TTreeNode *> addedNodes;

	String search = filter.LowerCase();
    String checkName;
	for (int itItem = 0; itItem < TreeViewDevice->Items->Count; ++itItem) {
		TTreeNode *sensorNode = TreeViewDevice->Items->Item[itItem];

    	if (!sensorNode->Data) {
        	//for rslinx deive, device tree can be different structure
            //TODO need to be fixed in next update
			continue;
        }

		TRecord *recordSensor = static_cast<TRecord*>(sensorNode->Data);
		if (recordSensor->record_type != RECORD_TYPE_SENSOR) continue;

		TTreeNode *nodeDevice = sensorNode->Parent;

        if (!nodeDevice->Data) {
        	//for rslinx deive, device tree can be different structure
            //TODO need to be fixed in next update
			continue;
        }

		TRecord *recordDevice = static_cast<TRecord *>(nodeDevice->Data);
		if (recordDevice->record_type != RECORD_TYPE_DEVICE) continue;

		TDevice *device = static_cast<TDevice *>(nodeDevice->Data);

		TSensor *sensor = static_cast<TSensor*>(recordSensor);
		//if (sensor->name.LowerCase().Pos(search) <= 0) continue;
        if (sensorNode->Text.LowerCase().Pos(search) <= 0) continue;

		TTreeNode *nodeCollectionPoint = nodeDevice->Parent;
		TRecord *recordCollectionPoint = static_cast<TRecord *>(nodeCollectionPoint->Data);
		if (recordCollectionPoint->record_type != RECORD_TYPE_COLLECTION_POINT) continue;

		TCollectionPoint *collectionPoint = static_cast<TCollectionPoint *>(recordCollectionPoint);

		if (addedNodes.find(collectionPoint) == addedNodes.end()) {
			TTreeNode *node = TreeViewDeviceFiltered->Items->Add(NULL, collectionPoint->title);
			node->Data = collectionPoint;
			addedNodes[collectionPoint] = node;
		}
		if (addedNodes.find(device) == addedNodes.end()) {
			TTreeNode *node = TreeViewDeviceFiltered->Items->AddChild(addedNodes[collectionPoint], device->name);
			node->Data = device;
			addedNodes[device] = node;
		}
		TTreeNode *node = TreeViewDeviceFiltered->Items->AddChild(addedNodes[device], sensorNode->Text/*sensor->name*/);
		node->Data = sensor;
	}

	if (!TreeViewDeviceFiltered->Items->Count) {
    	TreeViewDeviceFiltered->Items->Add(NULL, "Датчик не найден");
	} else {
	    TreeViewDeviceFiltered->Items->Item[0]->Expand(true);
	}

	TreeViewDeviceFiltered->Visible = true;
	TreeViewDevice->Visible = false;
}

// ------------------------------------------------------------------------------
void __BuildTreeViewDevice(TTreeView *TreeViewDevice){
	sysLogger::TRACE_A("Building tree view device...");

	/* remember when you clear selection function TreeViewDeviceChange will be invoked */
	/* it is good to be clear selection before delete elements */
	TreeViewDevice->ClearSelection();

	/* remember when you clear items selection function TreeViewDeviceChange will be invoked */
	TreeViewDevice->Items->Clear();

	std::list<TCollectionPoint *> &collectionPoints = COLLECTION_POINTS;
	std::list<TDevice *> &devices = DEVICES;
	std::list<TSensor *> &sensors = SENSORS;
	std::list<TSensorBit *> &sensor_bits = SENSORS_BITS;

	TTreeNode *dataPointNode = NULL;
	TTreeNode *deviceNode = NULL;
	TTreeNode *sensorNode = NULL;
	TTreeNode *sensorbitNode = NULL;

    bool doNotDisplayDiscreteSensorNode = true;

	for (std::list<TCollectionPoint *>::iterator dpi = collectionPoints.begin(); dpi != collectionPoints.end(); ++dpi){
		dataPointNode = TreeViewDevice->Items->Add(NULL, (*dpi)->title);
		dataPointNode->ImageIndex = -1;
		dataPointNode->SelectedIndex = -1;

		dataPointNode->Data = *dpi;

		for (std::list<TDevice *>::iterator di = devices.begin(); di != devices.end(); ++di){
			if ((*di)->collection_point_id == (*dpi)->id){
				if ((*di)->type == "firesec") {
					BuildTreeViewFiresecDevice(TreeViewDevice, *di, dataPointNode);
				} else if ((*di)->type == "rslinx"){
					BuildTreeViewRSLinxDevice(TreeViewDevice, *di, dataPointNode);

				} else {
					deviceNode = TreeViewDevice->Items->AddChild(dataPointNode, (*di)->name);
					deviceNode->ImageIndex = -1;
					deviceNode->SelectedIndex = -1;

					deviceNode->Data = *di;

					for (std::list<TSensor *>::iterator si = sensors.begin(); si != sensors.end(); si++){
						//do not display sensor if it has some device tree group
						if ((*si)->deviceTreeGroupId) {
							continue;
						}

						if ((*si)->device_id == (*di)->id){
							String sensorTitle = (*si)->GetName();

							if (doNotDisplayDiscreteSensorNode) {
								//check if sensor have descrete bits, do not add sensor node
								bool isSensorHasBits = false;
								if (UPDATE_TREE_VIEW_OPTIONS.display_sensor_bit == 1) {
									for (std::list<TSensorBit *>::iterator bi = sensor_bits.begin(); bi != sensor_bits.end(); bi++){
										if ((*bi)->sensor_id == (*si)->id){
											sensorbitNode = TreeViewDevice->Items->AddChild(deviceNode, IntToStr((*bi)->bit) + " (" + (*bi)->desc + ")");
											sensorbitNode->ImageIndex = -1;
											sensorbitNode->SelectedIndex = -1;

											sensorbitNode->Data = *bi;

											isSensorHasBits = true;
										}
									}
								}
								if (!isSensorHasBits) {
                                    sensorNode = TreeViewDevice->Items->AddChild(deviceNode, sensorTitle);
                                    sensorNode->ImageIndex = -1;
                                    sensorNode->SelectedIndex = -1;

									sensorNode->Data = *si;
								}
							} else {
								sensorNode = TreeViewDevice->Items->AddChild(deviceNode, sensorTitle);
								sensorNode->ImageIndex = -1;
								sensorNode->SelectedIndex = -1;

								sensorNode->Data = *si;

								if (UPDATE_TREE_VIEW_OPTIONS.display_sensor_bit == 1) {
									for (std::list<TSensorBit *>::iterator bi = sensor_bits.begin(); bi != sensor_bits.end(); bi++){
										if ((*bi)->sensor_id == (*si)->id){
											sensorbitNode = TreeViewDevice->Items->AddChild(sensorNode, IntToStr((*bi)->bit) + " (" + (*bi)->desc + ")");
											sensorbitNode->ImageIndex = -1;
											sensorbitNode->SelectedIndex = -1;

											sensorbitNode->Data = *bi;
										}
									}
								}
                            }
						}
					}
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------
void BuildTreeViewDevice(TTreeView *TreeViewDevice) {
	sysLogger::TRACE_A("Building tree view device with device tree groups...");

	/* remember when you clear selection function TreeViewDeviceChange will be invoked */
	/* it is good to be clear selection before delete elements */
	TreeViewDevice->ClearSelection();

	/* remember when you clear items selection function TreeViewDeviceChange will be invoked */
	TreeViewDevice->Items->Clear();

	std::list<TDeviceTreeGroup *> &device_tree_groups = DEVICE_TREE_GROUPS;

	//build tree view device as usual
	__BuildTreeViewDevice(TreeViewDevice);

	String name;
	//append new nodes as device groups
	for (std::list<TDeviceTreeGroup *>::iterator dgi = device_tree_groups.begin(); dgi != device_tree_groups.end(); ++dgi) {
		TDeviceTreeGroup *deviceTreeGroup = *dgi;

		//append root elements
		if (deviceTreeGroup->parent_uuid.IsEmpty()) {
			TTreeNode *node = TreeViewDevice->Items->Add(NULL, deviceTreeGroup->name);
			node->Data = (void *)deviceTreeGroup;
			continue;
		}

		TRecord *record = NULL;
		void *compareData = *dgi;
		TTreeNode *parentNode = sysVCL::FindTreeNodeByCompareFunction(TreeViewDevice, compareData, compare);
		if (!parentNode) {
			continue;
		}

		if (deviceTreeGroup->record_type == TRecordType::RECORD_TYPE_SENSOR) {
			TSensor *sensor = GetSensor(deviceTreeGroup->uuid);
			name = sensor->name;
			record = static_cast<TRecord *>(sensor);
		} else if (deviceTreeGroup->record_type == TRecordType::RECORD_TYPE_DEVICE_TREE_GROUP) {
			name = deviceTreeGroup->name;
			record = deviceTreeGroup;
		}

		if (!record) {
			continue;
		}

		TTreeNode *node = TreeViewDevice->Items->AddChild(parentNode, name);
		node->Data = record;
	}

	//expand collection points device tree nodes

	for (int i = 0, count = TreeViewDevice->Items->Count; i < count; ++i) {
		TTreeNode *node = TreeViewDevice->Items->Item[i];
		if (node->Data == NULL) {
			continue;
		}

		TRecord *record = (TRecord *)(node->Data);
		if (record->record_type == RECORD_TYPE_COLLECTION_POINT) {
			node->Expand(false);
		}
	}
}

// ------------------------------------------------------------------------------
int FindRecordInTreeViewDevice(TTreeView *TreeViewDevice, const TRecord *record) {
	for (int itItem = 0; itItem < TreeViewDevice->Items->Count; ++itItem) {
		TTreeNode *sensorNode = TreeViewDevice->Items->Item[itItem];

		if (!sensorNode->Data) {
			continue;
		}

		if (static_cast<TRecord*>(sensorNode->Data) == record) {
            return itItem;
        }
	}

	return -1;
}
