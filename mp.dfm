object FormMnemoshemaMain: TFormMnemoshemaMain
  Left = 0
  Top = 0
  Caption = #1052#1085#1077#1084#1086#1089#1093#1077#1084#1072
  ClientHeight = 615
  ClientWidth = 886
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsMDIForm
  KeyPreview = True
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 185
    Top = 0
    Height = 615
    ExplicitLeft = 256
    ExplicitTop = 280
    ExplicitHeight = 100
  end
  object Splitter2: TSplitter
    Left = 696
    Top = 0
    Height = 615
    Align = alRight
    ExplicitLeft = 708
    ExplicitTop = 47
    ExplicitHeight = 574
  end
  object PanelMnemoshemaLeft: TPanel
    Left = 0
    Top = 0
    Width = 185
    Height = 615
    Align = alLeft
    Caption = 'PanelMnemoshemaLeft'
    TabOrder = 0
    object PanelSensorSearch: TPanel
      Left = 1
      Top = 1
      Width = 183
      Height = 29
      Align = alTop
      Caption = 'PanelSensorSearch'
      TabOrder = 0
      object Label1: TLabel
        AlignWithMargins = True
        Left = 4
        Top = 7
        Width = 34
        Height = 18
        Margins.Top = 6
        Align = alLeft
        Caption = #1055#1086#1080#1089#1082':'
        ExplicitHeight = 13
      end
      object EditSearch: TEdit
        AlignWithMargins = True
        Left = 44
        Top = 4
        Width = 110
        Height = 21
        Align = alClient
        TabOrder = 0
        OnChange = EditSearchChange
      end
      object ButtonClearFilterText: TButton
        AlignWithMargins = True
        Left = 160
        Top = 4
        Width = 19
        Height = 21
        Align = alRight
        Caption = 'x'
        TabOrder = 1
        OnClick = ButtonClearFilterTextClick
      end
    end
    object TreeViewDevice: TTreeView
      Left = 1
      Top = 30
      Width = 183
      Height = 584
      Align = alClient
      Indent = 19
      PopupMenu = PopupMenuTreeViewDevice
      TabOrder = 1
      OnChange = TreeViewDeviceChange
    end
    object TreeViewDeviceFiltered: TTreeView
      Left = 1
      Top = 30
      Width = 183
      Height = 584
      Align = alClient
      Indent = 19
      PopupMenu = PopupMenuTreeViewDevice
      TabOrder = 2
      Visible = False
      OnChange = TreeViewDeviceChange
    end
  end
  object PanelEvents: TPanel
    Left = 699
    Top = 0
    Width = 187
    Height = 615
    Align = alRight
    Caption = #1055#1072#1085#1077#1083#1100' '#1089#1086#1073#1099#1090#1080#1081
    TabOrder = 1
    object Splitter3: TSplitter
      Left = 1
      Top = 511
      Width = 185
      Height = 3
      Cursor = crVSplit
      Align = alBottom
      ExplicitTop = 31
      ExplicitWidth = 494
    end
    object PanelEventDateTimeSettings: TPanel
      Left = 1
      Top = 1
      Width = 185
      Height = 30
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      object CheckBoxEventByDate: TCheckBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 102
        Height = 24
        Align = alLeft
        Caption = #1057#1086#1073#1099#1090#1080#1103' '#1079#1072' '#1076#1072#1090#1091
        TabOrder = 0
        OnClick = CheckBoxEventByDateClick
      end
      object DateTimePickerEventDate: TDateTimePicker
        AlignWithMargins = True
        Left = 111
        Top = 3
        Width = 71
        Height = 24
        Align = alClient
        Date = 42219.888795034720000000
        Time = 42219.888795034720000000
        TabOrder = 1
        OnChange = DateTimePickerEventDateChange
      end
    end
    object MemoEventDescription: TMemo
      Left = 1
      Top = 514
      Width = 185
      Height = 100
      Align = alBottom
      Lines.Strings = (
        #1057#1086#1076#1077#1088#1078#1080#1084#1086#1077' '#1092#1072#1081#1083#1072' '#1072#1074#1072#1088#1080#1081)
      ReadOnly = True
      ScrollBars = ssBoth
      TabOrder = 1
    end
  end
  object TimerUpdateHTTPData: TTimer
    OnTimer = TimerUpdateHTTPDataTimer
    Left = 584
    Top = 40
  end
  object PopupMenuTreeViewDevice: TPopupMenu
    Left = 72
    Top = 224
    object PMBuildChart: TMenuItem
      Caption = #1055#1086#1089#1090#1088#1086#1080#1090#1100' '#1075#1088#1072#1092#1080#1082
      OnClick = PMBuildChartClick
    end
  end
  object MainMenu1: TMainMenu
    Left = 224
    Top = 48
    object N1: TMenuItem
      Caption = #1052#1085#1077#1084#1086#1089#1093#1077#1084#1072
      object MMClose: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = MMCloseClick
      end
    end
    object N8: TMenuItem
      Caption = #1048#1085#1092#1086#1088#1084#1072#1094#1080#1103
      OnClick = N8Click
    end
    object N10: TMenuItem
      Caption = #1043#1088#1072#1092#1080#1082
      OnClick = N10Click
    end
    object N11: TMenuItem
      Caption = #1057#1080#1075#1085#1072#1083#1099
      OnClick = N11Click
    end
    object N4: TMenuItem
      Caption = #1054#1090#1095#1077#1090#1099
      object MMOpenOffice: TMenuItem
        Caption = #1055#1088#1086#1074#1077#1088#1082#1072' Open Office '#1080' '#1050#1086#1084#1087#1086#1085#1077#1085#1090
        OnClick = MMOpenOfficeClick
      end
      object MMReportTemplateManager: TMenuItem
        Caption = #1059#1087#1088#1072#1074#1083#1077#1085#1080#1077' '#1096#1072#1073#1083#1086#1085#1072#1084#1080' '#1086#1090#1095#1077#1090#1086#1074
        OnClick = MMReportTemplateManagerClick
      end
    end
    object N2: TMenuItem
      Caption = #1054#1090#1083#1072#1076#1082#1072
      Visible = False
      OnClick = N2Click
    end
  end
  object TimerUpdateEventsData: TTimer
    Interval = 5000
    OnTimer = TimerUpdateEventsDataTimer
    Left = 584
    Top = 96
  end
  object PopupMenuRecordView: TPopupMenu
    Left = 248
    Top = 224
    object PMBuildChartForRecordView: TMenuItem
      Caption = #1055#1086#1089#1090#1088#1086#1080#1090#1100' '#1075#1088#1072#1092#1080#1082
      OnClick = PMBuildChartForRecordViewClick
    end
    object FindSensor: TMenuItem
      Caption = #1053#1072#1081#1090#1080' '#1076#1072#1090#1095#1080#1082
      OnClick = FindSensorClick
    end
  end
  object PopupMenuStringGridEvent: TPopupMenu
    Left = 752
    Top = 376
    object PMOpenEventOnChart: TMenuItem
      Caption = #1055#1086#1089#1084#1086#1090#1088#1077#1090#1100' '#1085#1072' '#1075#1088#1072#1092#1080#1082#1077
      OnClick = PMOpenEventOnChartClick
    end
    object PMOpenEventFolder: TMenuItem
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1087#1072#1087#1082#1091' '#1089' '#1089#1086#1073#1099#1090#1080#1077#1084
      OnClick = PMOpenEventFolderClick
    end
    object PMCreateEventReport: TMenuItem
      Caption = #1054#1090#1095#1077#1090' '#1087#1086' '#1089#1086#1073#1099#1090#1080#1103#1084' '#1079#1072' '#1076#1077#1085#1100
      OnClick = PMCreateEventReportClick
    end
  end
end
