//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "formGrafico.h"
#include <fstream.h>
#include <sstream.h>
#include "interfaz.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define ERR_BUFFER (64*1024)-1


TformGraficos *formGraficos;
double canala[64*1024];
double canalb[64*1024];
unsigned int tamBuffer;

void llenarCanalB(char *path){
             ifstream myReadFile;
    myReadFile.open(path);
    char output[100];
    char dato[100];
    int j = 0, i =0, r = 0;
    tamBuffer = -2; // Porque el archivo tiene 3 lineas de mas

    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {

            myReadFile.getline(output, 100);
            tamBuffer++;
            while(output[i] != '\t')
                i++;
            i+=8;
            while(output[i] != '\0'){
                dato[r] = output[i];
                i++;
                r++;
            }
            dato[r] = '\0';
            canalb[j] = atoi(dato);
            j++;
            i=0;
            r = 0;
        }

    }
    /*float delta = 1/57.295779513082320876798154814105;
    float x = 0.;
    for(i=0;i<64*1024;i++){
        canalb[i] = (2047*sin(x)) + 2047;
        x += delta;
    }      */
    myReadFile.close();
    tamBuffer /= 1024;
}

void llenarCanalA(char *path){
             ifstream myReadFile;
    myReadFile.open(path);
    char output[100];
    char dato[100];
    int j = 0, i =7, r = 0;

    tamBuffer = -2; // Porque el archivo tiene 3 lineas de mas
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {

            myReadFile.getline(output, 100);
            tamBuffer++;
            while(output[i] != '\t'){
                dato[r] = output[i];
                i++;
                r++;
            }
            dato[r] = '\0';
            canala[j] = atoi(dato);
            j++;
            i=7;
            r = 0;
        }

    }
    /*float delta = 1/57.295779513082320876798154814105;
    float x = 0.;
    for(i=0;i<64*1024;i++){
        canala[i] = 2047*cos(x) + 2047;
        x += delta;
    }   */
    myReadFile.close();
    tamBuffer /= 1024;
}
void dibuja(){
        unsigned int    i,j;
        //Limpia la imagen
        formGraficos->Image1->Canvas->FloodFill(formGraficos->ClientWidth/4,formGraficos->ClientHeight/4,clMenu, fsBorder);

        for(i=0; i<StrToInt(formGraficos->TxtMax->Text)-
                StrToInt(formGraficos->TxtMin->Text); i++)
        {
                //Dibuja los puntos del canal A
                j=canala[StrToInt(formGraficos->TxtMin->Text)+i];
                if(j <= ERR_BUFFER)
                {
                        formGraficos->Image1->Canvas->Pixels[i][512-j/8]=clBlue;
                }
                else
                {
                        formGraficos->Image1->Canvas->Pixels[i][512]=clBlue;
                }

                //Dibuja los puntos del canal B
                j=canalb[StrToInt(formGraficos->TxtMin->Text)+i];
                if(j <= ERR_BUFFER)
                {
                        formGraficos->Image1->Canvas->Pixels[i][512-j/8]=clRed;
                }
                else
                {
                        formGraficos->Image1->Canvas->Pixels[i][512]=clRed;
                }
        }

}
/*
void dibuja()
{
        unsigned int    i,j;
        //Limpia la pantalla
        formGraficos->Image1->Canvas->FloodFill(formGraficos->ClientWidth/4,
        formGraficos->ClientHeight/4,clMenu, fsBorder);
        int min = StrToInt(formGraficos->TxtMin->Text);
        int max = StrToInt(formGraficos->TxtMax->Text);
        for(i=0; i<max - min; i++)
        {
                //Dibuja los puntos del canal A

                j=canala[min+i];
                formGraficos->Image1->Canvas->Pixels[i][512-j/8]=clBlue;


                //Dibuja los puntos del canal B ESTA CORRIDO 10 LUGARES A PROPOSITO!!!!!!
                j=canalb[min+i];
                formGraficos->Image1->Canvas->Pixels[i+10][512-j/8]=clRed;
        }


} */


//---------------------------------------------------------------------------
__fastcall TformGraficos::TformGraficos(TComponent* Owner)
        : TForm(Owner)
{
    llenarCanalA("output_ad.txt");
    llenarCanalB("output_ad.txt");
    dibuja();

}
//---------------------------------------------------------------------------

void __fastcall TformGraficos::Button2ClickClick(TObject *Sender)
{
    int             val_min, val_max;               //Rango a graficar

        val_min=StrToInt(formGraficos->TxtMin->Text);
        val_max=StrToInt(formGraficos->TxtMax->Text);
        //Desplazamiento hacia la derecha de 128 píxeles
        val_min=val_min+128;
        val_max=val_max+128;

        //Chequeo de condiciones en el borde superior
        if(val_max>tamBuffer*1024)
        {
                val_max=tamBuffer*1024;
                val_min -= 128;
        }
        TxtMax->Text=val_max;
        TxtMin->Text=val_min;

        //Actualiza la imagen
        dibuja();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TformGraficos::Button4Click(TObject *Sender)
{
        TxtMin->Text="0";
        TxtMax->Text="1024";
        if(StrToInt(TxtMax->Text)>tamBuffer*1024)
        {
                TxtMax->Text=tamBuffer*1024;
        }

        //Actualiza la imagen
        dibuja();
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

void __fastcall TformGraficos::Button3Click(TObject *Sender)
{
       int             val_min, val_max;               //Rango a graficar

        val_min=StrToInt(TxtMin->Text);
        val_max=StrToInt(TxtMax->Text);
        //Desplazamiento hacia la izquierda de 128 píxeles
        val_min=val_min-128;
        val_max=val_max-128;

        //Chequeo de condiciones en el borde inferior
        if(val_min<0)
        {
                val_min=0;
                val_max += 128;
        }
        TxtMax->Text=val_max;
        TxtMin->Text=val_min;

        //Actualiza la imagen
        dibuja();        
}
//---------------------------------------------------------------------------

void __fastcall TformGraficos::Button5Click(TObject *Sender)
{
   TxtMax->Text=tamBuffer*1024;
        TxtMin->Text=(tamBuffer*1024)-1024;
        if(StrToInt(TxtMin->Text)<0)
        {
                TxtMin->Text="0";
        }
        //Actualiza la imagen
        dibuja();        
}
//---------------------------------------------------------------------------

