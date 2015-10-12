//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "SensorViewEditBit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TSensorViewEditBit::TSensorViewEditBit(TWinControl *parent, TSensorBit *sensorBit) : TRecordViewEdit(parent, sensorBit){
}

//---------------------------------------------------------------------------
void TSensorViewEditBit::CreateControls() {
	TRecordViewEdit::CreateControls();
	/*
	//TSensorBit *sensorBit = dynamic_cast<TSensorBit *>(record);
	//if (!sensorBit) return;

	TSensorView::CreateControls();
	//viewControl = (TControl *)sysVCL::FindControl(parent, sensorBit->uuid);
	if (!viewControl) {
		viewControl = new TEdit(parent);
		viewControl->Name = sensorBit->uuid;
		viewControl->Parent = parent;
		viewControl->Left = 300;
		viewControl->Top = 300;
	}

	TLabel *leftLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_LEFT_PREFIX + sensorBit->uuid);
	if (!leftLabel) {
		leftLabel = new TLabel(parent);
		leftLabel->Parent = parent;
		leftLabel->Name = LABEL_LEFT_PREFIX + sensorBit->uuid;
		leftLabel->Caption = sensorBit->desc;
		leftLabel->AutoSize = true;
		leftLabel->Transparent = true;
		leftLabel->Alignment = taRightJustify;
		leftLabel->Left = viewControl->Left - leftLabel->Width - 5;
		leftLabel->Top = viewControl->Top + (viewControl->Height - leftLabel->Height) / 2;
	}

	TLabel *rightLabel = (TLabel *)sysVCL::FindControl(parent, LABEL_RIGHT_PREFIX + sensorBit->uuid);
	if (!rightLabel) {
		rightLabel = new TLabel(parent);
		rightLabel->Parent = parent;
		rightLabel->Name = LABEL_RIGHT_PREFIX + sensorBit->uuid;
		rightLabel->Caption = "";
		rightLabel->AutoSize = true;
		rightLabel->Transparent = true;
		rightLabel->Alignment = taLeftJustify;
		rightLabel->Left = viewControl->Left + viewControl->Width + 5;
		rightLabel->Top = viewControl->Top + (viewControl->Height - rightLabel->Height) / 2;
	}


	if (std::find(controls.begin(), controls.end(), viewControl) == controls.end()) {
		controls.push_back(viewControl);
	}

	if (std::find(controls.begin(), controls.end(), leftLabel) == controls.end()) {
		controls.push_back(leftLabel);
	}

	if (std::find(controls.begin(), controls.end(), rightLabel) == controls.end()) {
		controls.push_back(rightLabel);
	}
	*/
}

//---------------------------------------------------------------------------
void TSensorViewEditBit::DisplayData(TSensorData *data) {
	if (data != NULL) {
		DisplayText(&SensorBitDataToString(static_cast<const TSensorBit *>(record), data));
	} else {
		DisplayText(NULL);
	}

	/*
	TEdit *edit = (TEdit *)viewControl;

	if (!data) {
		edit->Text = "";
		return;
	}
	String displayResult;

	if (sensor->data_type == DATA_TYPE_CHAR) {
		std::bitset<8> bits(((TDataFile<char> *)data)->data);
		if (bits.test(sensorBit->bit)) displayResult = EmptyTo(sensorBit->value1, "1");
		else displayResult = EmptyTo(sensorBit->value0, "0");

	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_CHAR) {
		std::bitset<8> bits(((TDataFile<unsigned char> *)data)->data);
		if (bits.test(sensorBit->bit)) displayResult = EmptyTo(sensorBit->value1, "1");
		else displayResult = EmptyTo(sensorBit->value0, "0");

	} else if (sensor->data_type == DATA_TYPE_SHORT) {
		std::bitset<16> bits(((TDataFile<short> *)data)->data);
		if (bits.test(sensorBit->bit)) displayResult = EmptyTo(sensorBit->value1, "1");
		else displayResult = EmptyTo(sensorBit->value0, "0");

	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_SHORT) {
		std::bitset<16> bits(((TDataFile<unsigned short> *)data)->data);
		if (bits.test(sensorBit->bit)) displayResult = EmptyTo(sensorBit->value1, "1");
		else displayResult = EmptyTo(sensorBit->value0, "0");

	} else if (sensor->data_type == DATA_TYPE_INT) {
		std::bitset<32> bits(((TDataFile<int> *)data)->data);
		if (bits.test(sensorBit->bit)) displayResult = EmptyTo(sensorBit->value1, "1");
		else displayResult = EmptyTo(sensorBit->value0, "0");

	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT) {
		std::bitset<32> bits(((TDataFile<unsigned int> *)data)->data);
		if (bits.test(sensorBit->bit)) displayResult = EmptyTo(sensorBit->value1, "1");
		else displayResult = EmptyTo(sensorBit->value0, "0");

	} else if (sensor->data_type == DATA_TYPE_DOUBLE) {
		throw EWrongDisplayType("Ќеверное отображение бита данных значени€ double");

	} else if (sensor->data_type == DATA_TYPE_FLOAT) {
		throw EWrongDisplayType("Ќеверное отображение бита данных значени€ float");

	} else if (sensor->data_type == DATA_TYPE_INT64) {
		std::bitset<64> bits(((TDataFile<__int64> *)data)->data);
		if (bits.test(sensorBit->bit)) displayResult = EmptyTo(sensorBit->value1, "1");
		else displayResult = EmptyTo(sensorBit->value0, "0");

	} else if (sensor->data_type == DATA_TYPE_UNSIGNED_INT64) {
		std::bitset<64> bits(((TDataFile<unsigned __int64> *)data)->data);
		if (bits.test(sensorBit->bit)) displayResult = EmptyTo(sensorBit->value1, "1");
		else displayResult = EmptyTo(sensorBit->value0, "0");
	} else if (sensor->data_type == DATA_TYPE_STRING) {
		throw EWrongDisplayType("Ќеверное отображение бита данных значени€ string");
	}

	edit->Text = displayResult;
	*/
}

void TSensorViewEditBit::DisplayData(std::list<TSensorData *> *data) {
	if (data != NULL && data->size() != 0) {
		DisplayText(&SensorBitDataToString(static_cast<const TSensorBit *>(record), data->back()));
	} else {
		DisplayText(NULL);
	}
}
