#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "main_functions.h"
#include <string.h>
#include "ad.h"

int main ( int argc, char *argv[]){

    int result = 0;

    if(argv[1] == NULL){
        printf("Faltan argumentos para correr el programa\n");
        return 7;
    }

    if(!strcmp(argv[1],"mainParsePulseProgram")){

        if(argv[2]==NULL || argv[3]==NULL || argv[4]==NULL){
            printf("Faltan argumetos para mainParsePulseProgram\n");
            return 3;
        }        
        main_parse_pulse_program (argv[2], argv[3], argv[4]);

    }else if(!strcmp(argv[1],"mainRun")){

        if(argv[10]==NULL || argv[2]==NULL || argv[3]==NULL || argv[4]==NULL ||
           argv[5]==NULL || argv[6]==NULL || argv[7]==NULL || argv[8]==NULL ||
           argv[9]==NULL){
            printf("Faltan argumetos para mainRun\n");
            return 5;
        }
        result = main_run (argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]),
                           atoi(argv[7]), atoi(argv[8]), argv[9], atoi(argv[10]));
       /* result = main_run ("sintaxis_ejemplo", "config_conf", 1000000, 1, AD_MODO_CONTINUO, 5000000,
                           5000000, "probando", 10);*/

    }else if(!strcmp(argv[1],"mainDDSEnable")){

         main_dds_enable();

    }else if(!strcmp(argv[1],"mainDDSDisable")){

         main_dds_disable();

    }else if(!strcmp(argv[1],"mainDDSReset")){

         main_dds_reset();       

    }else{

        printf("Operación inexistente\n");
        return 2;

    } 

   // printf("\n>>>  TERMINANDO EL PROGRAMA CON RESULT = %i  <<<\n",result);   
    return result;        
}
