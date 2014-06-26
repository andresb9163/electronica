// Copyright 2014 FaMAF

//---------------------------------------------------------------------------
#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <vcl\sysutils.hpp>
#include <vcl\windows.hpp>
#include <vcl\messages.hpp>
#include <vcl\classes.hpp>
#include <vcl\graphics.hpp>
#include <vcl\controls.hpp>
#include <vcl\forms.hpp>
#include <vcl\dialogs.hpp>
#include <vcl\stdctrls.hpp>
#include <vcl\buttons.hpp>
#include <vcl\extctrls.hpp>
#include <vcl\menus.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>

#include "./usb.h"
#include "./grafico.h"


enum TADModoAdquisicion {mContinua, mModulada};

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:
        TButton *SALIR;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TOpenDialog *OpenDialog1;
        TComboBox *CmbProg;
        TComboBox *CmbLog;
        TButton *BotCargar;
        TButton *BotBorrar;
        TButton *TRANSFERIR;
        TButton *DISPARO;
        TTabSheet *TabSheet2;
        TButton *Ts;
        TButton *LEER;
        TButton *ADQUIRIR;
        TButton *CODIGO_A;
        TRadioGroup *RadioGroupAdq;
        TEdit *TxtMuestreo;
        TComboBox *CmbBarrido;
        TLabel *Label3;
        TLabel *Label4;
        TTabSheet *TabSheet3;
        TGroupBox *GroupBox2;
        TEdit *TxtMin;
        TEdit *TxtMax;
        TLabel *Label5;
        TLabel *Label6;
        TBitBtn *BotInicio;
        TBitBtn *BotAnt;
        TBitBtn *BotSig;
        TBitBtn *BotUlt;
        TImage *Image1;
        TEdit *Edit1;
        TTabSheet *TabSheet4;
        TGroupBox *GroupBox3;
        TGroupBox *GroupBox4;
        TGroupBox *GroupBox5;
        TEdit *Edit2;
        TEdit *Edit3;
        TButton *FTW1;
        TButton *FTW2;
        TButton *RESET;
        TButton *ACTIVAR;
        TButton *DESACTIVAR;
        TStaticText *StaticText1;
        TEdit *Edit4;
        TButton *FASE_1;
        TButton *FASE_2;
        TButton *FASE_3;
        TButton *FASE_4;
        TStaticText *StaticText2;
        TStaticText *StaticText3;
        TStaticText *StaticText4;
        TStaticText *StaticText5;
        void __fastcall SALIRClick(TObject *Sender);
        void __fastcall TRANSFERIRClick(TObject *Sender);
        void __fastcall DISPAROClick(TObject *Sender);
        void __fastcall BotCargarClick(TObject *Sender);
        void __fastcall BotBorrarClick(TObject *Sender);
        void __fastcall TsClick(TObject *Sender);
        void __fastcall ADQUIRIRClick(TObject *Sender);
        void __fastcall LEERClick(TObject *Sender);
        void __fastcall BotInicioClick(TObject *Sender);
        void __fastcall BotAntClick(TObject *Sender);
        void __fastcall BotSigClick(TObject *Sender);
        void __fastcall BotUltClick(TObject *Sender);
        void __fastcall CODIGO_AClick(TObject *Sender);
        void __fastcall FTW1Click(TObject *Sender);
        void __fastcall FTW2Click(TObject *Sender);
        void __fastcall RESETClick(TObject *Sender);
        void __fastcall ACTIVARClick(TObject *Sender);
        void __fastcall DESACTIVARClick(TObject *Sender);
        void __fastcall FASE_1Click(TObject *Sender);
        void __fastcall FASE_2Click(TObject *Sender);
        void __fastcall FASE_3Click(TObject *Sender);
        void __fastcall FASE_4Click(TObject *Sender);
private:        // private user declarations

       void  __fastcall ADSetConfig(int , int , int , int );
       void __fastcall pedir_buffer();


public:         // public user declarations

PuertoUSB usb;
        void __fastcall  LeerUnCanal( int direccion, char * puntero );
        void __fastcall  RealizarOpCanalAB(int nByte);


	virtual __fastcall TMainForm(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
