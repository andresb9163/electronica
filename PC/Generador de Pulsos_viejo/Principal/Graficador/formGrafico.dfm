object formGraficos: TformGraficos
  Left = -23
  Top = 47
  Width = 1036
  Height = 687
  Caption = 'formGraficos'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 8
    Top = 8
    Width = 1100
    Height = 513
  end
  object Label1: TLabel
    Left = 312
    Top = 552
    Width = 33
    Height = 13
    Caption = 'Minimo'
  end
  object Label2: TLabel
    Left = 544
    Top = 552
    Width = 36
    Height = 13
    Caption = 'Maximo'
  end
  object TxtMin: TEdit
    Left = 352
    Top = 544
    Width = 121
    Height = 21
    Enabled = False
    TabOrder = 0
    Text = '0'
  end
  object TxtMax: TEdit
    Left = 592
    Top = 544
    Width = 121
    Height = 21
    Enabled = False
    TabOrder = 1
    Text = '1024'
  end
  object Button2: TButton
    Left = 616
    Top = 576
    Width = 41
    Height = 33
    Caption = '-->'
    TabOrder = 2
    OnClick = Button2ClickClick
  end
  object Button3: TButton
    Left = 408
    Top = 576
    Width = 43
    Height = 33
    Caption = '<--'
    TabOrder = 3
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 352
    Top = 576
    Width = 43
    Height = 33
    Caption = '<<--'
    TabOrder = 4
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 672
    Top = 576
    Width = 41
    Height = 33
    Caption = '-->>'
    TabOrder = 5
    OnClick = Button5Click
  end
end
