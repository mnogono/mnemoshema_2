object FormDebug: TFormDebug
  Left = 0
  Top = 0
  Caption = 'FormDebug'
  ClientHeight = 339
  ClientWidth = 664
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  WindowState = wsMinimized
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 136
    Height = 13
    Caption = 'SENSOR_DATA_POOL used:'
  end
  object Label2: TLabel
    Left = 8
    Top = 27
    Width = 155
    Height = 13
    Caption = 'SENSOR_DATA_POOL available:'
  end
  object LabelSensorDataPoolUsed: TLabel
    Left = 185
    Top = 8
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object LabelSensorDataPoolAvailable: TLabel
    Left = 185
    Top = 27
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object Label3: TLabel
    Left = 8
    Top = 46
    Width = 89
    Height = 13
    Caption = 'Thread pool tasks:'
  end
  object LabelThreadPoolTasks: TLabel
    Left = 185
    Top = 46
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object Label4: TLabel
    Left = 8
    Top = 80
    Width = 121
    Height = 13
    Caption = 'HTTP Request pool used:'
  end
  object LabelHTTPRequestPoolUsed: TLabel
    Left = 185
    Top = 80
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object Label5: TLabel
    Left = 8
    Top = 99
    Width = 137
    Height = 13
    Caption = 'HTTP request pool available:'
  end
  object LabelHTTPRequestPoolAvailable: TLabel
    Left = 185
    Top = 99
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object Label6: TLabel
    Left = 8
    Top = 128
    Width = 112
    Height = 13
    Caption = 'File Request pool used:'
  end
  object Label7: TLabel
    Left = 8
    Top = 147
    Width = 128
    Height = 13
    Caption = 'File request pool available:'
  end
  object LabelFileRequestPoolUsed: TLabel
    Left = 185
    Top = 128
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object LabelFileRequestPoolAvailable: TLabel
    Left = 185
    Top = 147
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object Label8: TLabel
    Left = 8
    Top = 176
    Width = 147
    Height = 13
    Caption = 'Device File Request pool used:'
  end
  object Label9: TLabel
    Left = 8
    Top = 195
    Width = 163
    Height = 13
    Caption = 'Device File request pool available:'
  end
  object LabelDeviceFileRequestPoolUsed: TLabel
    Left = 185
    Top = 176
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object LabelDeviceFileRequestPoolAvailable: TLabel
    Left = 185
    Top = 195
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object Label10: TLabel
    Left = 8
    Top = 251
    Width = 109
    Height = 13
    Caption = 'request pool available:'
  end
  object Label11: TLabel
    Left = 8
    Top = 232
    Width = 93
    Height = 13
    Caption = 'Request pool used:'
  end
  object LabelRequestPoolUsed: TLabel
    Left = 185
    Top = 232
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object LabelRequestPoolAvailable: TLabel
    Left = 185
    Top = 251
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object Label12: TLabel
    Left = 264
    Top = 8
    Width = 173
    Height = 13
    Caption = 'SENSOR_EVENT_DATA_POOL used:'
  end
  object Label13: TLabel
    Left = 264
    Top = 27
    Width = 192
    Height = 13
    Caption = 'SENSOR_EVENT_DATA_POOL available:'
  end
  object LabelSensorEventDataPoolUsed: TLabel
    Left = 459
    Top = 8
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object LabelSensorEventDataPoolAvailable: TLabel
    Left = 459
    Top = 27
    Width = 35
    Height = 13
    Caption = 'COUNT'
  end
  object TimerSensorDataPoolUpdate: TTimer
    OnTimer = TimerSensorDataPoolUpdateTimer
    Left = 552
    Top = 8
  end
end
