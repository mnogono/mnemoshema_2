//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop

#include "string_grid_events_observer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TStringGridEventsObserver::TStringGridEventsObserver() {
}

void TStringGridEventsObserver::SetStringGrid(TStringGridDatasource<TDatasourceObjectEvent> *stringGrid) {
	this->stringGrid = stringGrid;
}

String GetEventTypeSetpointText(const TSensor *sensor, TSensorEventData *event){
	TStringBuilder *eventName = new TStringBuilder();

	if (event->compareType == event::TSensorEventCompareType::LT ||
		event->compareType == event::TSensorEventCompareType::LE) {
		eventName->Append(" < ");

	} else if (event->compareType == event::TSensorEventCompareType::GT ||
			   event->compareType == event::TSensorEventCompareType::GE) {
		eventName->Append(" > ");
	}

	if (event->dataType/*sensor->alarm_type*/ == event::INT64) {
		eventName->Append(event->i64Value);

	} else if (event->dataType/*sensor->alarm_type*/ == event::DOUBLE) {
		eventName->Append(event->dValue);

	} else if (event->dataType/*sensor->alarm_type*/ == event::STRING) {
		eventName->Append(event->strValue);
	}

	if (sensor->power != POWER_0 || sensor->unit != UNIT_UNKNOWN) {
		eventName->Append(" ")->Append(sensor->powerPrefix)->Append(sensor->unitName);
	}

	String result = eventName->ToString();

	delete eventName;
	/*
	TStringBuilder *eventName = new TStringBuilder(sensor->name);
	try {
		char eventTypeFirst;
		char eventTypeSecond;

		GetEventTypeBits(baseAlarm->eventType, &eventTypeFirst, &eventTypeSecond);

		//–егестрируем (показываем) событи€ которые не удоалетвор€ют уставкам

		if (eventTypeSecond == EVENT_TYPE_BIT) {
			if (baseAlarm->compareType == COMPARE_TYPE_LT ||
				baseAlarm->compareType == COMPARE_TYPE_LE) {
					eventName->Append(" < ");
			} else if (baseAlarm->compareType == COMPARE_TYPE_GT ||
				baseAlarm->compareType == COMPARE_TYPE_GE) {
					eventName->Append(" > ");
			}
			// eventName->Append("установленной уставки ");

			double alarmData = baseAlarm->toDouble();// alaram already in power  * sensor->powerValue;
			static char buffAlarmData[255];
			static char format[10];
			sprintf(format, "%%.%df", sensor->precision);
			sprintf(buffAlarmData, format, alarmData);

			eventName->Append(buffAlarmData);

			if (sensor->power != POWER_0 || sensor->unit != UNIT_) {
				eventName->Append(" ")->Append(sensor->powerPrefix)->Append(sensor->unitName);
			}

		} else {
			eventName->Append(EVENT_TYPE_NAME[eventTypeSecond]);
		}

		return eventName->ToString();

	} __finally {
		delete eventName;
	}
	*/

	return result;
}

void TStringGridEventsObserver::NewJsonExternalEvent(
	TTaskRequestDeviceEventDataFileFormat *request,
	const TDevice *device,
	const TSensor *sensor,
	TSensorEventData *sensorEventData) {

	TDateTime dtLocalTime;
	dtLocalTime.Val = sysTime::ConvertToDaysLocalTime(sensorEventData->timeGMT * sysTime::MSEC2SEC);

	TDatasourceObjectEvent oEvent;
	oEvent.device = request->device;
	oEvent.sensor = sensor;
	oEvent.datetime = dtLocalTime.FormatString("dd.mmm hh:nn:ss");
	oEvent.event = "—обытие";
	oEvent.dtLocalTime = dtLocalTime.Val;
	oEvent.jsonExternalData = sensorEventData->buffer;

	if (device != NULL) {
		oEvent.sensorName = device->GetName() + " "+ sensor->GetName();
	} else {
		oEvent.sensorName = sensor->GetName();
	}

	try {
		TJSONObject *jsonData = static_cast<TJSONObject *>(TJSONObject::ParseJSONValue(oEvent.jsonExternalData));
		String file1 = jsonData->GetValue(L"file1")->Value();

		jsonData->Free();

		oEvent.event = "—обытие: " + file1;
	} catch (Exception &e) {
		sysLogger::ERR_W(e.Message.c_str());
		sysLogger::ERR_W(oEvent.jsonExternalData.c_str());
	}

	stringGrid->datasource->Add(oEvent);
}

void TStringGridEventsObserver::UpdateData(sysObserverable::IObserverable *data) {
	if (data->GetObserverableType() != DEVICE_FILE_EVENT_DATA_FORMAT) {
		return;
	}

	TTaskRequestDeviceEventDataFileFormat *request = static_cast<TTaskRequestDeviceEventDataFileFormat *>(data);

	//remove datasource object correspond with request device
	for (int i = stringGrid->datasource->Size() - 1; i >= 0; --i) {
		TDatasourceObjectEvent &oEvent = (*stringGrid->datasource)[i];
		if (oEvent.device != request->device) {
			continue;
		}

		stringGrid->datasource->Remove(i);
	}

	//create and add new datasource object correspond with request device
	static double fLocalTimeBiasInDays = sysTime::GetLocalTimeBias() * sysTime::SEC2DAY;

	std::map<const TSensor *, std::list<TSensorEventData *> *> &d = request->data;
	std::map<const TSensor *, std::list<TSensorEventData *> *>::iterator it = d.begin(), itEnd = d.end();

	TDateTime dtLocalTime;

	for (; it != itEnd; ++it) {
		const TDevice *device = GetDevice(it->first->device_id);

		//find external event data START || CONTINUE
		for (std::list<TSensorEventData *>::iterator i = it->second->begin(), iEnd = it->second->end(); i != iEnd; ++i) {
			if ((*i)->eventType1 == event::TSensorEventType::RECORD_START) {
				NewJsonExternalEvent(
					request,
					device,
					it->first,
					*i);
			}
		}

		//skip all other events (ccustomers want is :( )
		continue;

		//find set point event
		std::list<TSensorEventData *>::reverse_iterator ri = it->second->rbegin(), riEnd = it->second->rend();
		for (; ri != riEnd; ++ri) {
			if ((*ri)->eventType1 == event::TSensorEventType::SETPOINT) {
				//check if setpoint is out of bounds
				if ((*ri)->compareResult == event::TSensorEventCompareResult::YES) {
					dtLocalTime.Val = sysTime::ConvertToDaysLocalTime((*ri)->timeGMT * sysTime::MSEC2SEC);

					TDatasourceObjectEvent oEvent;
					oEvent.device = request->device;
					oEvent.sensor = it->first;
					oEvent.datetime = dtLocalTime.FormatString("dd.mmm hh:nn::ss");
					oEvent.event = GetEventTypeSetpointText(it->first, (*ri)) + " YES " + IntToStr((*ri)->eventType0) + ":" + IntToStr((*ri)->eventType1);
					oEvent.dtLocalTime = dtLocalTime.Val;

					if (device != NULL) {
						oEvent.sensorName = device->GetName() + " "+ it->first->GetName();
					} else {
						oEvent.sensorName = it->first->GetName();
					}

					stringGrid->datasource->Add(oEvent);
				} else {
					dtLocalTime.Val = sysTime::ConvertToDaysLocalTime((*ri)->timeGMT * sysTime::MSEC2SEC);

					TDatasourceObjectEvent oEvent;
					oEvent.device = request->device;
					oEvent.sensor = it->first;
					oEvent.datetime = dtLocalTime.FormatString("dd.mmm hh:nn::ss");
					oEvent.event = GetEventTypeSetpointText(it->first, (*ri)) + " NO " + IntToStr((*ri)->eventType0) + ":" + IntToStr((*ri)->eventType1);
					oEvent.dtLocalTime = dtLocalTime.Val;

					if (device != NULL) {
						oEvent.sensorName = device->GetName() + " "+ it->first->GetName();
					} else {
						oEvent.sensorName = it->first->GetName();
					}

					stringGrid->datasource->Add(oEvent);
				}
			} else if ((*ri)->eventType1 == event::TSensorEventType::SENSOR_ONLINE) {
					TDatasourceObjectEvent oEvent;
					oEvent.device = request->device;
					oEvent.sensor = it->first;
					oEvent.datetime = dtLocalTime.FormatString("dd.mmm hh:nn::ss");
					oEvent.event = "Sensor online";
					oEvent.dtLocalTime = dtLocalTime.Val;

					stringGrid->datasource->Add(oEvent);
			} else if ((*ri)->eventType1 == event::TSensorEventType::SENSOR_OFFLINE) {
					TDatasourceObjectEvent oEvent;
					oEvent.device = request->device;
					oEvent.sensor = it->first;
					oEvent.datetime = dtLocalTime.FormatString("dd.mmm hh:nn::ss");
					oEvent.event = "Sensor offline";
					oEvent.dtLocalTime = dtLocalTime.Val;

					stringGrid->datasource->Add(oEvent);
			}  else if ((*ri)->eventType1 == event::TSensorEventType::UNKNOWN) {
					TDatasourceObjectEvent oEvent;
					oEvent.device = request->device;
					oEvent.sensor = it->first;
					oEvent.datetime = dtLocalTime.FormatString("dd.mmm hh:nn::ss");
					oEvent.event = "Sensor UNKNOWN";
					oEvent.dtLocalTime = dtLocalTime.Val;

					stringGrid->datasource->Add(oEvent);
            } else if ((*ri)->eventType1 == event::TSensorEventType::UNKNOWN) {
					TDatasourceObjectEvent oEvent;
					oEvent.device = request->device;
					oEvent.sensor = it->first;
					oEvent.datetime = dtLocalTime.FormatString("dd.mmm hh:nn::ss");
					oEvent.event = "Sensor EXTERNAL";
					oEvent.dtLocalTime = dtLocalTime.Val;

					stringGrid->datasource->Add(oEvent);
			}
		}
	}

	stringGrid->Clear();

	stringGrid->Build();

	//update data source of the string grid

	//create rows in string grid and fill the data
	/*
	std::map<const TSensor *, std::list<TSensorEventData *> *> &d = request->data;
	std::map<const TSensor *, std::list<TSensorEventData *> *>::iterator id = d.begin(), idEnd = d.end();

	TDateTime dtLocalTime;

	//d - sorted by date time
	//just get the last event data when alarm was initiated
	unsigned char eventType;
	for (; id != idEnd; ++id) {
		const TSensor *sensor = id->first;
		std::list<TSensorEventData *>::reverse_iterator ri = id->second->rbegin(), riEnd = id->second->rend();
		for (; ri != riEnd; ++ri) {
			eventType = (*ri)->eventType;
			if ((eventType & 0xF) == EVENT_TYPE_ALARM_CONDITION) {
				dtLocalTime.Val = sysTime::ConvertToDaysLocalTime((*ri)->timeGMT * sysTime::MSEC2SEC);

				stringGrid->Cells[0][stringGrid->RowCount - 1] = dtLocalTime.FormatString("dd.mm hh:nn");
				stringGrid->Cells[1][stringGrid->RowCount - 1] = sensor->GetName();
				stringGrid->Cells[2][stringGrid->RowCount - 1] = "";

				++stringGrid->RowCount;
				break;
			}
		}
	}
	*/
}
