object FormRecordInfo: TFormRecordInfo
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #1048#1085#1092#1086#1088#1084#1072#1094#1080#1103
  ClientHeight = 676
  ClientWidth = 704
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Visible = True
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControlPanels: TPageControl
    Left = 0
    Top = 0
    Width = 704
    Height = 377
    ActivePage = TabSheet3
    Align = alTop
    TabOrder = 0
    Visible = False
    object TabSheet1: TTabSheet
      Caption = #1058#1086#1095#1082#1072' '#1089#1073#1086#1088#1072
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PanelCollectionPoint: TPanel
        Left = 0
        Top = 0
        Width = 696
        Height = 90
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        Visible = False
        object PanelClient: TPanel
          Left = 0
          Top = 24
          Width = 696
          Height = 66
          Align = alClient
          TabOrder = 0
          object Label2: TLabel
            Left = 91
            Top = 9
            Width = 23
            Height = 13
            Caption = #1048#1084#1103':'
          end
          object Label4: TLabel
            Left = 79
            Top = 36
            Width = 35
            Height = 13
            Caption = #1040#1076#1088#1077#1089':'
          end
          object EditCollectionPointName: TEdit
            Left = 120
            Top = 6
            Width = 313
            Height = 21
            TabOrder = 0
          end
          object EditCollectionPointAddr: TEdit
            Left = 120
            Top = 33
            Width = 313
            Height = 21
            TabOrder = 1
          end
        end
        object PanelTop: TPanel
          Left = 0
          Top = 0
          Width = 696
          Height = 24
          Align = alTop
          Alignment = taLeftJustify
          TabOrder = 1
          object Label51: TLabel
            AlignWithMargins = True
            Left = 4
            Top = 4
            Width = 104
            Height = 13
            Align = alClient
            Caption = #1058#1086#1095#1082#1072' '#1089#1073#1086#1088#1072' '#1076#1072#1085#1085#1099#1093
          end
        end
      end
    end
    object TabSheetElementPosition: TTabSheet
      Caption = #1056#1072#1089#1087#1086#1083#1086#1078#1077#1085#1080#1077
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PanelMimicElementProperty: TPanel
        Left = 0
        Top = 0
        Width = 696
        Height = 90
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        Visible = False
        object Panel13: TPanel
          Left = 0
          Top = 24
          Width = 696
          Height = 66
          Align = alClient
          TabOrder = 0
          object Label56: TLabel
            Left = 115
            Top = 12
            Width = 10
            Height = 13
            Alignment = taRightJustify
            Caption = 'X:'
          end
          object Label57: TLabel
            Left = 320
            Top = 12
            Width = 10
            Height = 13
            Alignment = taRightJustify
            Caption = 'Y:'
          end
          object Label58: TLabel
            Left = 81
            Top = 39
            Width = 44
            Height = 13
            Alignment = taRightJustify
            Caption = #1064#1080#1088#1080#1085#1072':'
          end
          object Label59: TLabel
            Left = 289
            Top = 39
            Width = 41
            Height = 13
            Alignment = taRightJustify
            Caption = #1042#1099#1089#1086#1090#1072':'
          end
          object EditMimicElementX: TEdit
            Left = 131
            Top = 9
            Width = 100
            Height = 21
            NumbersOnly = True
            TabOrder = 0
          end
          object EditMimicElementY: TEdit
            Left = 336
            Top = 9
            Width = 100
            Height = 21
            NumbersOnly = True
            TabOrder = 1
          end
          object EditMimicElementWidth: TEdit
            Left = 131
            Top = 36
            Width = 100
            Height = 21
            NumbersOnly = True
            TabOrder = 2
          end
          object EditMimicElementHeight: TEdit
            Left = 336
            Top = 36
            Width = 100
            Height = 21
            NumbersOnly = True
            TabOrder = 3
          end
        end
        object Panel18: TPanel
          AlignWithMargins = True
          Left = 0
          Top = 0
          Width = 696
          Height = 24
          Margins.Left = 0
          Margins.Top = 0
          Margins.Right = 0
          Margins.Bottom = 0
          Align = alTop
          Alignment = taLeftJustify
          TabOrder = 1
          object Label47: TLabel
            AlignWithMargins = True
            Left = 4
            Top = 4
            Width = 73
            Height = 13
            Align = alClient
            Caption = #1056#1072#1089#1087#1086#1083#1086#1078#1077#1085#1080#1077
          end
        end
      end
    end
    object TabSheetDevice: TTabSheet
      Caption = #1059#1089#1090#1088#1086#1081#1089#1090#1074#1086
      ImageIndex = 2
      object PanelDevice: TPanel
        Left = 0
        Top = 0
        Width = 696
        Height = 132
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        Visible = False
        object Panel2: TPanel
          Left = 0
          Top = 24
          Width = 696
          Height = 108
          Align = alClient
          TabOrder = 0
          object Label16: TLabel
            Left = 102
            Top = 9
            Width = 23
            Height = 13
            Alignment = taRightJustify
            Caption = #1048#1084#1103':'
          end
          object Label17: TLabel
            Left = 59
            Top = 36
            Width = 66
            Height = 13
            Alignment = taRightJustify
            Caption = #1058#1086#1095#1082#1072' '#1089#1073#1086#1088#1072':'
          end
          object Label18: TLabel
            Left = 72
            Top = 63
            Width = 53
            Height = 13
            Alignment = taRightJustify
            Caption = #1054#1087#1080#1089#1072#1085#1080#1077':'
          end
          object EditDeviceName: TEdit
            Left = 127
            Top = 6
            Width = 305
            Height = 21
            ReadOnly = True
            TabOrder = 0
          end
          object ComboBoxDeviceCollectionPoint: TComboBox
            Left = 127
            Top = 33
            Width = 305
            Height = 21
            Style = csDropDownList
            TabOrder = 1
          end
          object CheckBoxDeviceEmulate: TCheckBox
            Left = 127
            Top = 87
            Width = 97
            Height = 17
            Caption = #1069#1084#1091#1083#1103#1094#1080#1103
            TabOrder = 2
          end
          object ButtonDeviceAdditional: TButton
            Left = 7
            Top = 110
            Width = 118
            Height = 25
            Caption = #1044#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1086
            TabOrder = 3
            Visible = False
          end
          object EditDeviceDescription: TEdit
            Left = 127
            Top = 60
            Width = 306
            Height = 21
            ReadOnly = True
            TabOrder = 4
          end
        end
        object Panel4: TPanel
          Left = 0
          Top = 0
          Width = 696
          Height = 24
          Align = alTop
          Alignment = taLeftJustify
          TabOrder = 1
          object Label52: TLabel
            AlignWithMargins = True
            Left = 4
            Top = 4
            Width = 59
            Height = 13
            Align = alClient
            Caption = #1059#1089#1090#1088#1086#1081#1089#1090#1074#1086
          end
        end
      end
    end
    object TabSheetDeviceLinx: TTabSheet
      Caption = #1059#1089#1090#1088#1086#1081#1089#1090#1074#1086' Linx'
      ImageIndex = 8
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PanelDeviceRSLinxSettingAdditional: TPanel
        Left = 0
        Top = 0
        Width = 696
        Height = 121
        Align = alTop
        TabOrder = 0
        Visible = False
        object Label62: TLabel
          Left = 96
          Top = 13
          Width = 29
          Height = 13
          Alignment = taRightJustify
          Caption = 'UUID:'
        end
        object Label63: TLabel
          Left = 308
          Top = 40
          Width = 22
          Height = 13
          Caption = #1058#1080#1087':'
        end
        object Label64: TLabel
          Left = 68
          Top = 40
          Width = 53
          Height = 13
          Alignment = taRightJustify
          Caption = #1055#1088#1086#1090#1086#1082#1086#1083':'
        end
        object Label70: TLabel
          Left = 23
          Top = 68
          Width = 102
          Height = 13
          Alignment = taRightJustify
          Caption = #1055#1077#1088#1080#1086#1076' '#1086#1087#1088#1086#1089#1072' ('#1084#1089'):'
        end
        object Label72: TLabel
          Left = 8
          Top = 96
          Width = 117
          Height = 13
          Alignment = taRightJustify
          Caption = #1058#1072#1081#1084#1072#1091#1090' '#1079#1072#1087#1088#1086#1089#1086#1074' ('#1084#1089'):'
        end
        object EditDeviceRSLinxPeriodMS: TEdit
          Left = 127
          Top = 65
          Width = 100
          Height = 21
          NumbersOnly = True
          TabOrder = 0
          Text = 'Edit2'
        end
        object EditDeviceRSLinxProtocol: TEdit
          Left = 127
          Top = 38
          Width = 100
          Height = 21
          NumbersOnly = True
          ReadOnly = True
          TabOrder = 1
          Text = 'Edit2'
        end
        object EditDeviceRSLinxTimeoutMS: TEdit
          Left = 126
          Top = 93
          Width = 101
          Height = 21
          NumbersOnly = True
          TabOrder = 2
          Text = 'Edit2'
        end
        object EditDeviceRSLinxType: TEdit
          Left = 332
          Top = 37
          Width = 100
          Height = 21
          ReadOnly = True
          TabOrder = 3
          Text = 'EditDeviceRSLinxType'
        end
        object EditDeviceRSLinxUUID: TEdit
          Left = 127
          Top = 10
          Width = 306
          Height = 21
          ReadOnly = True
          TabOrder = 4
          Text = 'EditDeviceUUID'
        end
      end
    end
    object TabSheet8: TTabSheet
      Caption = #1059#1089#1090#1088#1086#1081#1089#1090#1074#1086' '#1087#1086' '#1091#1084#1086#1083#1095#1072#1085#1080#1102
      ImageIndex = 10
      object PanelDeviceSettingAdditional: TPanel
        Left = 0
        Top = 0
        Width = 696
        Height = 35
        Align = alTop
        TabOrder = 0
        Visible = False
        object Label5: TLabel
          Left = 96
          Top = 13
          Width = 29
          Height = 13
          Alignment = taRightJustify
          Caption = 'UUID:'
        end
        object Label6: TLabel
          Left = 308
          Top = 40
          Width = 22
          Height = 13
          Caption = #1058#1080#1087':'
          Visible = False
        end
        object Label7: TLabel
          Left = 72
          Top = 40
          Width = 53
          Height = 13
          Alignment = taRightJustify
          Caption = #1055#1088#1086#1090#1086#1082#1086#1083':'
          Visible = False
        end
        object Label8: TLabel
          Left = 301
          Top = 67
          Width = 29
          Height = 13
          Caption = #1055#1086#1088#1090':'
          Visible = False
        end
        object Label9: TLabel
          Left = 40
          Top = 67
          Width = 85
          Height = 13
          Alignment = taRightJustify
          Caption = #1057#1082#1086#1088#1086#1089#1090#1100' '#1087#1086#1088#1090#1072':'
          Visible = False
        end
        object Label10: TLabel
          Left = 258
          Top = 94
          Width = 72
          Height = 13
          Caption = #1041#1080#1090#1099' '#1076#1072#1085#1085#1099#1093':'
          Visible = False
        end
        object Label11: TLabel
          Left = 73
          Top = 91
          Width = 52
          Height = 13
          Alignment = taRightJustify
          Caption = #1063#1077#1090#1085#1086#1089#1090#1100':'
          Visible = False
        end
        object Label12: TLabel
          Left = 238
          Top = 121
          Width = 92
          Height = 13
          Caption = #1057#1090#1086#1087#1086#1074#1099#1099#1077' '#1073#1080#1090#1099':'
          Visible = False
        end
        object Label13: TLabel
          Left = 23
          Top = 121
          Width = 102
          Height = 13
          Alignment = taRightJustify
          Caption = #1055#1077#1088#1080#1086#1076' '#1086#1087#1088#1086#1089#1072' ('#1084#1089'):'
          Visible = False
        end
        object Label14: TLabel
          Left = 255
          Top = 148
          Width = 75
          Height = 13
          Caption = #1040#1076#1088#1077#1089' Modbus:'
          Visible = False
        end
        object Label15: TLabel
          Left = 4
          Top = 145
          Width = 117
          Height = 13
          Alignment = taRightJustify
          Caption = #1058#1072#1081#1084#1072#1091#1090' '#1079#1072#1087#1088#1086#1089#1086#1074' ('#1084#1089'):'
          Visible = False
        end
        object EditDeviceUUID: TEdit
          Left = 127
          Top = 10
          Width = 306
          Height = 21
          ReadOnly = True
          TabOrder = 0
          Text = 'EditDeviceUUID'
        end
        object EditDeviceType: TEdit
          Left = 332
          Top = 37
          Width = 100
          Height = 21
          ReadOnly = True
          TabOrder = 1
          Text = 'Edit2'
          Visible = False
        end
        object ComboBoxDeviceProtocol: TComboBox
          Left = 127
          Top = 37
          Width = 100
          Height = 21
          Style = csDropDownList
          TabOrder = 2
          Visible = False
        end
        object EditDevicePort: TEdit
          Left = 332
          Top = 64
          Width = 100
          Height = 21
          TabOrder = 3
          Text = 'Edit2'
          Visible = False
        end
        object ComboBoxDeviceBaudrate: TComboBox
          Left = 127
          Top = 64
          Width = 100
          Height = 21
          Style = csDropDownList
          TabOrder = 4
          Visible = False
        end
        object ComboBoxDeviceDataBit: TComboBox
          Left = 332
          Top = 91
          Width = 48
          Height = 21
          TabOrder = 5
          Text = 'ComboBoxCollectionPoint'
          Visible = False
        end
        object ComboBoxDeviceParity: TComboBox
          Left = 127
          Top = 91
          Width = 100
          Height = 21
          Style = csDropDownList
          TabOrder = 6
          Visible = False
        end
        object ComboBoxDeviceStopBit: TComboBox
          Left = 332
          Top = 118
          Width = 48
          Height = 21
          TabOrder = 7
          Text = 'ComboBoxDeviceStopBit'
          Visible = False
        end
        object EditDevicePeriodMS: TEdit
          Left = 127
          Top = 118
          Width = 100
          Height = 21
          NumbersOnly = True
          TabOrder = 8
          Text = 'Edit2'
          Visible = False
        end
        object EditDeviceTimeoutMS: TEdit
          Left = 127
          Top = 145
          Width = 100
          Height = 21
          NumbersOnly = True
          TabOrder = 9
          Text = 'Edit2'
          Visible = False
        end
        object EditDeviceDevAddr: TEdit
          Left = 332
          Top = 145
          Width = 100
          Height = 21
          TabOrder = 10
          Text = 'Edit2'
          Visible = False
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = #1059#1089#1090#1088#1086#1081#1089#1090#1074#1086' '#1041#1056#1048#1047
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PanelDeviceBrizSettingAdditional: TPanel
        Left = 0
        Top = 0
        Width = 696
        Height = 145
        Align = alTop
        TabOrder = 0
        Visible = False
        object Label22: TLabel
          Left = 96
          Top = 13
          Width = 29
          Height = 13
          Alignment = taRightJustify
          Caption = 'UUID:'
        end
        object Label23: TLabel
          Left = 308
          Top = 40
          Width = 22
          Height = 13
          Caption = #1058#1080#1087':'
        end
        object Label24: TLabel
          Left = 72
          Top = 40
          Width = 53
          Height = 13
          Alignment = taRightJustify
          Caption = #1055#1088#1086#1090#1086#1082#1086#1083':'
        end
        object Label25: TLabel
          Left = 301
          Top = 67
          Width = 29
          Height = 13
          Caption = #1055#1086#1088#1090':'
        end
        object Label26: TLabel
          Left = 90
          Top = 67
          Width = 35
          Height = 13
          Alignment = taRightJustify
          Caption = #1040#1076#1088#1077#1089':'
        end
        object Label29: TLabel
          Left = 289
          Top = 93
          Width = 41
          Height = 13
          Caption = #1055#1072#1088#1086#1083#1100':'
        end
        object Label36: TLabel
          Left = 49
          Top = 93
          Width = 76
          Height = 13
          Alignment = taRightJustify
          Caption = #1055#1086#1083#1100#1079#1086#1074#1072#1090#1077#1083#1100':'
        end
        object Label38: TLabel
          Left = 23
          Top = 121
          Width = 102
          Height = 13
          Alignment = taRightJustify
          Caption = #1055#1077#1088#1080#1086#1076' '#1086#1087#1088#1086#1089#1072' ('#1084#1089'):'
        end
        object Label40: TLabel
          Left = 259
          Top = 121
          Width = 71
          Height = 13
          Alignment = taRightJustify
          Caption = #1058#1072#1081#1084#1072#1091#1090'  ('#1084#1089'):'
        end
        object EditDeviceBrizAddr: TEdit
          Left = 127
          Top = 64
          Width = 100
          Height = 21
          TabOrder = 0
          Text = 'EditDeviceBrizType'
        end
        object EditDeviceBrizPass: TEdit
          Left = 333
          Top = 90
          Width = 100
          Height = 21
          TabOrder = 3
          Text = 'Edit2'
        end
        object EditDeviceBrizPeriodMS: TEdit
          Left = 127
          Top = 118
          Width = 100
          Height = 21
          NumbersOnly = True
          TabOrder = 4
          Text = 'Edit2'
        end
        object EditDeviceBrizPort: TEdit
          Left = 333
          Top = 64
          Width = 100
          Height = 21
          TabOrder = 1
          Text = 'Edit2'
        end
        object EditDeviceBrizProtocol: TEdit
          Left = 127
          Top = 37
          Width = 100
          Height = 21
          ReadOnly = True
          TabOrder = 6
          Text = 'EditDeviceBrizType'
        end
        object EditDeviceBrizTimeoutMS: TEdit
          Left = 333
          Top = 118
          Width = 100
          Height = 21
          NumbersOnly = True
          TabOrder = 5
          Text = 'Edit2'
        end
        object EditDeviceBrizType: TEdit
          Left = 333
          Top = 37
          Width = 100
          Height = 21
          ReadOnly = True
          TabOrder = 7
          Text = 'EditDeviceBrizType'
        end
        object EditDeviceBrizUser: TEdit
          Left = 127
          Top = 90
          Width = 100
          Height = 21
          TabOrder = 2
          Text = 'EditDeviceBrizType'
        end
        object EditDeviceBrizUUID: TEdit
          Left = 127
          Top = 10
          Width = 306
          Height = 21
          ReadOnly = True
          TabOrder = 8
          Text = 'EditDeviceUUID'
        end
      end
    end
    object TabSheet7: TTabSheet
      Caption = #1044#1072#1090#1095#1080#1082' '#1041#1056#1048#1047
      ImageIndex = 9
      object PanelSensorBriz: TPanel
        Left = 0
        Top = 0
        Width = 696
        Height = 41
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        Visible = False
        object Panel22: TPanel
          Left = 0
          Top = 0
          Width = 696
          Height = 41
          Align = alClient
          TabOrder = 0
          object Label67: TLabel
            Left = 85
            Top = 13
            Width = 36
            Height = 13
            Alignment = taRightJustify
            Caption = #1056#1077#1078#1080#1084':'
          end
          object ComboBoxSensorBrizMode: TComboBox
            Left = 127
            Top = 10
            Width = 146
            Height = 21
            TabOrder = 0
            Text = 'ComboBoxSensorBrizMode'
          end
        end
      end
    end
    object TabSheet3: TTabSheet
      Caption = #1044#1072#1090#1095#1080#1082
      ImageIndex = 4
      object PanelSensor: TPanel
        Left = 0
        Top = 0
        Width = 696
        Height = 345
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        Visible = False
        object PanelSensorSettingAdditional: TPanel
          Left = 0
          Top = 230
          Width = 696
          Height = 115
          Align = alClient
          TabOrder = 0
          object Label19: TLabel
            Left = 97
            Top = 8
            Width = 29
            Height = 13
            Alignment = taRightJustify
            Caption = 'UUID:'
          end
          object Label20: TLabel
            Left = 266
            Top = 35
            Width = 64
            Height = 13
            Caption = #1058#1080#1087' '#1076#1072#1085#1085#1099#1093':'
            Visible = False
          end
          object Label21: TLabel
            Left = 85
            Top = 35
            Width = 41
            Height = 13
            Alignment = taRightJustify
            Caption = #1048#1085#1076#1077#1082#1089':'
            Visible = False
          end
          object Label27: TLabel
            Left = 9
            Top = 61
            Width = 117
            Height = 13
            Alignment = taRightJustify
            Caption = #1056#1072#1079#1084#1077#1088' '#1089#1086#1073#1099#1090#1080#1081' ('#1089#1090#1088'.):'
            Visible = False
          end
          object Label28: TLabel
            Left = 267
            Top = 61
            Width = 63
            Height = 13
            Caption = #1056#1072#1079#1084#1077#1088' '#1089#1090#1088'.:'
            Visible = False
          end
          object Label41: TLabel
            Left = 262
            Top = 88
            Width = 68
            Height = 13
            Caption = #1058#1080#1087' '#1089#1086#1073#1099#1090#1080#1081':'
            Visible = False
          end
          object Label61: TLabel
            Left = 54
            Top = 88
            Width = 72
            Height = 13
            Alignment = taRightJustify
            Caption = #1057#1075#1083#1072#1078#1080#1074#1072#1085#1080#1077':'
          end
          object EditSensorUUID: TEdit
            Left = 127
            Top = 5
            Width = 305
            Height = 21
            ReadOnly = True
            TabOrder = 0
            Text = 'EditDeviceUUID'
          end
          object EditSensorAlarmSize: TEdit
            Left = 127
            Top = 58
            Width = 100
            Height = 21
            Hint = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1099#1081' '#1088#1072#1079#1084#1077#1088' '#1089#1090#1088#1086#1082#1086#1074#1099#1093' '#1089#1086#1073#1099#1090#1080#1081' '#1074' '#1073#1072#1081#1090#1072#1093
            NumbersOnly = True
            TabOrder = 1
            Visible = False
          end
          object EditSensorIndex: TEdit
            Left = 127
            Top = 32
            Width = 100
            Height = 21
            NumbersOnly = True
            ReadOnly = True
            TabOrder = 2
            Text = 'EditSensorIndex'
            Visible = False
          end
          object EditSensorSize: TEdit
            Left = 332
            Top = 58
            Width = 100
            Height = 21
            Hint = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1099#1081' '#1088#1072#1079#1084#1077#1088' '#1089#1090#1088#1086#1082#1086#1074#1099#1093' '#1076#1072#1085#1085#1099#1093
            NumbersOnly = True
            TabOrder = 3
            Text = 'Edit2'
            Visible = False
          end
          object ComboBoxSensorDataType: TComboBox
            Left = 332
            Top = 32
            Width = 100
            Height = 21
            Hint = #1058#1080#1087' '#1076#1072#1085#1085#1099#1093
            Style = csDropDownList
            Enabled = False
            TabOrder = 4
            Visible = False
          end
          object ComboBoxAlarmType: TComboBox
            Left = 332
            Top = 85
            Width = 100
            Height = 21
            Hint = #1058#1080#1087' '#1076#1072#1085#1085#1099#1093
            Style = csDropDownList
            TabOrder = 5
            Visible = False
          end
          object EditGaussFilter: TEdit
            Left = 127
            Top = 85
            Width = 100
            Height = 21
            Hint = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1099#1081' '#1088#1072#1079#1084#1077#1088' '#1089#1090#1088#1086#1082#1086#1074#1099#1093' '#1089#1086#1073#1099#1090#1080#1081' '#1074' '#1073#1072#1081#1090#1072#1093
            NumbersOnly = True
            TabOrder = 6
            OnChange = EditGaussFilterChange
          end
        end
        object Panel8: TPanel
          Left = 0
          Top = 24
          Width = 696
          Height = 206
          Align = alTop
          TabOrder = 1
          object Label30: TLabel
            Left = 84
            Top = 64
            Width = 42
            Height = 13
            Alignment = taRightJustify
            Caption = #1044#1072#1090#1095#1080#1082':'
          end
          object Label31: TLabel
            Left = 23
            Top = 118
            Width = 103
            Height = 13
            Alignment = taRightJustify
            Caption = #1045#1076#1080#1085#1080#1094#1072' '#1080#1079#1084#1077#1088#1077#1085#1080#1103':'
          end
          object Label32: TLabel
            Left = 73
            Top = 91
            Width = 53
            Height = 13
            Alignment = taRightJustify
            Caption = #1054#1087#1080#1089#1072#1085#1080#1077':'
          end
          object Label33: TLabel
            Left = 268
            Top = 118
            Width = 62
            Height = 13
            Alignment = taRightJustify
            Caption = #1052#1085#1086#1078#1080#1090#1077#1083#1100':'
          end
          object Label34: TLabel
            Left = 59
            Top = 145
            Width = 67
            Height = 13
            Alignment = taRightJustify
            Caption = #1059#1089#1090#1072#1074#1082#1072' '#1084#1080#1085':'
          end
          object Label35: TLabel
            Left = 258
            Top = 145
            Width = 72
            Height = 13
            Alignment = taRightJustify
            Caption = #1059#1089#1090#1072#1074#1082#1072' '#1084#1072#1082#1089':'
          end
          object Label46: TLabel
            Left = 279
            Top = 172
            Width = 51
            Height = 13
            Alignment = taRightJustify
            Caption = #1058#1086#1095#1085#1086'c'#1090#1100':'
          end
          object Label1: TLabel
            Left = 63
            Top = 37
            Width = 63
            Height = 13
            Alignment = taRightJustify
            Caption = #1059#1089#1090#1088#1086#1081#1089#1090#1074#1086':'
          end
          object Label3: TLabel
            Left = 60
            Top = 10
            Width = 66
            Height = 13
            Alignment = taRightJustify
            Caption = #1058#1086#1095#1082#1072' '#1089#1073#1086#1088#1072':'
          end
          object EditSensorName: TEdit
            Left = 127
            Top = 61
            Width = 305
            Height = 21
            ReadOnly = True
            TabOrder = 0
          end
          object ComboBoxSensorUnit: TComboBox
            Left = 127
            Top = 115
            Width = 100
            Height = 21
            Style = csDropDownList
            Enabled = False
            TabOrder = 1
          end
          object ButtonSensorAdditional: TButton
            Left = 7
            Top = 175
            Width = 118
            Height = 25
            Caption = #1044#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1086
            TabOrder = 2
            Visible = False
          end
          object EditSensorDescription: TEdit
            Left = 127
            Top = 88
            Width = 305
            Height = 21
            ReadOnly = True
            TabOrder = 3
          end
          object ComboBoxSensorPower: TComboBox
            Left = 332
            Top = 115
            Width = 100
            Height = 21
            Style = csDropDownList
            Enabled = False
            TabOrder = 4
          end
          object EditSensorMinValue: TEdit
            Left = 127
            Top = 142
            Width = 100
            Height = 21
            ReadOnly = True
            TabOrder = 5
            Text = 'Edit2'
          end
          object EditSensorMaxValue: TEdit
            Left = 332
            Top = 142
            Width = 100
            Height = 21
            ReadOnly = True
            TabOrder = 6
            Text = 'Edit2'
          end
          object ComboBoxSensorPrecision: TComboBox
            Left = 333
            Top = 169
            Width = 100
            Height = 21
            Style = csDropDownList
            Enabled = False
            TabOrder = 7
          end
          object EditSensorDevice: TEdit
            Left = 127
            Top = 34
            Width = 305
            Height = 21
            ReadOnly = True
            TabOrder = 8
          end
          object EditSensorRemotePoint: TEdit
            Left = 127
            Top = 7
            Width = 305
            Height = 21
            ReadOnly = True
            TabOrder = 9
          end
        end
        object Panel9: TPanel
          Left = 0
          Top = 0
          Width = 696
          Height = 24
          Align = alTop
          Alignment = taLeftJustify
          TabOrder = 2
          object Label53: TLabel
            AlignWithMargins = True
            Left = 4
            Top = 4
            Width = 688
            Height = 16
            Align = alClient
            Caption = #1044#1072#1090#1095#1080#1082
            ExplicitWidth = 38
            ExplicitHeight = 13
          end
        end
      end
    end
    object TabSheet4: TTabSheet
      Caption = #1044#1080#1089#1082#1088#1077#1090#1085#1099#1081' '#1076#1072#1090#1095#1080#1082
      ImageIndex = 5
      object PanelSensorBit: TPanel
        Left = 0
        Top = 0
        Width = 696
        Height = 90
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        Visible = False
        object Panel16: TPanel
          Left = 0
          Top = 24
          Width = 696
          Height = 66
          Align = alClient
          TabOrder = 0
          object Label49: TLabel
            Left = 72
            Top = 9
            Width = 53
            Height = 13
            Alignment = taRightJustify
            Caption = #1054#1087#1080#1089#1072#1085#1080#1077':'
          end
          object Label39: TLabel
            Left = 52
            Top = 37
            Width = 73
            Height = 13
            Alignment = taRightJustify
            Caption = #1042#1080#1076' '#1101#1083#1077#1084#1077#1085#1090#1072':'
          end
          object EditSensorBitSettingDesc: TEdit
            Left = 127
            Top = 6
            Width = 305
            Height = 21
            TabOrder = 0
          end
          object ComboBoxSensorBitView: TComboBox
            Left = 127
            Top = 33
            Width = 100
            Height = 21
            Style = csDropDownList
            TabOrder = 1
          end
        end
        object Panel17: TPanel
          Left = 0
          Top = 0
          Width = 696
          Height = 24
          Align = alTop
          Alignment = taLeftJustify
          TabOrder = 1
          object Label55: TLabel
            AlignWithMargins = True
            Left = 4
            Top = 4
            Width = 104
            Height = 13
            Align = alClient
            Caption = #1044#1080#1089#1082#1088#1077#1090#1085#1099#1081' '#1044#1072#1090#1095#1080#1082
          end
        end
      end
      object PanelSensorBitImage: TPanel
        Left = 0
        Top = 155
        Width = 696
        Height = 94
        Align = alTop
        TabOrder = 1
        Visible = False
        object Label44: TLabel
          Left = 44
          Top = 14
          Width = 81
          Height = 13
          Alignment = taRightJustify
          Caption = #1048#1079#1086#1073#1088#1072#1078#1077#1085#1080#1077' 0:'
        end
        object Label45: TLabel
          Left = 44
          Top = 49
          Width = 81
          Height = 13
          Alignment = taRightJustify
          Caption = #1048#1079#1086#1073#1088#1072#1078#1077#1085#1080#1077' 1:'
        end
        object ImageSensorBitValue0: TImage
          Left = 127
          Top = 6
          Width = 32
          Height = 32
        end
        object ImageSensorBitValue1: TImage
          Left = 127
          Top = 41
          Width = 32
          Height = 32
        end
        object ButtonSetSensorBitValue0: TButton
          Left = 301
          Top = 11
          Width = 32
          Height = 21
          Hint = #1047#1072#1075#1088#1091#1079#1080#1090#1100' '#1085#1086#1074#1086#1077' '#1080#1079#1086#1073#1088#1072#1078#1077#1085#1080#1077
          Caption = '...'
          TabOrder = 0
        end
        object ButtonSetSensorBitValue1: TButton
          Left = 301
          Top = 46
          Width = 32
          Height = 21
          Hint = #1047#1072#1075#1088#1091#1079#1080#1090#1100' '#1085#1086#1074#1086#1077' '#1080#1079#1086#1073#1088#1072#1078#1077#1085#1080#1077
          Caption = '...'
          TabOrder = 1
        end
        object ComboBoxSensorBitValue0: TComboBox
          Left = 165
          Top = 11
          Width = 135
          Height = 21
          TabOrder = 2
          Text = 'ComboBoxSensorBitValue0'
        end
        object ComboBoxSensorBitValue1: TComboBox
          Left = 165
          Top = 46
          Width = 135
          Height = 21
          TabOrder = 3
          Text = 'ComboBoxSensorBitValue1'
        end
        object RadioButtonSensorBitValue0: TRadioButton
          Left = 343
          Top = 13
          Width = 113
          Height = 17
          Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100
          TabOrder = 4
        end
        object RadioButtonSensorBitValue1: TRadioButton
          Left = 343
          Top = 48
          Width = 113
          Height = 17
          Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100
          TabOrder = 5
        end
      end
      object PanelSensorBitText: TPanel
        Left = 0
        Top = 90
        Width = 696
        Height = 65
        Align = alTop
        TabOrder = 2
        Visible = False
        object Label42: TLabel
          Left = 64
          Top = 9
          Width = 61
          Height = 13
          Alignment = taRightJustify
          Caption = #1047#1085#1072#1095#1077#1085#1080#1077' 0:'
        end
        object Label43: TLabel
          Left = 64
          Top = 36
          Width = 61
          Height = 13
          Alignment = taRightJustify
          Caption = #1047#1085#1072#1095#1077#1085#1080#1077' 1:'
        end
        object EditSensorBitValue0: TEdit
          Left = 127
          Top = 6
          Width = 305
          Height = 21
          TabOrder = 0
        end
        object EditSensorBitValue1: TEdit
          Left = 127
          Top = 33
          Width = 305
          Height = 21
          TabOrder = 1
        end
      end
    end
    object TabSheet5: TTabSheet
      Caption = #1052#1085#1077#1084#1086#1089#1093#1077#1084#1072
      ImageIndex = 6
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PanelMnemoshema: TPanel
        Left = 0
        Top = 0
        Width = 696
        Height = 193
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        Visible = False
        object Panel7: TPanel
          Left = 0
          Top = 24
          Width = 696
          Height = 169
          Align = alClient
          TabOrder = 0
          object Label37: TLabel
            Left = 91
            Top = 9
            Width = 23
            Height = 13
            Caption = #1048#1084#1103':'
          end
          object ImageMnemoshema: TImage
            Left = 127
            Top = 33
            Width = 314
            Height = 129
            Proportional = True
          end
          object EditMnemoshemaName: TEdit
            Left = 120
            Top = 6
            Width = 313
            Height = 21
            TabOrder = 0
          end
          object Button1: TButton
            Left = 7
            Top = 33
            Width = 118
            Height = 25
            Caption = #1055#1086#1076#1083#1086#1078#1082#1072
            TabOrder = 1
          end
        end
        object Panel12: TPanel
          Left = 0
          Top = 0
          Width = 696
          Height = 24
          Align = alTop
          Alignment = taLeftJustify
          TabOrder = 1
          object Label54: TLabel
            AlignWithMargins = True
            Left = 4
            Top = 4
            Width = 61
            Height = 13
            Align = alClient
            Caption = #1052#1085#1077#1084#1086#1089#1093#1077#1084#1072
          end
        end
      end
    end
    object TabSheet6: TTabSheet
      Caption = #1044#1072#1090#1095#1080#1082' '#1076#1086#1087'.'
      ImageIndex = 7
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PanelMimicElementView: TPanel
        Left = 0
        Top = 0
        Width = 696
        Height = 90
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        Visible = False
        object Panel14: TPanel
          Left = 0
          Top = 24
          Width = 696
          Height = 66
          Align = alTop
          TabOrder = 0
          object Label48: TLabel
            Left = 46
            Top = 9
            Width = 79
            Height = 13
            Alignment = taRightJustify
            Caption = #1055#1086#1076#1087#1080#1089#1100' '#1089#1083#1077#1074#1072':'
          end
          object Label50: TLabel
            Left = 40
            Top = 36
            Width = 85
            Height = 13
            Alignment = taRightJustify
            Caption = #1055#1086#1076#1087#1080#1089#1100' '#1089#1087#1088#1072#1074#1072':'
          end
          object EditMimicPanelElementLabelLeft: TEdit
            Left = 131
            Top = 6
            Width = 305
            Height = 21
            TabOrder = 0
          end
          object EditMimicPanelElementLabelRight: TEdit
            Left = 131
            Top = 33
            Width = 305
            Height = 21
            TabOrder = 1
          end
        end
        object Panel15: TPanel
          Left = 0
          Top = 0
          Width = 696
          Height = 24
          Align = alTop
          TabOrder = 1
          object Label60: TLabel
            AlignWithMargins = True
            Left = 4
            Top = 4
            Width = 80
            Height = 13
            Align = alClient
            Caption = #1044#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1086
          end
        end
      end
    end
  end
end
