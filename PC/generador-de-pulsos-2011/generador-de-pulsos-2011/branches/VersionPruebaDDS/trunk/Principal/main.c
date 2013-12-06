#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "instruction_sheet.h"
#include "instruction.h"
#include "interface.h"
#include "pp2.h"
#include "dds.h"
#include "parser.h"
#include <math.h>
#include "ad.h"

int main ( int argc, char *argv[]){
    instruction_sheet inst_sheet = NULL;
    int times = 0; 
    unsigned int result = 0, i = 0, kpc = 32;   

    if(argc != 3){
        printf("Error. Uso: ./rmnPulseGenerator \
               <path del programa de pulsos>\t \
               <path de la hoja de delays>\n");
        return 0;
    }
    /****************PARSEAR HOJA DE PULSOS****************//*LISTOOOOOO*/
    printf("ETAPA DE PARSEO DE HOJA DE PULSOS\n\n");
    inst_sheet = parse(argv[1]);
    if(inst_sheet == NULL){
        printf("Error:(mostrar mensaje de error devuelto) codigo: %i\n",result);
        getchar(); 
        return 0;
    }

    /****************CONFIGURACION DEL DDS****************//*LISTOOOOOO*/
    printf("ETAPA DE CONFIGURACION DEL DDS\n\n");
    dds_config(5000000, 5000000); /* frec1 y frec2 deseadas = 5MHz */

    /*****************CARGAR DELAYS y DEMAS DATOS**********//*LISTOOOOOO*/
    printf("ETAPA DE CARGA DE DELAYS Y CONVERSION DE LOOPS\n\n");
    instruction_sheet_config_instructions(inst_sheet, "config_conf");

    /*****************CARGAR LA RAM FASE*******************/
    printf("ETAPA DE CARGA DE FASES\n\n");
    dds_load_phases_ram(inst_sheet, 0);

    /*****************CARGAR EL PROGRAMA*******************/
    printf("ETAPA DE CARGA DE PROGRAMA DE PULSOS\n\n");
    result = pp2_load_program (inst_sheet);
    if(result == 0){
        result = pp2_microprocessor_mode_enabled();
        if(result == 0){
            result = pp2_launch_pulse_sequence();
        }
    }else{
          printf("Error en la carga del programa\n");      
    }
    
    //result = ad_adquirir( kpc, AD_MODO_CONTINUO, 1000000);

    /* 
     *  Corro las repeticiones del experimento corriendo 
     *  la fase si asi se pidio.
     */

    //times = instruction_sheet_get_times(inst_sheet);
       
    //for(i = 1; i < abs(times); i++){
    //    if(times < 0){       /*times negativo indica cambio de fase en c/prueba*/
    //            dds_load_phases_ram(inst_sheet, i);
    //        }
    //    result = pp2_launch_pulse_sequence();
    //    result = ad_adquirir(kpc, AD_MODO_CONTINUO, 1000000);
    //}

    printf("\nTermianndo el programa con result = %i\n",result);   
    return result;        
}
