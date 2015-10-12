object FormReports: TFormReports
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1085#1080#1077' '#1096#1072#1073#1083#1086#1085#1086#1074' '#1086#1090#1095#1077#1090#1086#1074
  ClientHeight = 456
  ClientWidth = 571
  Color = clBtnFace
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
  PixelsPerInch = 96
  TextHeight = 13
  object Label6: TLabel
    Left = 8
    Top = 35
    Width = 66
    Height = 13
    Caption = #1048#1084#1103' '#1096#1072#1073#1083#1086#1085#1072
  end
  object ButtonSelectTemplateFile: TButton
    Left = 407
    Top = 8
    Width = 143
    Height = 25
    Caption = #1042#1099#1073#1088#1072#1090#1100' '#1092#1072#1081#1083' '#1096#1072#1073#1083#1086#1085#1072
    TabOrder = 0
    OnClick = ButtonSelectTemplateFileClick
  end
  object EditTemplateTitle: TEdit
    Left = 8
    Top = 50
    Width = 393
    Height = 21
    ReadOnly = True
    TabOrder = 1
  end
  object EditReportTemplateFile: TEdit
    Left = 8
    Top = 8
    Width = 393
    Height = 21
    TabOrder = 2
  end
  object ButtonSaveTempateFile: TButton
    Left = 8
    Top = 77
    Width = 105
    Height = 25
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 3
    OnClick = ButtonSaveTempateFileClick
  end
  object DBGridReports: TDBGrid
    Left = 8
    Top = 120
    Width = 545
    Height = 281
    DataSource = DataSource
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit, dgMultiSelect, dgTitleClick, dgTitleHotTrack]
    TabOrder = 4
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'title'
        Title.Caption = #1064#1072#1073#1083#1086#1085' '#1086#1090#1095#1077#1090#1072
        Visible = True
      end>
  end
  object ButtonOpenReportTemplate: TButton
    Left = 8
    Top = 407
    Width = 105
    Height = 25
    Caption = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1090#1100
    TabOrder = 5
    OnClick = ButtonOpenReportTemplateClick
  end
  object ButtonDeleteReportTemplate: TButton
    Left = 119
    Top = 407
    Width = 75
    Height = 25
    Caption = #1059#1076#1072#1083#1080#1090#1100
    TabOrder = 6
    OnClick = ButtonDeleteReportTemplateClick
  end
  object OpenDialog: TOpenDialog
    Left = 408
    Top = 48
  end
  object SaveDialog: TSaveDialog
    FileName = '111'
    Left = 480
    Top = 48
  end
  object DataSource: TDataSource
    DataSet = ADOQuery
    Left = 360
    Top = 272
  end
  object ADOQuery: TADOQuery
    Active = True
    ConnectionString = 
      'Provider=MSDASQL.1;Persist Security Info=False;Data Source=mnemo' +
      'shema'
    CursorType = ctStatic
    Parameters = <>
    SQL.Strings = (
      'select id, title from reports')
    Left = 424
    Top = 272
  end
end
