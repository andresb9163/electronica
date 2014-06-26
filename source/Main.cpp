// Copyright 2014 FaMAF

#include <vcl\vcl.h>
#pragma hdrstop

#include "./mmsystem.h"
#include "./pp2.h"
#include "./Main.h"
#include "./usb.h"

#pragma resource "*.dfm"
TMainForm *MainForm;
pp2             cont_pp2;
#define ERR_BUFFER 65535
#define MAXIMOS_INTENTOS 100


long double frec_1, frec_2, calculo;
unsigned int w1_0, w1_1, w1_2, w1_3, w1_4, w1_5, cuenta;
unsigned int w2_0, w2_1, w2_2, w2_3, w2_4, w2_5;
unsigned int fs1_l, fs1_h, fs2_l, fs2_h, fs3_l, fs3_h, fs4_l, fs4_h;
unsigned int fase1, fase2, fase3, fase4;
int activado;
long int t, h;


__fastcall TMainForm::TMainForm(TComponent* Owner): TForm(Owner) {}


void __fastcall TMainForm::SALIRClick(TObject *Sender) {
    Close();
}


void __fastcall TMainForm::TRANSFERIRClick(TObject *Sender) {
    usb.SetComando('R'); // resetear
    usb.direccion(0x50);
    usb.parametro(0x02);
    usb.EnviarDatos_Respuesta();
    usb.SetComando('c'); // habilitar modo carga
    usb.direccion(0x50);
    usb.parametro(0x03);
    usb.EnviarDatos_Respuesta();

    cont_pp2.transferir_programa ();
}


void __fastcall TMainForm::DISPAROClick(TObject *Sender)
{
    usb.SetComando('D');
    usb.direccion(0x50);
    usb.parametro(0x00);  // modo microprocesador
    usb.parametro(0x80);  // disparo.
    usb.EnviarDatos_Respuesta();
}


void __fastcall TMainForm::BotCargarClick(TObject *Sender) {
    TStringList* Lista_Prog = new TStringList();  // Listado del programa
    char* filename = new char[4096];
    char  buffer[4096];
    int   i, j, nl;
    int   error;
    char  *linea = NULL;

    if (OpenDialog1->Execute()) {
    linea = (char *)malloc(sizeof(char)*4096);

    // En este punto, obtengo el nombre del archivo
    Label2->Caption = OpenDialog1->FileName;

    // Obtengo el índice de la úlitma ´/´, para poder
    // extraer únicamente el nombre del archivo, sin el path
    j = 0;

    for (i = 0; i < Label2->Caption.Length() - 1; i++) {
        if (Label2->Caption.SubString(i, 1) == "\\") {
            j = i+1;
        }
    }

    strncpy(filename, Label2->Caption.c_str(), 4096);
    Label2->Caption = Label2->Caption.SubString(j, 255);
    //  Punto donde se proccesa el archivo de texto
    error = cont_pp2.compilar(filename, &nl, &linea);
    if (error) {
        Label2->Caption = "Ninguna";
    } else {
      // Agrego los comandos al listado de programa
        Lista_Prog->Add(filename);
        CmbProg->Items->AddStrings(Lista_Prog);
    }
    free(linea);
    linea = NULL;
    }
}


void __fastcall TMainForm::BotBorrarClick(TObject *Sender) {
    CmbProg->Items->Clear();
}


void __fastcall TMainForm::TsClick(TObject *Sender) {
    unsigned int delta_t;
    double muestras;
    int mps =(atoi(TxtMuestreo->Text.c_str()));


    usb.SetComando('t');
    usb.direccion(0x0C);

    if (mps * .0000001 != 0) {
        muestras = 1. / (mps * .0000001);
        delta_t = 255 - (int) muestras;
        usb.parametro(delta_t);
        usb.EnviarDatos_Respuesta();
    }
}


void __fastcall TMainForm::ADQUIRIRClick(TObject *Sender) {
    TADModoAdquisicion mAdq;
    int i;
    int intentos = 0;

    mAdq = TADModoAdquisicion(RadioGroupAdq->ItemIndex);

    // modoAdq - K - reset - modo
    ADSetConfig(mAdq, CmbBarrido->ItemIndex, 1, 0);
    ADSetConfig(mAdq, CmbBarrido->ItemIndex, 0, 1);
    DISPAROClick(DISPARO);
    // ---- Delay para detectar final de conversion -----
    do {
        // Redaccion y envio del pedido de status por soft.
        usb.RxBuffer[1] = 0;
        usb.SetComando('S');
        usb.direccion(0x0b);
        usb.EnviarDatos_Respuesta();
        if ( usb.RxBuffer[1] & 0x01 ) {
            /*
             El primer byte es el comando, el segundo es el valor leido de
             respuesta. OK-S- la deberia perderse (?)
            */
            intentos = MAXIMOS_INTENTOS;
        } else {
            intentos++;
            Sleep(50);
        }
    } while ( intentos < MAXIMOS_INTENTOS );

    pedir_buffer();
}


void __fastcall TMainForm::ADSetConfig(int modoAdq, int k, int reset, int modo) {
    int config = 0x00;

    switch (k) {
        case 0: config = 0x00; break;
        case 1: config = 0x10; break;
        case 2: config = 0x20; break;
        case 3: config = 0x30; break;
        case 4: config = 0x40; break;
        case 5: config = 0x50; break;
        case 6: config = 0x60; break;
        case 7: config = 0x70; break;
        default: {}
    }

    switch (modoAdq) {
        case mContinua: config = config | 0x02;  break;
        case mModulada: config = config & 0xFD;  break;
        default: {}
    }

    switch (reset) {
        case 0: config = config & 0x7f; break;
        case 1: config = config | 0x80; break;
        default: {}
    }

    switch (modo) {
        case 0: config = config & 0xfe; break;
        case 1: config = config | 0x01; break;
        default: {}
    }

    usb.SetComando('r');
    usb.direccion(0x0b);
    usb.parametro(config);    // Modo microprocesador
    usb.EnviarDatos_Respuesta();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::pedir_buffer() {
     DWORD EndTime, startTime, velocidad;
     TADModoAdquisicion mAdq;
     int i;

     mAdq = TADModoAdquisicion(RadioGroupAdq->ItemIndex);

     // modoAdq - K - reset - modo
     ADSetConfig(mAdq, CmbBarrido->ItemIndex, 1, 0);
     ADSetConfig(mAdq, CmbBarrido->ItemIndex, 0, 0);

     LeerUnCanal(0x08, usb.Canal1);  // Lectura canal B
     // modoAdq - K - reset - modo
     ADSetConfig(mAdq, CmbBarrido->ItemIndex, 1, 0);
     ADSetConfig(mAdq, CmbBarrido->ItemIndex, 0, 0);

     LeerUnCanal(0x0A, usb.Canal2);  // Lectura canal A

     // modoAdq - K - reset - modo
     ADSetConfig(mAdq, CmbBarrido->ItemIndex, 1, 0);
     ADSetConfig(mAdq, CmbBarrido->ItemIndex, 0, 0);

     LeerUnCanal(0x09, usb.Canal3);  // Lectura canal AB

     RealizarOpCanalAB(1024*(atoi(CmbBarrido->Text.c_str())));
     TxtMin->Text = "0";
     TxtMax->Text = "1024";
     dibuja();
}


//------------------------------------------------------------------------
void __fastcall TMainForm::LeerUnCanal(int direccion, char * puntero) {
    DWORD EndTime, startTime, velocidad;
    unsigned int cantidad_bytes =0;

    usb.SetComando('B');
    usb.direccion(direccion);
    usb.soloEnviarDatos();

    switch (CmbBarrido->ItemIndex) {
        case 0: cantidad_bytes = 1024; break;
        case 1: cantidad_bytes = 2*1024; break;
        case 2: cantidad_bytes = 4*1024; break;
        case 3: cantidad_bytes = 8*1024; break;
        case 4: cantidad_bytes = 16*1024; break;
        case 5: cantidad_bytes = 32*1024; break;
        case 6: cantidad_bytes = 64*1024; break;
        case 7: cantidad_bytes = 128*1024; break;
        default: cantidad_bytes = 1024;
    }

    usb.soloEsperarRespuesta(cantidad_bytes, puntero);
}


void __fastcall TMainForm::RealizarOpCanalAB(int nByte) {
    int i;

    for (i = 0; i < nByte; i++) {
        usb.CanalB[i]= ((usb.Canal3[i])&0x0f)+(usb.Canal1[i]*16);
        usb.CanalA[i]= ((usb.Canal3[i]/16)&0x0f)+((usb.Canal2[i])*16);
    }

    // Reinicializa los canales a partir de la possiciones no utilizadas
    for (i = nByte; i < MAXRXBUFF; i++) {
        usb.Canal1[i]= 0;
        usb.Canal2[i]= 0;
        usb.Canal3[i]= 0;
    }
}


void __fastcall TMainForm::LEERClick(TObject *Sender) {
    pedir_buffer();
}


void __fastcall TMainForm::BotInicioClick(TObject *Sender) {
    TxtMin->Text = "0";
    TxtMax->Text = "1024";
    dibuja();
}


void __fastcall TMainForm::BotAntClick(TObject *Sender) {
    if (atoi(TxtMin->Text.c_str()) > 0) {
        TxtMin->Text = atoi(TxtMin->Text.c_str()) - 1024;
        TxtMax->Text = atoi(TxtMax->Text.c_str()) - 1024;
        dibuja();
    }
}


void __fastcall TMainForm::BotSigClick(TObject *Sender) {
    if ((atoi(TxtMax->Text.c_str()) < 128*1024) &
      (atoi(TxtMax->Text.c_str()) < atoi(CmbBarrido->Text.c_str()) * 1024)) {
        TxtMin->Text = atoi(TxtMin->Text.c_str()) + 1024;
        TxtMax->Text = atoi(TxtMax->Text.c_str()) + 1024;
        dibuja();
    }
}


void __fastcall TMainForm::BotUltClick(TObject *Sender) {
    TxtMin->Text = atoi(CmbBarrido->Text.c_str())*1024 - 1024;
    TxtMax->Text = atoi(CmbBarrido->Text.c_str())*1024;
    dibuja();
}


void __fastcall TMainForm::CODIGO_AClick(TObject *Sender) {
    usb.SetComando('d');
    usb.direccion(0x0d);
    usb.parametro(0x00);

    usb.EnviarDatos_Respuesta();

    AnsiString MyAnsiString = IntToStr(usb.RxBuffer[0]);
    Edit1->Text = MyAnsiString;
}


void __fastcall TMainForm::FTW1Click(TObject *Sender) {
    frec_1 = (atoi(Edit2->Text.c_str()));  // frecuencia 1 deseada

    // la ecuación para obtener el valor del registro de frecuencia
    // para una frecuencia deseada es:
    //
    //    valor = frec * 2^48 /  fMCLK.     fMCLK =  300MHZ ó 200MHZ
    // obtener los seis bytes a cargar en el registro de frecuencia
    // calculo =  frec_1 * 281474976710656 / 300000000;

    calculo =  frec_1 * 281474976710656 / 200000000;

    w1_5 = calculo / 1099511627776;  // divide por 2^40
    calculo = calculo - (w1_5 * 1099511627776);

    w1_4 = calculo / 4294967296;  // divide por 2^32
    calculo = calculo - (w1_4 * 4294967296);

    w1_3 = calculo / 16777216;  // divide por 2^24
    calculo = calculo - (w1_3 * 16777216);

    w1_2 = calculo / 65536;  // divide por 2^16
    calculo = calculo - (w1_2 * 65536);

    w1_1 = calculo / 256;  // divide por 2^8
    w1_0 = calculo - (w1_1 * 256);

    /************* transferir la frecuencia 1 al dds **************/
    usb.SetComando('b');
    usb.direccion(0x71);  // modo PC
    usb.parametro(0x00);
    usb.EnviarDatos_Respuesta();

    // cargar 6 registros de frec. del DDS
    // direccionar los registros de frecuencias y cargar con FTW1

    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 04 del DDS
    usb.parametro(0x04);
    usb.parametro(0x78);
    usb.parametro(w1_5);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // FIXME: What the hell is this!!!!!!
    for (h = 0; h < 400000; h++);

    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 05 del DDS
    usb.parametro(0x05);
    usb.parametro(0x78);
    usb.parametro(w1_4);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // FIXME: What the hell is this!!!!!!
    for (h = 0; h < 400000; h++);

    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 06 del DDS
    usb.parametro(0x06);
    usb.parametro(0x78);
    usb.parametro(w1_3);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // FIXME: What the hell is this!!!!!!
    for (h = 0; h < 400000; h++);

    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 07 del DDS
    usb.parametro(0x07);
    usb.parametro(0x78);
    usb.parametro(w1_2);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // FIXME: What the hell is this!!!!!!
    for (h = 0; h < 400000; h++);

    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 08 del DDS
    usb.parametro(0x08);
    usb.parametro(0x78);
    usb.parametro(w1_1);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // FIXME: What the hell is this!!!!!!
    for (h = 0; h < 400000; h++);

    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 09 del DDS
    usb.parametro(0x09);
    usb.parametro(0x78);
    usb.parametro(w1_0);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // actualizar los registros internos
    usb.SetComando('u');
    usb.direccion(0x76);  // direcionar registro de comando  UDCLK
    usb.parametro(0x00);
    usb.EnviarDatos_Respuesta();
}


void __fastcall TMainForm::FTW2Click(TObject *Sender) {
    frec_2 = (atoi(Edit3->Text.c_str()));  // frecuencia 2 deseada
    // la ecuación para obtener el valor del registro de frecuencia
    // para una frecuencia deseada es:
    //
    // frec * 2^48 /  fMCLK.     fMCLK =  50MHZ

    // obtener los seis bytes a cargar en el registro de frecuencia

    // calculo =  frec_2 * 281474976710656 / 300000000;

    calculo =  frec_2 * 281474976710656 / 200000000;

    w2_5 = calculo / 1099511627776;  // divide por 2^40
    calculo = calculo - (w2_5 * 1099511627776);

    w2_4 = calculo / 4294967296;  // divide por 2^32
    calculo = calculo - (w2_4 * 4294967296);

    w2_3 = calculo / 16777216;  // divide por 2^24
    calculo = calculo - (w2_3 * 16777216);

    w2_2 = calculo / 65536;  // divide por 2^16
    calculo = calculo - (w2_2 * 65536);

    w2_1 = calculo / 256;  // divide por 2^8
    w2_0 = calculo - (w2_1 * 256);

    /************* transferir la frecuencia 2 al dds **************/
    usb.SetComando('b');
    usb.direccion(0x71);  // modo PC
    usb.parametro(0x00);
    usb.EnviarDatos_Respuesta();
    // cargar 6 registros de frec. del DDS
    // direccionar los registros de frecuencias y cargar con FTW2

    // cargar un registro 0a del DDS2 con w2_5
    // k 75 0a 78 w2_5
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 04 del DDS
    usb.parametro(0x0a);
    usb.parametro(0x78);
    usb.parametro(w2_5);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // FIXME: What the hell is this!!!!!!
    for (h = 0; h < 400000; h++);

    // cargar un registro 0b del DDS2 con w2_4
    // k 75 0b 78 w2_4
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 05 del DDS
    usb.parametro(0x0b);
    usb.parametro(0x78);
    usb.parametro(w2_4);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // FIXME: What the hell is this!!!!!!
    for (h = 0; h < 400000; h++);

    // cargar un registro 0c del DDS2 con w2_3
    // k 75 0c 78 w2_3
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 06 del DDS
    usb.parametro(0x0c);
    usb.parametro(0x78);
    usb.parametro(w2_3);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // FIXME: What the hell is this!!!!!!
    for (h = 0; h < 400000; h++);

    // cargar un registro 0d del DDS2 con w2_2
    // k 75 0d 78 w2_2
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 07 del DDS
    usb.parametro(0x0d);
    usb.parametro(0x78);
    usb.parametro(w2_2);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // FIXME: What the hell is this!!!!!!
    for (h = 0; h < 400000; h++);

    // cargar un registro 0e del DDS2 con w2_1
    // k 75 0e 78 w2_1
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 08 del DDS
    usb.parametro(0x0e);
    usb.parametro(0x78);
    usb.parametro(w2_1);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // FIXME: What the hell is this!!!!!!
    for (h = 0; h < 400000; h++);

    // cargar un registro 0f del DDS2 con w2_0
    // k 75 0f 78 w2_0
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 09 del DDS
    usb.parametro(0x0f);
    usb.parametro(0x78);
    usb.parametro(w2_0);  // cargar dato en FTW1
    usb.EnviarDatos_Respuesta();

    // actualizar los registros internos

    // u 76 00
    // pulso UDCLK
    usb.SetComando('u');
    usb.direccion(0x76);  // direcionar registro de comando  UDCLK
    usb.parametro(0x00);
    usb.EnviarDatos_Respuesta();

    // actualizar los registros internos

    // u 76 00
    // pulso UDCLK
    usb.SetComando('u');
    usb.direccion(0x76);  // direcionar registro de comando  UDCLK
    usb.parametro(0x00);
    usb.EnviarDatos_Respuesta();
}


void __fastcall TMainForm::RESETClick(TObject *Sender) {
    // b 71 00
    // pasar a modo PC  (registro de comando 2)
    usb.SetComando('b');
    usb.direccion(0x71);  // modo PC
    usb.parametro(0x00);
    usb.EnviarDatos_Respuesta();

    // resetear y desactivar el DDS
    // Master RESET

    // a 72 00
    // pulso de RESET (registro de comando 3)
    usb.SetComando('a');
    usb.direccion(0x72);  // reset DDS2
    usb.parametro(0x00);
    usb.EnviarDatos_Respuesta();

    // FIXME: What the hell is this!!!!!!
    for (h = 0; h < 400000; h++);

    // configurar el DDS
    // k 75 <dir reg DDS2> 78 <dato> (El <dato> se graba en  <dir reg DDS2>

    // cargar un registro 1f del DDS2 con 02
    // k 75 1f 78 00
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 1f
    usb.parametro(0x1f);
    usb.parametro(0x78);
    usb.parametro(0x02);  // modo FSK, UDCLK externo
    usb.EnviarDatos_Respuesta();


    // cargar un registro 1d del DDS2 con 0x17
    // k 75 1d 78 17
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 1d
    usb.parametro(0x1d);
    usb.parametro(0x78);
    usb.parametro(0x17);  // 00010111, todo en PowerDown
    usb.EnviarDatos_Respuesta();


    // cargar un registro 1e del DDS2 con 0x44
    // k 75 1e 78 44
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 1e
    usb.parametro(0x1e);
    usb.parametro(0x78);
    usb.parametro(0x44);  // PLL out 0x44 = 200Mhz, 0x45 = 250Mhz, 0x46 = 300Mhz
    usb.EnviarDatos_Respuesta();


    // cargar un registro 20 del DDS2 con 00
    // k 75 20 78 00
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 1d
    usb.parametro(0x20);
    usb.parametro(0x78);
    usb.parametro(0x00);  // Shape keying deshabilitado
    // filtro deshabilitado
    usb.EnviarDatos_Respuesta();

    StaticText1 ->Caption = "DDS  DESACTIVADO";
}


void __fastcall TMainForm::ACTIVARClick(TObject *Sender) {
    // inicializar los registros de control

    // b 71 00
    // pasar a modo PC  (registro de comando 2)
    usb.SetComando('b');
    usb.direccion(0x71);  // modo PC
    usb.parametro(0x00);
    usb.EnviarDatos_Respuesta();

    // cargar un registro 1d del DDS2 con 0x10
    // k 75 1d 78 10
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 1d
    usb.parametro(0x1d);
    usb.parametro(0x78);
    usb.parametro(0x10);  // 00010000, todo habilitado
    usb.EnviarDatos_Respuesta();

    // activar el cargador de fase del ALTERA
    // b 71 05
    // pasar a modo DDS con fases  (registro de comando 2)
    usb.SetComando('b');
    usb.direccion(0x71);  // modo DDS con fases
    usb.parametro(0x05);
    usb.EnviarDatos_Respuesta();

    activado = 1;  // indicar activado.

    StaticText1 ->Caption = "DDS  ACTIVADO";
}


void __fastcall TMainForm::DESACTIVARClick(TObject *Sender) {
    // b 71 00
    // pasar a modo PC  (registro de comando 2)
    usb.SetComando('b');
    usb.direccion(0x71);  // modo PC
    usb.parametro(0x00);
    usb.EnviarDatos_Respuesta();

    // cargar un registro 1d del DDS2 con 0x17
    // k 75 1d 78 17
    usb.SetComando('k');
    usb.direccion(0x75);  // direccionar registro 1d
    usb.parametro(0x1d);
    usb.parametro(0x78);
    usb.parametro(0x17);  // 00010111, todo en PowerDown
    usb.EnviarDatos_Respuesta();

    // actualizar los registros internos

    // u 76 00
    // pulso UDCLK
    usb.SetComando('u');
    usb.direccion(0x76);  // direcionar registro de comando  UDCLK
    usb.parametro(0x00);
    usb.EnviarDatos_Respuesta();

    activado = 0;  // indicar desactivado.

    StaticText1 ->Caption = "DDS  DESACTIVADO";
}


void __fastcall TMainForm::FASE_1Click(TObject *Sender) {
    if (!activado) {
        fase1 = (atoi(Edit4->Text.c_str()));
        if (fase1 > 360) {
            fase1 = 0;
            StaticText2 ->Caption = "ERROR. fase > = 360";
        } else {
            StaticText2 ->Caption = Edit4->Text;
        }
        fase1 = 45.508 * fase1;  // convertir a valor de 14 bits
        fs1_h = fase1 / 256;  // divide por 2^8
        fs1_l = fase1 - (fs1_h * 256);
        // habilitar la escritura de la RAM FASE
        // b 71 02
        // pasar a modo PC escritura RAM Fases (registro de comando 2)
        usb.SetComando('b');
        usb.direccion(0x71);
        usb.parametro(0x02);  // modo PC escritura RAM Fases
        usb.EnviarDatos_Respuesta();
        // direccionar los registros de fase, RAM FASE en el ALTERA
        // w 70 00 74 fs1_h 70 01 74 fs1_l
        usb.SetComando('w');
        usb.direccion(0x70);  // direcionar registro PH_00_h de la RAM Fases
        usb.parametro(0x00);
        usb.parametro(0x74);  // cargar dato en RAM FASE
        usb.parametro(fs1_h);
        usb.direccion(0x70);  // direcionar registro PH_00_l de la RAM Fases
        usb.parametro(0x01);
        usb.parametro(0x74);  // cargar dato en RAM FASE
        usb.parametro(fs1_l);
        usb.EnviarDatos_Respuesta();
        // deshabilitar escritura de la RAM FASES
        // b 71 00
        // pasar a modo PC  (registro de comando 2)
        usb.SetComando('b');
        usb.direccion(0x71);
        usb.parametro(0x00);   // modo PC
        usb.EnviarDatos_Respuesta();
    } else {
        ShowMessage("ATENCION Desactivar para cambiar fase");
    }
}


void __fastcall TMainForm::FASE_2Click(TObject *Sender) {
    if (!activado) {
        fase2 = (atoi(Edit4->Text.c_str()));
        if (fase2 > 360) {
            fase2 = 0;
            StaticText3 ->Caption = "ERROR. fase < = 360";
        } else {
            StaticText3 ->Caption = Edit4->Text;
        }
        fase2 = 45.51111 * fase2;  // convertir a valor de 14 bits
        fs2_h = fase2 / 256;  // divide por 2^8
        fs2_l = fase2 - (fs2_h * 256);
        // habilitar la escritura de la RAM FASE
        // b 71 02
        // pasar a modo PC escritura RAM Fases (registro de comando 2)
        usb.SetComando('b');
        usb.direccion(0x71);
        usb.parametro(0x02);  // modo PC escritura RAM Fases
        usb.EnviarDatos_Respuesta();
        // direccionar los registros de fase, RAM FASE en el ALTERA
        // w 70 02 74 fs2_h 70 03 74 fs2_l
        usb.SetComando('w');
        usb.direccion(0x70);  // direcionar registro PH_01_h de la RAM Fases
        usb.parametro(0x02);
        usb.parametro(0x74);  // cargar dato en RAM FASE
        usb.parametro(fs2_h);
        usb.direccion(0x70);  // direcionar registro PH_01_l de la RAM Fases
        usb.parametro(0x03);
        usb.parametro(0x74);  // cargar dato en RAM FASE
        usb.parametro(fs2_l);
        usb.EnviarDatos_Respuesta();
        // b 71 00
        // pasar a modo PC  (registro de comando 2)
        usb.SetComando('b');
        usb.direccion(0x71);
        usb.parametro(0x00);  // modo PC
        usb.EnviarDatos_Respuesta();
    } else {
        ShowMessage("ATENCION Desactivar para cambiar fase");
    }
}


void __fastcall TMainForm::FASE_3Click(TObject *Sender) {
    if (!activado) {
        fase3 = (atoi(Edit4->Text.c_str()));
        if (fase3 > 360) {
            fase3 = 0;
            StaticText4 ->Caption = "ERROR. fase < = 360";
        } else {
            StaticText4 ->Caption = Edit4->Text;
        }
        fase3 = 45.51111 * fase3;  // convertir a valor de 14 bits
        fs3_h = fase3 / 256;  // divide por 2^8
        fs3_l = fase3 - (fs3_h * 256);
        // habilitar la escritura de la RAM FASE
        // b 71 02
        // pasar a modo PC escritura RAM Fases (registro de comando 2)
        usb.SetComando('b');
        usb.direccion(0x71);
        usb.parametro(0x02);  // modo PC escritura RAM Fases
        usb.EnviarDatos_Respuesta();
        // direccionar los registros de fase, RAM FASE en el ALTERA
        // w 70 04 74 fs3_h 70 05 74 fs3_l
        usb.SetComando('w');
        usb.direccion(0x70);  // direcionar registro PH_02_h de la RAM Fases
        usb.parametro(0x02);
        usb.parametro(0x74);  // cargar dato en RAM FASE
        usb.parametro(fs3_h);
        usb.direccion(0x70);  // direcionar registro PH_02_l de la RAM Fases
        usb.parametro(0x03);
        usb.parametro(0x74);  // cargar dato en RAM FASE
        usb.parametro(fs3_l);
        usb.EnviarDatos_Respuesta();
        // b 71 00
        // pasar a modo PC  (registro de comando 2)
        usb.SetComando('b');
        usb.direccion(0x71);
        usb.parametro(0x00);  // modo PC
        usb.EnviarDatos_Respuesta();
        // deshabilitar escritura de la RAM FASES
        // b 71 00
        // pasar a modo PC  (registro de comando 2)
        usb.SetComando('b');
        usb.direccion(0x71);
        usb.parametro(0x00);  // modo PC
        usb.EnviarDatos_Respuesta();
    } else {
        ShowMessage("ATENCION Desactivar para cambiar fase");
    }
}


void __fastcall TMainForm::FASE_4Click(TObject *Sender) {
    if (!activado) {
        fase4 = (atoi(Edit4->Text.c_str()));
        if (fase4 > 360) {
            fase4 = 0;
            StaticText5 ->Caption = "ERROR. fase < = 360";
        } else {
            StaticText5 ->Caption = Edit4->Text;
        }
        fase4 = 45.51111 * fase4;  // convertir a valor de 14 bits
        fs4_h = fase4 / 256;  // divide por 2^8
        fs4_l = fase4 - (fs4_h * 256);
        // habilitar la escritura de la RAM FASE
        // b 71 02
        // pasar a modo PC escritura RAM Fases (registro de comando 2)
        usb.SetComando('b');
        usb.direccion(0x71);
        usb.parametro(0x02);  // modo PC escritura RAM Fases
        usb.EnviarDatos_Respuesta();
        // direccionar los registros de fase, RAM FASE en el ALTERA
        // w 70 06 74 fs4_h 70 07 74 fs4_l
        usb.SetComando('w');
        usb.direccion(0x70);  // direcionar registro PH_03_h de la RAM Fases
        usb.parametro(0x06);
        usb.parametro(0x74);  // cargar dato en RAM FASE
        usb.parametro(fs4_h);
        usb.direccion(0x70);  // direcionar registro PH_03_l de la RAM Fases
        usb.parametro(0x07);
        usb.parametro(0x74);  // cargar dato en RAM FASE
        usb.parametro(fs4_l);
        usb.EnviarDatos_Respuesta();
        // deshabilitar escritura de la RAM FASES
        // b 71 00
        // pasar a modo PC  (registro de comando 2)
        usb.SetComando('b');
        usb.direccion(0x71);
        usb.parametro(0x00);  // modo PC
        usb.EnviarDatos_Respuesta();
    } else {
        ShowMessage("ATENCION Desactivar para cambiar fase");
    }
}

