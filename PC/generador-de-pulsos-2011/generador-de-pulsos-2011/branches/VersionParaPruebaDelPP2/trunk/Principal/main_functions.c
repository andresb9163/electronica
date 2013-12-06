#include "main_functions.h"
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

/*
 * Recibe la direccion de la hoja de pulsos en 'pulse_sheet', la parsea(1), y 
 * genera dos archivos nuevos: 'phases_sheet' con todos los datos de cada fase 
 * separados por tab (una fase por linea)(2), 'inst_sheet' lo mismo pero con las 
 * instrucciones(3). 
 * Devuelve 0 si todo salio bien, de lo contrario el codigo de error 
 * correspondiente.
 */
unsigned int main_parse_pulse_program (char* pulse_sheet, char *phases_sheet, char* inst_sheet){
    
    FILE *phases = NULL, *insts = NULL;
    unsigned int inst_count = 0, phase_count = 0, phase_value_count = 0;    
    instruction_sheet is = NULL;
    instruction inst = NULL;
    int i = 0, result = 0, j = 0;
    phase p = NULL;

    assert(pulse_sheet!=NULL && phases_sheet!= NULL && inst_sheet!= NULL);
    /*(1)*/
    is = parse(pulse_sheet);
    if(is == NULL){
        printf("Error:(mostrar mensaje de error devuelto) codigo: %i\n",result);
        return 1;
    }

    /*(2)*/
    insts = fopen(inst_sheet,"w");
    inst_count = instruction_sheet_instruction_count(is);    
    for(i = 0; i < inst_count-1; i++){
        inst = instruction_sheet_get_nth_instruction(is, i);
        fprintf(insts,"%u\t%u\t%i\t%u\t%u\t%i\n", instruction_get_id(inst),
                instruction_get_type(inst), instruction_get_data(inst),
                instruction_get_duration(inst), instruction_get_number(inst),
                instruction_phase_get_shift(inst));           
    }
    /* La ultima no lleva '\n' al final */
    inst = instruction_sheet_get_nth_instruction(is, inst_count-1);
    fprintf(insts,"%u\t%u\t%i\t%u\t%u\t%i", instruction_get_id(inst),
            instruction_get_type(inst), instruction_get_data(inst),
            instruction_get_duration(inst), instruction_get_number(inst),
            instruction_phase_get_shift(inst)); 
    fclose(insts);

    /*(3)*/
    phases = fopen(phases_sheet,"w");
    phase_count = instruction_sheet_phase_count(is);
    for(i = 0; i < phase_count-1; i++){
        p = instruction_sheet_get_nth_phase(is, i);
        fprintf(phases,"%u",phase_id(p));
        phase_value_count = phase_count_values(p);
        for(j = 0; j < phase_value_count; j++){
            fprintf(phases,"\t%u",phase_nth_value(p,j));
        }
		fprintf(phases,"\n");
        //fprintf(phases,"%u\n",phase_get_mem_address(p, 0));
    }
    /* La ultima no lleva '\n' al final */
    p = instruction_sheet_get_nth_phase(is, phase_count-1);
    fprintf(phases,"%u",phase_id(p));
    phase_value_count = phase_count_values(p);
    for(j = 0; j < phase_value_count; j++){
        fprintf(phases,"\t%u",phase_nth_value(p,j));
    }
    //fprintf(phases,"%u",phase_get_mem_address(p, 0));
    fclose(phases);    

    instruction_sheet_destroy(is);

    return result;
}



/* Corre todo el programa */

unsigned int main_run (char *pulse_sheet, char *delay_sheet, unsigned int mps, 
                       unsigned int kpc, unsigned int modo, unsigned int frec1,
                       unsigned int frec2, char *ad_output, int times ){

    instruction_sheet inst_sheet = NULL;
    unsigned int result = 0, i = 0;
    ad_t ad = NULL;
    
    inst_sheet = parse(pulse_sheet);
    if(inst_sheet == NULL){
        printf("Error:(mostrar mensaje de error devuelto) codigo: %i\n",result);
        return 1;
    }

    /****************CONFIGURACION DEL DDS****************//*LISTOOOOOO*/
    dds_config();
    
    /****************CONFIGURACION DEL A/D****************//*LISTOOOOOO*/
    ad = ad_config(mps, kpc, AD_MODO_CONTINUO);/*arreglar lo del MODO*/

    /****************CARGA DE FRECUENCIAS DEL DDS****************//*LISTOOOOOO*/
    dds_set_freq(frec1, frec2); /* frec1 y frec2 deseadas = 5MHz */
    dds_enable();
    
    /*****************CARGAR DELAYS y DEMAS DATOS**********//*LISTOOOOOO*/
    instruction_sheet_config_instructions(inst_sheet, delay_sheet);

    /*****************CARGAR LA RAM FASE*******************/
    dds_load_phases_ram(inst_sheet, 0);

    /*****************CARGAR EL PROGRAMA*******************/
    result = pp2_load_program (inst_sheet);
    if(result == 0){
        result = pp2_microprocessor_mode_enabled();
        if(result == 0){
            result = pp2_launch_pulse_sequence();
        }
    }else{
          printf("Error en la carga del programa\n");      
    }
    
    result = ad_adquirir(ad);
    if(strcmp("",ad_output)){
        ad_to_file(ad, ad_output);
    }

    for(i = 1; i < abs(times); i++){
        if(times < 0){      /*times negativo indica cambio de fase en c/prueba*/
            dds_load_phases_ram(inst_sheet, i);
        }
        pp2_microprocessor_mode_enabled();
        result = pp2_launch_pulse_sequence();
        result = ad_adquirir(ad);
        if(strcmp("",ad_output)){
            ad_to_file(ad, ad_output);
        }
    }

    ad = ad_destroy(ad);
    inst_sheet = instruction_sheet_destroy(inst_sheet);   

    return 0;        
}


void main_dds_enable(void){
    dds_enable();
}


void main_dds_disable(void){
    dds_disable();
}

void main_dds_reset(void){
    dds_reset();
}
