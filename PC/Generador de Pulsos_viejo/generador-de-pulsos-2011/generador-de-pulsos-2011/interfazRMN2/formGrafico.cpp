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
#define ERR_BUFFER 64*1024


TformGraficos *formGraficos;
double canala[64*1024];
double canalb[64*1024];

void llenarCanalB(char *path){
             ifstream myReadFile;
    myReadFile.open(path);
    char output[100];
    char dato[100];
    int j = 0, i =0, r = 0;

    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {

            myReadFile.getline(output, 100);
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
}

void llenarCanalA(char *path){
             ifstream myReadFile;
    myReadFile.open(path);
    char output[100];
    char dato[100];
    int j = 0, i =7, r = 0;

    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {

            myReadFile.getline(output, 100);
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
                if(j != ERR_BUFFER)
                {
                        formGraficos->Image1->Canvas->Pixels[i][512-j/8]=clBlue;
                }
                else
                {
                        formGraficos->Image1->Canvas->Pixels[i][512]=clBlue;
                }

                //Dibuja los puntos del canal B
                j=canalb[StrToInt(formGraficos->TxtMin->Text)+i];
                if(j != ERR_BUFFER)
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



int getMps(){
     switch(formPrincipal->grupoTamBuffer->ItemIndex){
        case 0:{return 1;
                break;
        }
        case 1:{return  2;
                break;
        }
        case 2:{return  4;
                break;
        }
        case 3:{return  8;
                break;
        }
        case 4:{return  16;
                break;
        }
        case 5:{return  32;
                break;
        }
        case 6:{return  64;
                break;
        }
        case 7:{return  128;
                break;
        }
     }
}

void __fastcall TformGraficos::Button2Click(TObject *Sender)
{
    int             val_min, val_max;               //Rango a graficar

        val_min=StrToInt(formGraficos->TxtMin->Text);
        val_max=StrToInt(formGraficos->TxtMax->Text);
        //Desplazamiento hacia la derecha de 100 píxeles
        val_min=val_min+100;
        val_max=val_max+100;

        //Chequeo de condiciones en el borde superior
        if(val_max>getMps()*1024)
        {
                val_max=getMps()*1024;
        }
        TxtMax->Text=val_max;
        TxtMin->Text=val_min;

        //Actualiza la imagen
        dibuja();
}
//---------------------------------------------------------------------------

