//---------------------------------------------------------------------------

#ifndef recordInfoH
#define recordInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include "tree_view_observer.h"
//---------------------------------------------------------------------------

//disable warning about all interface should be inherted from IUnknown
#pragma option push -w-8130

class TFormRecordInfo :
	public TForm,
	public ITreeViewEvents
{
__published:	// IDE-managed Components
	TPageControl *PageControlPanels;
	TTabSheet *TabSheet1;
	TPanel *PanelCollectionPoint;
	TPanel *PanelClient;
	TLabel *Label2;
	TLabel *Label4;
	TEdit *EditCollectionPointName;
	TEdit *EditCollectionPointAddr;
	TPanel *PanelTop;
	TLabel *Label51;
	TTabSheet *TabSheetElementPosition;
	TPanel *PanelMimicElementProperty;
	TPanel *Panel13;
	TLabel *Label56;
	TLabel *Label57;
	TLabel *Label58;
	TLabel *Label59;
	TEdit *EditMimicElementX;
	TEdit *EditMimicElementY;
	TEdit *EditMimicElementWidth;
	TEdit *EditMimicElementHeight;
	TPanel *Panel18;
	TLabel *Label47;
	TTabSheet *TabSheetDevice;
	TPanel *PanelDevice;
	TPanel *Panel2;
	TLabel *Label16;
	TLabel *Label17;
	TLabel *Label18;
	TEdit *EditDeviceName;
	TComboBox *ComboBoxDeviceCollectionPoint;
	TCheckBox *CheckBoxDeviceEmulate;
	TButton *ButtonDeviceAdditional;
	TEdit *EditDeviceDescription;
	TPanel *Panel4;
	TLabel *Label52;
	TTabSheet *TabSheetDeviceLinx;
	TPanel *PanelDeviceRSLinxSettingAdditional;
	TLabel *Label62;
	TLabel *Label63;
	TLabel *Label64;
	TLabel *Label70;
	TLabel *Label72;
	TEdit *EditDeviceRSLinxPeriodMS;
	TEdit *EditDeviceRSLinxProtocol;
	TEdit *EditDeviceRSLinxTimeoutMS;
	TEdit *EditDeviceRSLinxType;
	TEdit *EditDeviceRSLinxUUID;
	TTabSheet *TabSheet8;
	TPanel *PanelDeviceSettingAdditional;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TEdit *EditDeviceUUID;
	TEdit *EditDeviceType;
	TComboBox *ComboBoxDeviceProtocol;
	TEdit *EditDevicePort;
	TComboBox *ComboBoxDeviceBaudrate;
	TComboBox *ComboBoxDeviceDataBit;
	TComboBox *ComboBoxDeviceParity;
	TComboBox *ComboBoxDeviceStopBit;
	TEdit *EditDevicePeriodMS;
	TEdit *EditDeviceTimeoutMS;
	TEdit *EditDeviceDevAddr;
	TTabSheet *TabSheet2;
	TPanel *PanelDeviceBrizSettingAdditional;
	TLabel *Label22;
	TLabel *Label23;
	TLabel *Label24;
	TLabel *Label25;
	TLabel *Label26;
	TLabel *Label29;
	TLabel *Label36;
	TLabel *Label38;
	TLabel *Label40;
	TEdit *EditDeviceBrizAddr;
	TEdit *EditDeviceBrizPass;
	TEdit *EditDeviceBrizPeriodMS;
	TEdit *EditDeviceBrizPort;
	TEdit *EditDeviceBrizProtocol;
	TEdit *EditDeviceBrizTimeoutMS;
	TEdit *EditDeviceBrizType;
	TEdit *EditDeviceBrizUser;
	TEdit *EditDeviceBrizUUID;
	TTabSheet *TabSheet7;
	TPanel *PanelSensorBriz;
	TPanel *Panel22;
	TLabel *Label67;
	TComboBox *ComboBoxSensorBrizMode;
	TTabSheet *TabSheet3;
	TPanel *PanelSensor;
	TPanel *PanelSensorSettingAdditional;
	TLabel *Label19;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label27;
	TLabel *Label28;
	TLabel *Label41;
	TEdit *EditSensorUUID;
	TEdit *EditSensorAlarmSize;
	TEdit *EditSensorIndex;
	TEdit *EditSensorSize;
	TComboBox *ComboBoxSensorDataType;
	TComboBox *ComboBoxAlarmType;
	TPanel *Panel8;
	TLabel *Label30;
	TLabel *Label31;
	TLabel *Label32;
	TLabel *Label33;
	TLabel *Label34;
	TLabel *Label35;
	TLabel *Label46;
	TEdit *EditSensorName;
	TComboBox *ComboBoxSensorUnit;
	TButton *ButtonSensorAdditional;
	TEdit *EditSensorDescription;
	TComboBox *ComboBoxSensorPower;
	TEdit *EditSensorMinValue;
	TEdit *EditSensorMaxValue;
	TComboBox *ComboBoxSensorPrecision;
	TPanel *Panel9;
	TLabel *Label53;
	TTabSheet *TabSheet4;
	TPanel *PanelSensorBit;
	TPanel *Panel16;
	TLabel *Label49;
	TLabel *Label39;
	TEdit *EditSensorBitSettingDesc;
	TComboBox *ComboBoxSensorBitView;
	TPanel *Panel17;
	TLabel *Label55;
	TPanel *PanelSensorBitImage;
	TLabel *Label44;
	TLabel *Label45;
	TImage *ImageSensorBitValue0;
	TImage *ImageSensorBitValue1;
	TButton *ButtonSetSensorBitValue0;
	TButton *ButtonSetSensorBitValue1;
	TComboBox *ComboBoxSensorBitValue0;
	TComboBox *ComboBoxSensorBitValue1;
	TRadioButton *RadioButtonSensorBitValue0;
	TRadioButton *RadioButtonSensorBitValue1;
	TPanel *PanelSensorBitText;
	TLabel *Label42;
	TLabel *Label43;
	TEdit *EditSensorBitValue0;
	TEdit *EditSensorBitValue1;
	TTabSheet *TabSheet5;
	TPanel *PanelMnemoshema;
	TPanel *Panel7;
	TLabel *Label37;
	TImage *ImageMnemoshema;
	TEdit *EditMnemoshemaName;
	TButton *Button1;
	TPanel *Panel12;
	TLabel *Label54;
	TTabSheet *TabSheet6;
	TPanel *PanelMimicElementView;
	TPanel *Panel14;
	TLabel *Label48;
	TLabel *Label50;
	TEdit *EditMimicPanelElementLabelLeft;
	TEdit *EditMimicPanelElementLabelRight;
	TPanel *Panel15;
	TLabel *Label60;
	TLabel *Label1;
	TEdit *EditSensorDevice;
	TLabel *Label3;
	TEdit *EditSensorRemotePoint;
	TLabel *Label61;
	TEdit *EditGaussFilter;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall EditGaussFilterChange(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
private:	// User declarations

    void __fastcall ShowSensor();

	void __fastcall ShowDevice();

public:		// User declarations
	//record for display
	TRecord *record;

    TPanel *currentPanel;

	__fastcall TFormRecordInfo(TComponent* Owner);

    void __fastcall SetRecord(TRecord *record);

    void __fastcall InitComboBoxSensorUnit();

    void __fastcall InitComboBoxSensorPower();

	void __fastcall InitComboBoxSensorPrecision();

	void __fastcall InitComboBoxDeviceCollectionPoint();

    void __fastcall DeleteComboBoxStringObjectElements(TComboBox *comboBox);

    void __fastcall UserLogin(TUser *User);

	void __fastcall SetInvisiblePanels();

	virtual void OnTreeViewNodeChange(TTreeNode *node);

	void __fastcall RestoreLastUserSettings();

	void __fastcall SaveLastUserSettings();
};

//---------------------------------------------------------------------------
extern PACKAGE TFormRecordInfo *FormRecordInfo;
//---------------------------------------------------------------------------

#pragma option pop

#endif
