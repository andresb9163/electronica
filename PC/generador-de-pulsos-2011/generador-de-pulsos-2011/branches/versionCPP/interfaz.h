//---------------------------------------------------------------------------

#ifndef interfazH
#define interfazH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include "TDLPortIO.h"
//---------------------------------------------------------------------------
class TformPrincipal : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *dialogoAbrirArchivo;
        TLabel *etiquetaStatus;
        TPanel *Panel1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label9;
        TEdit *campoHojaPulsos;
        TButton *botonCargarHojaPulsos;
        TButton *botonCargarConfiguracion;
        TEdit *campoCargarConfig;
        TStringGrid *StringGridDelays;
        TGroupBox *GroupBox2;
        TLabel *Label7;
        TLabel *Label8;
        TEdit *campoDDSFrecuencia1;
        TEdit *campoDDSFrecuencia2;
        TButton *botonEncenderDDS;
        TButton *botonApagarDDS;
        TButton *botonResetearDDS;
        TGroupBox *GroupBox3;
        TLabel *Label10;
        TRadioGroup *grupoModoOperacion;
        TRadioGroup *grupoTamBuffer;
        TEdit *campoMuestrasPorSegundo;
        TStringGrid *StringGrid1;
        TComboBox *ComboBox1;
        TButton *botonParsearHoja;
        TButton *botonEjecutar;
        TEdit *Repeticiones;
        TCheckBox *CheckBox1;
        TEdit *Edit1;
        TButton *botonGuardarAdquisiciones;
        TCheckBox *CheckBox2;
        TDLPortIO *DLPortIO1;
        void __fastcall botonCargarHojaPulsosClick(TObject *Sender);
    void __fastcall botonParsearHojaClick(TObject *Sender);
    void __fastcall botonCargarConfiguracionClick(TObject *Sender);
        void __fastcall botonCargarDelaysClick(TObject *Sender);
        void __fastcall ComboBox1Click(TObject *Sender);
        void __fastcall botonEncenderDDSClick(TObject *Sender);
        void __fastcall botonEjecutarClick(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall CheckBox2Click(TObject *Sender);
        void __fastcall botonGuardarAdquisicionesClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
        __fastcall TformPrincipal(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformPrincipal *formPrincipal;
//---------------------------------------------------------------------------
#endif
