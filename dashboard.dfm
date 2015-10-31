object FormDashboard: TFormDashboard
  Left = 0
  Top = 0
  Caption = #1056#1072#1073#1086#1095#1080#1081' '#1089#1090#1086#1083
  ClientHeight = 543
  ClientWidth = 722
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poMainFormCenter
  Visible = True
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControlMP: TPageControl
    Left = 0
    Top = 28
    Width = 722
    Height = 515
    Align = alClient
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 722
    Height = 28
    Align = alTop
    TabOrder = 1
    object CheckBoxDataHistory: TCheckBox
      AlignWithMargins = True
      Left = 4
      Top = 4
      Width = 108
      Height = 20
      Align = alLeft
      Caption = #1048#1089#1090#1086#1088#1080#1103' '#1076#1072#1085#1085#1099#1093
      TabOrder = 0
      OnClick = CheckBoxDataHistoryClick
    end
    object DateDashboardHistory: TDateTimePicker
      AlignWithMargins = True
      Left = 118
      Top = 4
      Width = 107
      Height = 20
      Align = alLeft
      Date = 42295.428069548610000000
      Time = 42295.428069548610000000
      TabOrder = 1
      OnChange = DateDashboardHistoryChange
    end
    object TimeDashboardHistory: TDateTimePicker
      AlignWithMargins = True
      Left = 231
      Top = 4
      Width = 107
      Height = 20
      Align = alLeft
      Date = 42295.428069548610000000
      Time = 42295.428069548610000000
      Kind = dtkTime
      TabOrder = 2
      OnChange = TimeDashboardHistoryChange
    end
    object ButtonRequestDataMnemoshemaHistory: TButton
      AlignWithMargins = True
      Left = 344
      Top = 4
      Width = 75
      Height = 20
      Align = alLeft
      Caption = #1054#1073#1085#1086#1074#1080#1090#1100
      TabOrder = 3
      OnClick = ButtonRequestDataMnemoshemaHistoryClick
    end
  end
end
