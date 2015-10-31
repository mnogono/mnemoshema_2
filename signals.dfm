object FormSignals: TFormSignals
  Left = 880
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #1057#1080#1075#1085#1072#1083#1099
  ClientHeight = 364
  ClientWidth = 647
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  Visible = True
  WindowState = wsMinimized
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object StringGridSignals: TStringGrid
    Left = 0
    Top = 0
    Width = 647
    Height = 364
    Align = alClient
    ColCount = 3
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect, goThumbTracking]
    TabOrder = 0
  end
end
