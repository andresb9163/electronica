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


unsigned int load_program (instruction_sheet is);
bool load_phases_ram(instruction_sheet inst_sheet, unsigned int shift);


unsigned int load_program (instruction_sheet is){
    
    unsigned int ins_count = 0, i = 0;
    instruction inst = NULL;
    phase p = NULL;
    unsigned int p_addr = 0;
    unsigned int pattern = 0;
    unsigned int data = 0;
    int loop_level = -1; /*¿¿¿¿¿Al encontrar el primer loop se incrementa en 1 y 
                           quedamos en el nivel 0???????*/    
    unsigned int inst_code = 0;
    unsigned int delay = 0;
    unsigned int result = 0;
    
    if(pp2_full_reset()){
		return 1;
	}else if(pp2_charge_mode_enabled()){
		return 1;
    }
    
    ins_count = instruction_sheet_instruction_count(is);
    
    /*Tomo cada instruccion y las traduzco al "lenguaje" del pp2*/
    for(i = 0; i < ins_count; i++){
        inst = instruction_sheet_get_nth_instruction(is, i);
        
        /*Si es pulso tengo que cambiar fase (lo hago con en 3 continue seguidos)*/
        if(instruction_get_type(inst) == PULSE_INST_CODE){
        
            /*  primero direcciono la fase en la ram con los pulsos 11 al 14*/          
            p = instruction_sheet_get_phase(is, instruction_get_data(inst));
            p_addr = phase_get_mem_address(p, instruction_phase_get_shift(inst));
            pattern = (p_addr << 2);
            
            data = 0;           /*No se utiliza este campo en los 'continue'*/
            
            /*loop_level = 0;     No se utiliza este campo en los 'continue'*/
            
            inst_code = CONTINUE_PP2_CODE;
            
            delay = 2;
            
            result = pp2_write_instruction(pattern, data, loop_level, 
                                           delay, inst_code);
            
            if(pp2_transfer_instruction()) return 1;
            
            /* enciendo el pulso 9 manteniendo la fase ya cargada en pattern
            de este modo cargo la fase en el dds (el delay es el minimo: 2)*/

            pattern = 0x0080 | pattern;
            
            result = pp2_write_instruction(pattern, data, loop_level, 
                                           delay, inst_code);

            if(pp2_transfer_instruction()) return 1;
                                                                                      
            /*  Ahora bajo el pulso 9 y subo el 10 para que se cargue la nueva 
                fase en el registro de trabajo del dds */
            
            pattern = 0x0040 | (p_addr << 2);
            
            result = pp2_write_instruction(pattern, data, loop_level, 
                                           delay, inst_code);

            if(pp2_transfer_instruction()) return 1;
            
            /* por ultimo bajo el pulso 10 y subo el 16 para enviar el pulso. 
               envio el continue con el delay pedido tomando en cuenta que la 
               proxima instruccion tardara 160ns en cargarse */                               
            
            pattern = 0x0001 | (p_addr << 2);
            
            delay = instruction_get_duration(inst) - 4; /* 4*40ns es el tiempo 
                                                        de carga para la proxima 
                                                        instruccion */
            result = pp2_write_instruction(pattern, data, loop_level, 
                                           delay, inst_code);

          
        }else if(instruction_get_type(inst) == LOOP_INST_CODE){
        /*Si no es pulso entonces es más fácil*/
            pattern = 0; /* NO se usa*/
            
            data = instruction_get_data(inst);
            
            loop_level++;
            
            assert(loop_level <= 3);
            
            inst_code = LAZO_PP2_CODE;
            
            delay = 2; /*el menor delay posible*/
            
            result = pp2_write_instruction(pattern, data, loop_level, 
                                           delay, inst_code);
                                           
        }else if(instruction_get_type(inst) == ENDLOOP_INST_CODE){
        
            pattern = 0;
            
            data = instruction_get_data(inst);
            
            loop_level--;
            
            inst_code = RETL_PP2_CODE;
            
            delay = 2; /*el menor delay posible*/
        
        }else if(instruction_get_type(inst) == DELAY_INST_CODE){
            
            pattern = 0; /*Apago el bit 16 para que no envie el pulso*/
            
            data = 0;
            
            inst_code = CONTINUE_PP2_CODE;
            
            delay = instruction_get_duration(inst) - 4; /* 4*40ns es el tiempo 
                                                        de carga para la proxima 
                                                        instruccion */
        }else if(instruction_get_type(inst) == ACQUIRE_INST_CODE){
        
            /*TODAVIA NO SE QUE HACER CON ESTO*/
            pattern = 16; /* activo el bit 5 para que lance el AD */
            
            inst_code = CONTINUE_PP2_CODE;
            
            delay = 2;
            
                   
        }else if(instruction_get_type(inst) == END_INST_CODE){
        
            inst_code = FIN_PP2_CODE;
            
        }else{
            result = 2;
        }
        
        
        if(result == 1 || result == 2){ 
        /*1 = Puero ocupado , 2 = Codigo de instruccion no valido*/
			return result;
		}else if(pp2_transfer_instruction()){
			return 1;
		}
    }
    
    return result;
}


int main ( int argc, char *argv[]){

    instruction_sheet inst_sheet = NULL;
    int times = 0; 
    unsigned int result = 0, i = 0;   
    short *canala = NULL, *canalb = NULL;

    if(argc != 3){
        printf("Error. Uso: ./rmnPulseGenerator \
               <path del programa de pulsos>\t \
               <path de la hoja de delays>\n");
        return 0;
    }
    
    /****************PARSEAR HOJA DE PULSOS****************//*LISTOOOOOO*/
    inst_sheet = parse(argv[1]);
    if(inst_sheet == NULL){
        printf("Error:(mostrar mensaje de error devuelto) codigo: %i\n",result);
        return 0;
    }
    
    /*****************CARGAR DELAYS y DEMAS DATOS**********//*LISTOOOOOO*/
    /*generate_configuration_sheet(inst_sheet, "config");
    set_delay_values_from_stdin(inst_sheet);*/
    
    set_delay_values_from_file(inst_sheet, "config_conf");
    instruction_sheet_print(inst_sheet);
    /*****************CARGAR LA RAM FASE*******************/

    load_phases_ram(inst_sheet, 0);
    instruction_sheet_print(inst_sheet);
    
    /*****************CARGAR EL PROGRAMA*******************/

    result = load_program (inst_sheet);
    if(result == 0){
        result = pp2_microprocessor_mode_enabled();
        if(result == 0){
            result = pp2_launch_pulse_sequence();
        }
    }
    
    result = ad_adquirir(&canala, &canalb, AD_MODO_MODULADO, 32, 1000000);
    
    /* Corro las repeticiones del experimento corriendo 
       la fase si asi se pidio*/
    times = instruction_sheet_get_times(inst_sheet);
    
    for(i = 1; i < abs(times); i++){
        if(times < 0)       /*times negativo indica cambio de fase en c/prueba*/
            load_phases_ram(inst_sheet, i);
        result = pp2_launch_pulse_sequence();
        result = ad_adquirir(&canala, &canalb, AD_MODO_MODULADO, 32, 1000000);
    }
    
    
    /*¿como sabemos cuando termina de correr el programa*/

    return result;        
}


#define RAM_SPACE_SIZE 16
bool load_phases_ram(instruction_sheet inst_sheet, unsigned int shift){
        unsigned int count_phases = 0,
                     count_phases_value = 0,
                     n = 0,
                     m = 0,
                     phase_value = 0,
                     next_base_address = 0;             
        phase p = NULL;
        bool result = true;
        
        assert(inst_sheet != NULL);
        
        count_phases = instruction_sheet_phase_count(inst_sheet);
        if (count_phases > RAM_SPACE_SIZE) result = false;
        else{
            next_base_address = 0;
            for(n = 0; n < count_phases; n++){
                p = instruction_sheet_get_nth_phase(inst_sheet, n);
                assert(p != NULL);
                
                set_base_address(p, next_base_address);
                printf("Dirección Base: %i\n", next_base_address);
                count_phases_value = phase_count_values(p);
                for(m = 0; m < count_phases_value; m++){
                    phase_value = phase_nth_value(p, m + shift);
                    
                    printf("\tValores: %X\n", phase_value);                    
                                        
                    load_ram_phase(next_base_address, phase_value);                           
                                        
                    next_base_address++;    
                }
            }
        }
    return result;
}













