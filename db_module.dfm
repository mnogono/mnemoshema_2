object DataModuleMP: TDataModuleMP
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  OnDestroy = DataModuleDestroy
  Height = 515
  Width = 657
  object ADOConnection: TADOConnection
    CommandTimeout = 5
    ConnectionString = 
      'Provider=MSDASQL.1;Persist Security Info=False;User ID=root;Data' +
      ' Source=mnemoshema;'
    ConnectionTimeout = 5
    LoginPrompt = False
    Provider = 'MSDASQL.1'
    Left = 40
    Top = 16
  end
  object ADOQuery: TADOQuery
    Connection = ADOConnection
    Parameters = <>
    Left = 56
    Top = 96
  end
  object ADOQuerySensorFolder: TADOQuery
    AutoCalcFields = False
    Connection = ADOConnection
    CursorType = ctStatic
    Parameters = <>
    Prepared = True
    Left = 56
    Top = 152
  end
end
