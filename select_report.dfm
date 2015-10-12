object FormReportList: TFormReportList
  Left = 0
  Top = 0
  BorderIcons = [biMinimize, biMaximize]
  BorderStyle = bsDialog
  Caption = #1042#1099#1073#1077#1088#1080#1090#1077' '#1096#1072#1073#1083#1086#1085' '#1086#1090#1095#1077#1090#1072
  ClientHeight = 71
  ClientWidth = 305
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 299
    Height = 13
    Align = alTop
    Caption = #1064#1072#1073#1083#1086#1085' '#1086#1090#1095#1077#1090#1072
    ExplicitWidth = 79
  end
  object ComboBoxReports: TComboBox
    AlignWithMargins = True
    Left = 3
    Top = 22
    Width = 299
    Height = 21
    Align = alTop
    TabOrder = 0
  end
  object ButtonCance: TButton
    Left = 230
    Top = 46
    Width = 75
    Height = 25
    Align = alRight
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 1
    OnClick = ButtonCanceClick
    ExplicitLeft = 392
    ExplicitTop = 49
  end
  object ButtonOK: TButton
    Left = 155
    Top = 46
    Width = 75
    Height = 25
    Align = alRight
    Caption = #1054#1050
    TabOrder = 2
    OnClick = ButtonOKClick
    ExplicitLeft = 505
    ExplicitTop = 56
  end
end
