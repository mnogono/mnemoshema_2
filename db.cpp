//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "db.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TUser & LoadUser(const String &login, const String &password) {
	sysLogger::TRACE_A("Loading user record...");

	//mark as undefine user in db
	USER.id = -1;

	try {
		DataModuleMP->ADOQuery->SQL->Text = "select * from user where login = '" + login + "' and password = '" + password + "'";
		DataModuleMP->ADOQuery->Open();

		sysLogger::DEBUG_W(DataModuleMP->ADOQuery->SQL->Text.c_str());

		if (DataModuleMP->ADOQuery->RecordCount > 0){

			USER.id =  DataModuleMP->ADOQuery->Fields->FieldByName("id")->AsInteger;
			USER.login = DataModuleMP->ADOQuery->Fields->FieldByName("login")->AsString;
			USER.profile_id = DataModuleMP->ADOQuery->Fields->FieldByName("profile_id")->AsInteger;
			USER.group_id = DataModuleMP->ADOQuery->Fields->FieldByName("group_id")->AsInteger;
			USER.description = DataModuleMP->ADOQuery->Fields->FieldByName("description")->AsString;
			USER.settings = DataModuleMP->ADOQuery->Fields->FieldByName("settings")->AsString;
			USER.is_admin = DataModuleMP->ADOQuery->Fields->FieldByName("is_admin")->AsInteger == 1;
		}
	} catch (Exception &e){
		sysLogger::DEBUG_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	DataModuleMP->ADOQuery->Close();

	return USER;
}

//---------------------------------------------------------------------------
std::list<TCollectionPoint *> & LoadCollectionPoints(const TUser &user) {
	sysLogger::TRACE_A("Loading collection points for user...");

	for (std::list<TCollectionPoint *>::iterator it = COLLECTION_POINTS.begin(), itEnd = COLLECTION_POINTS.end(); it != itEnd; ++it) {
		delete *it;
	}
	COLLECTION_POINTS.clear();

	try {
		String sql = "SELECT cp.*, cpp.value AS signal_file_max_size FROM collection_point AS cp"
			" LEFT JOIN collection_point_parameters AS cpp ON (cp.id = cpp.collection_point_id AND cpp.name = 'signal_file_max_size')";
		if (!user.is_admin) {
			sql += " WHERE cp.id IN (SELECT DISTINCT collection_point_id FROM profile_sensor"
				   " LEFT JOIN device_sensor ON device_sensor_id = device_sensor.id"
				   " LEFT JOIN device ON device_sensor.device_id = device.id"
				   " WHERE profile_id = " + IntToStr(user.profile_id) + ")";
		}

        DataModuleMP->ADOQuery->SQL->Text = sql;

		sysLogger::DEBUG_W(DataModuleMP->ADOQuery->SQL->Text.c_str());

		DataModuleMP->ADOQuery->Open();

		for (int iCollectionPoint = 0, dataPointsCount = DataModuleMP->ADOQuery->RecordCount; iCollectionPoint < dataPointsCount;
		iCollectionPoint++ , DataModuleMP->ADOQuery->Next()){
			TCollectionPoint *collectionPoint = new TCollectionPoint();
			collectionPoint->id = DataModuleMP->ADOQuery->Fields->FieldByName("id")->AsInteger;
			collectionPoint->title = DataModuleMP->ADOQuery->Fields->FieldByName("title")->AsString;
			collectionPoint->addr = DataModuleMP->ADOQuery->Fields->FieldByName("addr")->AsString;
			collectionPoint->description = DataModuleMP->ADOQuery->Fields->FieldByName("description")->AsString;
			collectionPoint->uuid = DataModuleMP->ADOQuery->Fields->FieldByName("uuid")->AsString;

			COLLECTION_POINTS.push_back(collectionPoint);

			RECORDS[collectionPoint->uuid] = COLLECTION_POINTS.back();
		}
	} catch (Exception &e){
		sysLogger::DEBUG_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	DataModuleMP->ADOQuery->Close();

	return COLLECTION_POINTS;
}

//---------------------------------------------------------------------------
std::list<TDevice *> & LoadDevices(const TUser &user) {
	sysLogger::TRACE_A("Loading devices for user...");

	for (std::list<TDevice *>::iterator it = DEVICES.begin(), itEnd = DEVICES.end(); it != itEnd; ++it) {
		delete *it;
	}
	DEVICES.clear();

	try {
		String sql =
			"SELECT d.*, "
			"dp_name.value AS `name`, "
			"dp_type.value AS `type`, "
			"CASE WHEN dp_rate.value IS NULL THEN 5000 ELSE dp_rate.value END AS `rate`, "
			"CASE WHEN dp_data_block_size.value IS NULL THEN 0 ELSE dp_data_block_size.value END AS `data_block_size` "
			"FROM device d "
			"LEFT JOIN device_parameters dp_rate ON dp_rate.device_id = d.id AND dp_rate.`name` = 'reqPeriodMs' "
			"LEFT JOIN device_parameters dp_name ON dp_name.device_id = d.id AND dp_name.`name` = 'name' "
			"LEFT JOIN device_parameters dp_type ON dp_type.device_id = d.id AND dp_type.`name` = 'type' "
			"LEFT JOIN device_parameters dp_data_block_size ON dp_data_block_size.device_id = d.id AND dp_data_block_size.`name` = 'data_block_size' ";
		if (!user.is_admin){
			sql += " WHERE d.id in (SELECT distinct device_sensor.device_id"
				   " FROM  profile_sensor LEFT JOIN device_sensor ON device_sensor_id = device_sensor.id"
			       " where profile_id=" + IntToStr(user.profile_id) + ")";
		}

		DataModuleMP->ADOQuery->SQL->Text = sql;

		sysLogger::DEBUG_W(DataModuleMP->ADOQuery->SQL->Text.c_str());

		DataModuleMP->ADOQuery->Open();

		for (int iDevice = 0, deviceCount = DataModuleMP->ADOQuery->RecordCount; iDevice < deviceCount; iDevice++ , DataModuleMP->ADOQuery->Next()){
			TDevice *device = new TDevice();
			device->id = DataModuleMP->ADOQuery->Fields->FieldByName("id")->AsInteger;
			device->uuid = DataModuleMP->ADOQuery->Fields->FieldByName("uuid")->AsString;
			device->collection_point_id = DataModuleMP->ADOQuery->Fields->FieldByName("collection_point_id")->AsInteger;
			device->name = DataModuleMP->ADOQuery->Fields->FieldByName("name")->AsString;
			device->type = DataModuleMP->ADOQuery->Fields->FieldByName("type")->AsString;
			device->rate_msec = DataModuleMP->ADOQuery->Fields->FieldByName("rate")->AsInteger;
			device->data_block_size = DataModuleMP->ADOQuery->Fields->FieldByName("data_block_size")->AsInteger;

			DEVICES.push_back(device);

			RECORDS[device->uuid] = DEVICES.back();
		}

	} catch (Exception &e){
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	int dataFileFormat = DataModuleMP->GetSettingInt("data_file_format");
	if (dataFileFormat == DATA_FILE_FORMAT_DEVICE) {
		//if the new device file format is enabled, read the size of all sensors of the device
		for (std::list<TDevice *>::iterator i = DEVICES.begin(), iEnd = DEVICES.end(); i != iEnd; ++i) {
			try {
				TDevice *device = *i;

				DataModuleMP->ADOQuery->SQL->Text =
					"SELECT "
					"ds.id as id, "
					"uuid, "
					"dsp_data_type.value as data_type, "
					"dsp_size.value as size, "
					"CAST(dsp_index.value as UNSIGNED INTEGER) as `index` "
					"FROM device_sensor AS ds "
					"LEFT JOIN device_sensor_parameters AS dsp_data_type ON dsp_data_type.sensor_id = ds.id AND dsp_data_type.name = 'data_type' "
					"LEFT JOIN device_sensor_parameters AS dsp_size ON dsp_size.sensor_id = ds.id AND dsp_size.name = 'size' "
					"LEFT JOIN device_sensor_parameters as dsp_index ON dsp_index.sensor_id = ds.id AND dsp_index.name = 'index' "
					"WHERE ds.device_id = " + IntToStr(device->id) +
					" ORDER BY `index`";

				sysLogger::DEBUG_W(DataModuleMP->ADOQuery->SQL->Text.c_str());

				DataModuleMP->ADOQuery->Open();

				int sensorsCount = DataModuleMP->ADOQuery->RecordCount;
				device->sensorsInfo.resize(sensorsCount);

				for (int itSensor = 0; itSensor < sensorsCount; ++itSensor) {
					int id       = DataModuleMP->ADOQuery->Fields->FieldByName("id")->AsInteger;
					int dataType = DataModuleMP->ADOQuery->Fields->FieldByName("data_type")->AsInteger;
					int size     = DataModuleMP->ADOQuery->Fields->FieldByName("size")->AsInteger;
					int index    = DataModuleMP->ADOQuery->Fields->FieldByName("index")->AsInteger;
					String uuid  = DataModuleMP->ADOQuery->Fields->FieldByName("uuid")->AsString;

					TDevice::TSensorInfo *sensorInfo = new TDevice::TSensorInfo();
					sensorInfo->dataType = dataType;
					sensorInfo->size = size;
					sensorInfo->sensorDataSize = TSensor::GetDataSize(dataType, size) - sizeof(long);
					sensorInfo->index = index;
					sensorInfo->uuid = uuid;
					sensorInfo->sensor = GetSensor(id);

					device->sensorsInfo[index] = sensorInfo;

					DataModuleMP->ADOQuery->Next();
				}
			} catch (Exception &e) {
				sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
				sysLogger::ERR_W(e.Message.c_str());
			}
		}
	}

	DataModuleMP->ADOQuery->Close();

	return DEVICES;
}

/*
void UpdateDeviceDataBlockSize(std::list<TDevice *> &devices) {
	try {
		int sensorsCount;
		int sensorDataType;
		int sensorSize;
		size_t deviceBlockSize;

		for (std::list<TDevice *>::iterator i = devices.begin(), iEnd = devices.end(); i != iEnd; ++i) {
			TDevice *device = *i;

			String sql =
				"SELECT dsp_data_type.value AS data_type, "
				"dsp_size.value AS size "
				"FROM device_sensor AS ds "
				"LEFT JOIN device_parameters dsp_data_type ON dsp_data_type.sensor_id = ds.id AND dsp_data_type.`name` = 'data_type' "
				"LEFT JOIN device_parameters dsp_size ON dsp_size.sensor_id = ds.id AND dsp_size.`name` = 'size' "
				"WHERE device_id = " + IntToStr(device->id);

			DataModuleMP->ADOQuery->SQL->Text = sql;

			deviceBlockSize = 0;
			sensorsCount = DataModuleMP->ADOQuery->RecordCount;
			for (int si = 0; si < sensorsCount; ++si) {
				sensorDataType = DataModuleMP->ADOQuery->Fields->FieldByName("data_type")->AsInteger;
				sensorSize     = DataModuleMP->ADOQuery->Fields->FieldByName("size")->AsInteger;

				DataModuleMP->ADOQuery->Next();
			}
		}
	} catch (Exception &e) {
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	DataModuleMP->ADOQuery->Close();
}
*/

//---------------------------------------------------------------------------
std::list<TSensor *> & LoadSensors(const TUser &user) {
	sysLogger::TRACE_A("Loading sensors for user...");

	for (std::list<TSensor *>::iterator it = SENSORS.begin(), itEnd = SENSORS.end(); it != itEnd; ++it) {
		delete *it;
	}
	SENSORS.clear();

	String defaultPrecision = sysBaseTypes::EmptyTo(DataModuleMP->GetSetting("default_precision"), "4");

	try {
		String sql =
			"SELECT ds.*, "
			"dsp_name.value as name, "
			"dsp_data_type.value as data_type, "
			"dsp_size.value as size, "
			"CAST(dsp_index.value as UNSIGNED INTEGER) as `index`, "
			"CASE WHEN dsp_rate.value IS NULL THEN CASE WHEN dp_rate.value IS NULL THEN 0 ELSE dp_rate.value END END as rate, "
			"CASE WHEN dsp_desc.value IS NULL THEN '' ELSE dsp_desc.value END as `desc`, "
			"(SELECT COUNT(id) FROM sensor_bit WHERE sensor_id = ds.id) as bit_count, "
			"CASE WHEN dsp_max.value IS NULL THEN (CASE WHEN dsp_maxeq.value IS NULL THEN '-999' ELSE dsp_maxeq.value END) ELSE dsp_max.value END as `max_val`, "
			"CASE WHEN dsp_min.value IS NULL THEN (CASE WHEN dsp_mineq.value IS NULL THEN '-999' ELSE dsp_mineq.value END) ELSE dsp_min.value END as `min_val`, "
			"CASE WHEN dsp_alarm_type.value IS NULL THEN "+IntToStr(ALARM_DATA_TYPE_DOUBLE)+" ELSE dsp_alarm_type.value END as alarm_type, "
			"CASE WHEN dsp_alarm_size.value IS NULL THEN 0 ELSE dsp_alarm_size.value END as alarm_size, " "CASE WHEN dsp_precision.value IS NULL THEN "+defaultPrecision+" ELSE dsp_precision.value END as `precision`, "
			"CASE WHEN dsp_power.value IS NULL THEN " + IntToStr(POWER_0) + " ELSE dsp_power.value END as `power`, "
			"CASE WHEN dsp_unit.value IS NULL THEN " + IntToStr(UNIT_UNKNOWN) + " ELSE dsp_unit.value END as `unit`, "
			"CASE WHEN dsp_unit_mode.value IS NULL THEN " + IntToStr(TBrizAnalogMode::amdNone) + " ELSE dsp_unit_mode.value END as `unit_mode`, "
			"device_tree_group.id as dtg_id "
			"FROM device_sensor AS ds "
			"LEFT JOIN device_sensor_parameters AS dsp_name ON dsp_name.sensor_id = ds.id AND dsp_name.name = 'name' "
			"LEFT JOIN device_sensor_parameters AS dsp_data_type ON dsp_data_type.sensor_id = ds.id AND dsp_data_type.name = 'data_type' "
			"LEFT JOIN device_sensor_parameters AS dsp_size ON dsp_size.sensor_id = ds.id AND dsp_size.name = 'size' "
			"LEFT JOIN device_sensor_parameters AS dsp_rate ON dsp_rate.sensor_id = ds.id AND dsp_rate.name = 'rate' "
			"LEFT JOIN device_parameters AS dp_rate ON dp_rate.device_id = ds.device_id AND dp_rate.name = 'reqPeriodMs' "
			"LEFT JOIN device_sensor_parameters AS dsp_desc ON dsp_desc.sensor_id = ds.id AND dsp_desc.name = 'desc' "
			"LEFT JOIN device_sensor_parameters as dsp_unit ON dsp_unit.sensor_id = ds.id AND dsp_unit.name = 'unit' "
			"LEFT JOIN device_sensor_parameters as dsp_power ON dsp_power.sensor_id = ds.id AND dsp_power.name = 'power' "
			"LEFT JOIN device_sensor_parameters as dsp_max ON dsp_max.sensor_id = ds.id AND dsp_max.name = 'gt' "
			"LEFT JOIN device_sensor_parameters as dsp_min ON dsp_min.sensor_id = ds.id AND dsp_min.name = 'lt' "
			"LEFT JOIN device_sensor_parameters as dsp_maxeq ON dsp_maxeq.sensor_id = ds.id AND dsp_maxeq.name = 'le' "
			"LEFT JOIN device_sensor_parameters as dsp_mineq ON dsp_mineq.sensor_id = ds.id AND dsp_mineq.name = 'ge' "
			"LEFT JOIN device_sensor_parameters as dsp_alarm_type ON dsp_alarm_type.sensor_id = ds.id AND dsp_alarm_type.name = 'alarm_type' "
			"LEFT JOIN device_sensor_parameters as dsp_alarm_size ON dsp_alarm_size.sensor_id = ds.id AND dsp_alarm_size.name = 'alarm_size' "
			"LEFT JOIN device_sensor_parameters as dsp_precision ON dsp_precision.sensor_id = ds.id AND dsp_precision.name = 'precision' "
			"LEFT JOIN device_sensor_parameters as dsp_unit_mode ON dsp_unit_mode.sensor_id = ds.id AND dsp_unit_mode.name = 'unit_mode' "
			"LEFT JOIN device_sensor_parameters as dsp_index ON dsp_index.sensor_id = ds.id AND dsp_index.name = 'index' "
			"LEFT JOIN device_tree_group ON device_tree_group.uuid = ds.uuid ";

		//for new device data file format, need to load all sensors into the memmory of client application
		//bacause need to known the size of all sensor data size while read new data file format
		//no way... it broke the profile restrict functionality...

		if (!user.is_admin){
			sql += " where ds.id in ( select device_sensor_id from profile_sensor where profile_id = " + IntToStr(user.profile_id) + " )";
		}
		sql += " order by ds.device_id, `index`";

		DataModuleMP->ADOQuery->SQL->Text = sql;

		sysLogger::DEBUG_W(DataModuleMP->ADOQuery->SQL->Text.c_str());

		DataModuleMP->ADOQuery->Open();

		int recordCount = DataModuleMP->ADOQuery->RecordCount;

		for (int iSensor = 0; iSensor < recordCount; iSensor++ , DataModuleMP->ADOQuery->Next()){
			TSensor *sensor = new TSensor();
			sensor->id = DataModuleMP->ADOQuery->Fields->FieldByName("id")->AsInteger;
			sensor->uuid = DataModuleMP->ADOQuery->Fields->FieldByName("uuid")->AsString;
			sensor->device_id = DataModuleMP->ADOQuery->Fields->FieldByName("device_id")->AsInteger;
			sensor->name = DataModuleMP->ADOQuery->Fields->FieldByName("name")->AsString;
			sensor->data_type = DataModuleMP->ADOQuery->Fields->FieldByName("data_type")->AsInteger;
			sensor->size = DataModuleMP->ADOQuery->Fields->FieldByName("size")->AsInteger;
			sensor->rate_msec = DataModuleMP->ADOQuery->Fields->FieldByName("rate")->AsInteger;
//			sensor.status = DataModuleMP->ADOQuery->Fields->FieldByName("status")->AsInteger;
			sensor->desc = DataModuleMP->ADOQuery->Fields->FieldByName("desc")->AsString;
			sensor->full_bit_count = DataModuleMP->ADOQuery->Fields->FieldByName("bit_count")->AsInteger;

			sensor->sensorBits.resize(sensor->full_bit_count);

			String sMaxValue = DataModuleMP->ADOQuery->Fields->FieldByName("max_val")->AsString;
			if (sMaxValue.IsEmpty() || sMaxValue == "null") {
				sensor->max_value = StrToFloatDef(sMaxValue, -999);
			}
			String sMinValue = DataModuleMP->ADOQuery->Fields->FieldByName("min_val")->AsString;
			if (sMinValue.IsEmpty() || sMinValue == "null") {
				sensor->min_value = StrToFloatDef(sMinValue, -999);
			}
			sensor->alarm_size = DataModuleMP->ADOQuery->Fields->FieldByName("alarm_size")->AsInteger;
			sensor->alarm_type = DataModuleMP->ADOQuery->Fields->FieldByName("alarm_type")->AsInteger;
			sensor->deviceTreeGroupId = DataModuleMP->ADOQuery->Fields->FieldByName("dtg_id")->AsInteger;

			if (sensor->alarm_size == 0 && sensor->alarm_type == ALARM_DATA_TYPE_STRING) {
				//by default using alarm_size 8 for string data type.... for current moment
				sensor->alarm_size = 8;
			}

			sensor->precision = DataModuleMP->ADOQuery->Fields->FieldByName("precision")->AsInteger;
			sensor->power = DataModuleMP->ADOQuery->Fields->FieldByName("power")->AsInteger;
			sensor->unit = DataModuleMP->ADOQuery->Fields->FieldByName("unit")->AsInteger;
			sensor->unit_mode = (TBrizAnalogMode)DataModuleMP->ADOQuery->Fields->FieldByName("unit_mode")->AsInteger;
			sensor->unitName = GetUnitName(sensor->unit);
			sensor->powerValue = GetPowerValue(sensor->power);
			sensor->powerPrefix = GetPowerPrefix(sensor->power);
			sensor->unitModeName = GetUnitModeName(sensor->unit_mode);
			sensor->index = DataModuleMP->ADOQuery->Fields->FieldByName("index")->AsInteger;

			SENSORS.push_back(sensor);

			RECORDS[sensor->uuid] = SENSORS.back();
		}
	} catch (Exception &e){
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	DataModuleMP->ADOQuery->Close();

	return SENSORS;
}

// ------------------------------------------------------------------------------
std::list<TSensorBit *> & LoadSensorsBits(const TUser & user){
	sysLogger::TRACE_A("Loading sensors bits for user...");

	for (std::list<TSensorBit *>::iterator it = SENSORS_BITS.begin(), itEnd = SENSORS_BITS.end(); it != itEnd; ++it) {
		delete *it;
	}
	SENSORS_BITS.clear();

	try {
		String sql = "select sensor_bit.*, device_sensor.uuid as sensor_uuid from sensor_bit left join device_sensor on sensor_bit.sensor_id=device_sensor.id ";
		if (!user.is_admin){ // условие на профиль
			sql += " where sensor_id in ( select device_sensor_id from profile_sensor where profile_id = " + IntToStr(user.profile_id) + " )";/* +
				   " and sensor_bit.id in (select sensor_bit_id from profile_sensor where profile_id=" + IntToStr(user.profile_id) + ")";*/
		}
		sql += " order by bit";

        DataModuleMP->ADOQuery->SQL->Text = sql;

		sysLogger::DEBUG_W(DataModuleMP->ADOQuery->SQL->Text.c_str());

		DataModuleMP->ADOQuery->Open();

		int bitsCount = DataModuleMP->ADOQuery->RecordCount;

		for (int iBit = 0; iBit < bitsCount; ++iBit){
			String sensorUuid = DataModuleMP->ADOQuery->Fields->FieldByName("sensor_uuid")->AsString;

			TSensorBit *sensorBit = new TSensorBit();
			sensorBit->id = DataModuleMP->ADOQuery->Fields->FieldByName("id")->AsInteger;
			sensorBit->value0 = DataModuleMP->ADOQuery->Fields->FieldByName("value0")->AsString;
			sensorBit->value1 = DataModuleMP->ADOQuery->Fields->FieldByName("value1")->AsString;
			sensorBit->desc = DataModuleMP->ADOQuery->Fields->FieldByName("desc")->AsString;
			sensorBit->uuid = sensorUuid + DataModuleMP->ADOQuery->Fields->FieldByName("bit")->AsString;
			sensorBit->sensor_id = DataModuleMP->ADOQuery->Fields->FieldByName("sensor_id")->AsInteger;
			sensorBit->bit = DataModuleMP->ADOQuery->Fields->FieldByName("bit")->AsInteger;
			sensorBit->view = DataModuleMP->ADOQuery->Fields->FieldByName("view")->AsString;
			if (sensorBit->view == "edit"){
				sensorBit->view = "text";
			}
			sensorBit->sensor = GetSensor(sensorUuid);

			//check is uuid is : "*_undefined" - remove this sensor bit from list
			if (sensorBit->uuid.Pos("undefined") != 0) {
				if (sensorBit->sensor != NULL) {
					const_cast<TSensor *>(sensorBit->sensor)->full_bit_count -= 1;
				}

				delete sensorBit;
			} else {
				SENSORS_BITS.push_back(sensorBit);

				RECORDS[sensorBit->uuid] = SENSORS_BITS.back();

				const_cast<TSensor *>(sensorBit->sensor)->SetSensorBit(sensorBit);
			}

			DataModuleMP->ADOQuery->Next();
		}
	} catch (Exception &e){
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	DataModuleMP->ADOQuery->Close();

	return SENSORS_BITS;
}

// ------------------------------------------------------------------------------
std::list<TDeviceTreeGroup *> & LoadDeviceTreeGroups(const TUser &User) {
	sysLogger::TRACE_A("Loading device tree groups for user...");

	for (std::list<TDeviceTreeGroup *>::iterator it = DEVICE_TREE_GROUPS.begin(), itEnd = DEVICE_TREE_GROUPS.end(); it != itEnd; ++it) {
		delete *it;
	}
	DEVICE_TREE_GROUPS.clear();

	DataModuleMP->ADOQuery->SQL->Text = "SELECT `id`, `name`, `uuid`, `parent_uuid`, `record_type` FROM device_tree_group";
	try {
		DataModuleMP->ADOQuery->Open();
		for (int itRecord = 0, len = DataModuleMP->ADOQuery->RecordCount; itRecord < len; ++itRecord) {
			TDeviceTreeGroup *deviceTreeGroup = new TDeviceTreeGroup();
			deviceTreeGroup->id = DataModuleMP->ADOQuery->Fields->FieldByName("id")->AsInteger;
			deviceTreeGroup->name = DataModuleMP->ADOQuery->Fields->FieldByName("name")->AsString;
			deviceTreeGroup->uuid = DataModuleMP->ADOQuery->Fields->FieldByName("uuid")->AsAnsiString;
			deviceTreeGroup->parent_uuid = DataModuleMP->ADOQuery->Fields->FieldByName("parent_uuid")->AsString;
			deviceTreeGroup->record_type = DataModuleMP->ADOQuery->Fields->FieldByName("record_type")->AsInteger;

			DEVICE_TREE_GROUPS.push_back(deviceTreeGroup);

			RECORDS[deviceTreeGroup->uuid] = DEVICE_TREE_GROUPS.back();

			DataModuleMP->ADOQuery->Next();
		}
	} catch (Exception &e) {
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	DataModuleMP->ADOQuery->Close();

	return DEVICE_TREE_GROUPS;
}

// ------------------------------------------------------------------------------
std::list<TMnemoshema *> & LoadMnemoshems(const TUser &user){
	sysLogger::TRACE_A("Loading mnemoshems for user...");

	for (std::list<TMnemoshema *>::iterator it = MNEMOSHEMS.begin(), itEnd = MNEMOSHEMS.end(); it != itEnd; ++it) {
		delete *it;
	}
	MNEMOSHEMS.clear();

	DataModuleMP->ADOQuery->SQL->Text =
		"SELECT id, name, structure, uuid FROM mnemoshema.mnemoschema WHERE ID IN ( "
		"SELECT mnemoschema_id FROM profile_elem WHERE profile_id = " + String(user.profile_id) + ")";

	sysLogger::DEBUG_W(DataModuleMP->ADOQuery->SQL->Text.c_str());

	try {
		DataModuleMP->ADOQuery->Open();
		for (int itRecord = 0, len = DataModuleMP->ADOQuery->RecordCount; itRecord < len; ++itRecord) {
			TMnemoshema *mnemoshema = new TMnemoshema();
			mnemoshema->id = DataModuleMP->ADOQuery->Fields->FieldByName("id")->AsInteger;
			mnemoshema->uuid = DataModuleMP->ADOQuery->Fields->FieldByName("uuid")->AsString;
			mnemoshema->name = DataModuleMP->ADOQuery->Fields->FieldByName("name")->AsString;
			mnemoshema->structure = DataModuleMP->ADOQuery->Fields->FieldByName("structure")->AsString;

			MNEMOSHEMS.push_back(mnemoshema);

			RECORDS[mnemoshema->uuid] = MNEMOSHEMS.back();

			DataModuleMP->ADOQuery->Next();
		}
	} catch (Exception &e) {
		sysLogger::ERR_W(DataModuleMP->ADOQuery->SQL->Text.c_str());
		sysLogger::ERR_W(e.Message.c_str());
	}

	DataModuleMP->ADOQuery->Close();

	return MNEMOSHEMS;
}


