#include "grafico.h"
#include "Main.h"
#include "time.h"

#include <stdio.h>

#define ERR_BUFFER 65535


void dibuja() {
    unsigned int i, j;
    // Limpia la imagen
    MainForm->Image1->Canvas->FloodFill(MainForm->ClientWidth / 4,
    MainForm->ClientHeight / 4, clMenu, fsBorder);

    for (i = 0;
        i < (StrToInt(MainForm->TxtMax->Text) - StrToInt(MainForm->TxtMin->Text));
        i++) {
        // Dibuja los puntos del canal A
        j = MainForm->usb.CanalB[StrToInt(MainForm->TxtMin->Text)+i];
        // j=formMain->usb.ADVal_canala(i);
        if (j != ERR_BUFFER) {
            MainForm->Image1->Canvas->Pixels[i+5][512-j/8]=clBlue;
        } else {
            MainForm->Image1->Canvas->Pixels[i][512]=clBlue;
        }
        // Dibuja los puntos del canal B
        // j=canalb[StrToInt(formMain->TxtMin->Text)+i];
        j = MainForm->usb.CanalA[StrToInt(MainForm->TxtMin->Text) + i];
        if (j != ERR_BUFFER) {
            MainForm->Image1->Canvas->Pixels[i][512-j/8]=clRed;
        } else {
            MainForm->Image1->Canvas->Pixels[i][512]=clRed;
        }
    }
}

void escribir_en_archivo(int velocidad) {
    char filename[30] = {0};
    FILE *logger;
    nombreParaLoguer(filename);
    logger = fopen(filename, "a+");
    fprintf(logger, "LA velocidad fue de %i Bytes por segundo \n", velocidad);
    fprintf(logger, "%s", MainForm->usb.Canal1);
    fclose(logger);
}

void nombreParaLoguer(char *filename) {
    char date[28] = {0};
    time_t rawtime;
    time(&rawtime);

    strncat(filename, "Log ", 4);
    strcpy(date, ctime(&rawtime));
    // Log dd Mmm yy hh:mm:ss.txt
    strncat(filename, &date[8] , 3);
    strncat(filename, &date[4] , 4);
    strncat(filename, &date[22] , 2);
    strncat(filename, &date[10] , 9);
    filename[16] = '_';
    filename[19] = '_';

    strncat(filename, ".txt" , 4);
}
