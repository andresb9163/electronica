#include "pp2.h"
#include <stdlib.h>
#include <assert.h>
#include "lpt_windows.h"
#include <stdio.h>
/*#include "lpt_windows.h"*/

/*Direcciones de los registros de trabajo*/
#define COMMAND_REG_ADDR 0x50
#define STATUS_REG_ADDR 0x50
#define INSTRUCTION_REG_ADDR 0x51
#define TRANSFER_REG_ADDR 0x52

/*Patrones de configuracion*/
#define FULL_RESET 0x02
#define CHARGE_MODE 0x03
#define MICRO_MODE 0x00
#define LAUNCH_PULSE_SEQ 0x80

/*Numero de bytes de las instrucciones aceptadas por el pp2 */
#define INST_LENGTH 8 




/******************************************************************************/


/****Arma una instruccion de la forma:
	* 
	*|pattern | data | loop_level | inst_code | delay|
	* 
    *Requiere: Haber reservado memoria para todos los parametros
    *Modifica: Nada
    *Retorna: La instruccion 
    * 		   codigo de error: 0 = ok
    * 		  				    1 = Error dispositivo ocupado
    * 						    2 = Codigo de instruccion no valido 
*****/

unsigned int pp2_build_instruction (unsigned char *instruction, 
                                unsigned int pattern, unsigned int data, 
                                int loop_level, unsigned int delay, 
                                unsigned int inst_code);
                                


                                
/******************************************************************************/                                

unsigned int pp2_build_instruction (unsigned char *instruction, 
                                unsigned int pattern, unsigned int data, 
                                int loop_level, unsigned int delay, 
                                unsigned int inst_code){
    unsigned char aux[2] = {0,0};
    unsigned int result = NO_ERROR_CODE, i = 0;
      
    result = !(inst_code == CONTINUE_PP2_CODE || inst_code == LAZO_PP2_CODE ||
              inst_code == RETL_PP2_CODE || inst_code == FIN_PP2_CODE );

    if(!result){        
        aux[1] = inst_code;
        loop_level = loop_level << 3;
        aux[1] = aux[1] | loop_level;
        data = data << 5;                   /*solo sirven los 11 lsb de data*/
        aux[1] = aux[1] | data;
        data = data >> 8;
        aux[0] = data;
        
        instruction[7] = delay;
        delay = delay >> 8;
        instruction[6] = delay;
        delay = delay >> 8;
        instruction[5] = delay;
        delay = delay >> 8;
        instruction[4] = delay;
        instruction[3] = aux[1];
        instruction[2] = aux[0];
        instruction[1] = pattern;
        pattern = pattern >> 8;
        instruction[0] = pattern;
        
    }else{
        result = WRONG_INST_ERROR_CODE;
    }
    
    return result;
}


/******************************************************************************/
/* Construye una instruccion para el pp2 llamando a pp2_build_instruction y 
   luego la graba en la memoria del pp2. */
unsigned int pp2_write_instruction( unsigned int pattern, unsigned int data, 
                                    int loop_level, unsigned int delay, 
                                    unsigned int inst_code){
    
    int i = 0;
    unsigned int result = NO_ERROR_CODE;
    unsigned char instruction[INST_LENGTH] = {0,0,0,0,0,0,0,0}; 
   
    if(!lpt_is_busy(LPT_BASE)){
        result = pp2_build_instruction(instruction, pattern, data, loop_level, delay, inst_code);

       // printf("INSTRUCCION A PUNTO DE ENVIARSE:\n");
      //  for(i = 0; i < INST_LENGTH; i++){
      //        printf("%X\n",instruction[i]);
      //  }
       // printf("\n\n");

        if(result == NO_ERROR_CODE){                       
            result = direccion(INSTRUCTION_REG_ADDR);        
            for (i=INST_LENGTH-1; i>=0; i--){
/*                result = direccion(INSTRUCTION_REG_ADDR);Al vicio meterlo aca en el for????*/
		        if(!result){
                            
                    result =  escritura(instruction[i]);               
                }
                if(result != NO_ERROR_CODE){
                    printf("DEBUG: pp2_write_instruction: Problema al hablar con el pp2\n");      
                    break;
                }
            }
        }
    }                                  
	return result;
}



/******************************************************************************/



unsigned int pp2_full_reset(void){
	unsigned int result = NO_ERROR_CODE;
	
	if(!lpt_is_busy(LPT_BASE)){    
                                                      
		result = direccion(COMMAND_REG_ADDR);
		if(!result){
            result =  escritura(FULL_RESET);
            }
	}else{
		result = BUSY_ERROR_CODE;
	}
	return result;
}



/******************************************************************************/



unsigned int pp2_charge_mode_enabled(void){
	unsigned int result = NO_ERROR_CODE;
	if(!lpt_is_busy(LPT_BASE)){
            result = direccion(COMMAND_REG_ADDR);
		if(!result){
            result =  escritura(CHARGE_MODE);
            }                               
	}else{
		result = BUSY_ERROR_CODE;
	}
	return result;
}


/******************************************************************************/



unsigned int pp2_transfer_instruction(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT_BASE)){
        result = direccion(TRANSFER_REG_ADDR);
		if(!result){
            result =  escritura(0x00);
            }
	}else{
		result = BUSY_ERROR_CODE;
	}
	return result;
}


/******************************************************************************/


bool pp2_is_working(void){

     direccion(STATUS_REG_ADDR);
     return (1 == (0x01 & leer()));
}


/******************************************************************************/


unsigned int pp2_microprocessor_mode_enabled(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT_BASE)){
        result = direccion(COMMAND_REG_ADDR);
		if(!result){
            result =  escritura(MICRO_MODE);
            }
	}else{
		result = BUSY_ERROR_CODE;
	}
	return result;
}


/******************************************************************************/


unsigned int pp2_launch_pulse_sequence(void){
	unsigned int result = NO_ERROR_CODE;
	if(!lpt_is_busy(LPT_BASE)){
	result = direccion(COMMAND_REG_ADDR);
		if(!result){
            result =  escritura(LAUNCH_PULSE_SEQ);
        }
	}else{
		result = BUSY_ERROR_CODE;
	}
	return result;
}


/******************************************************************************/


unsigned int pp2_load_program (instruction_sheet is){
    
    unsigned int ins_count = 0, i = 0;
    instruction inst = NULL;
    phase p = NULL;
    unsigned int p_addr = 0, pattern = 0, data = 0;
    int loop_level = -1; /*Al encontrar el primer loop se incrementa en 1 y 
                           quedamos en el nivel 0*/    
    unsigned int inst_code = 0, delay = 0, result = 0;
    
   // printf("\n\t\tPP2_LOAD_PROGRAM DEBUG:\n\n");

  //  printf("\nReseteando y luego abilitando el PP2\n");
    if(pp2_full_reset()){
		return 1;
	}else if(pp2_charge_mode_enabled()){
		return 1;
    }
    
    ins_count = instruction_sheet_instruction_count(is);
    
    /* Tomo cada instruccion y las traduzco al "lenguaje" del pp2. */
    for(i = 0; i < ins_count; i++){
        inst = instruction_sheet_get_nth_instruction(is, i);
       
        /*Si es pulso tengo que cambiar fase (lo hago con en 3 continue seguidos)*/
        if(instruction_get_type(inst) == PULSE_INST_CODE){

          //  printf("\nDEBUG 'pp2_load_program': Se encontro una instruccion de pulso en el IS y nos disponemos a cargarla en el PP2\n");

            /*  primero direcciono la fase en la ram con los pulsos 11 al 14*/
            /*  En el campo data de cada instruccion de tipo pulso se encuentra 
                el id de su fase.  */          
            p = instruction_sheet_get_phase(is, instruction_get_data(inst));
            /* Debemos tener en cuenta que el pulso puede pertenecer a un bucle 
               con corrimiento de fase. */
            p_addr = phase_get_mem_address(p, instruction_phase_get_shift(inst));

           // printf("\nADDRESS: %X\n",p_addr);
            /* Los bits de direccionamiento de l ram fase son b10,b11,b12,b13 */
            pattern = (p_addr << 10);

            inst_code = CONTINUE_PP2_CODE;   
         
            delay = 2;            

          //  printf("\nDEBUG pp2_load_program instruccion de pulso: primer continue: estabilizo el pattern\n");

            result = pp2_write_instruction(pattern, 0, 0, delay, inst_code);  

          //  printf("\nTRANSFIRIENDO...\n");            

            if(pp2_transfer_instruction() || result) return 1;
            
            /* enciendo el pulso 9 manteniendo la fase ya cargada en pattern
            de este modo cargo la fase en el dds (el delay es el minimo: 2)*/

            pattern = 0x0100 | pattern;
            
           // printf("\nDEBUG pp2_load_program instruccion de pulso: segundo continue: levanto bit 8\n");

            result = pp2_write_instruction(pattern, 0, 0, 2, inst_code);
            
           // printf("\nTRANSFIRIENDO...\n");

            if(pp2_transfer_instruction() || result) return 1;
                                                                                      
            /*  Ahora bajo el pulso 9 y subo el 10 para que se cargue la nueva 
                fase en el registro de trabajo del dds */
            
            pattern = 0x0200 | (p_addr << 10);
            
            /*ESTE DELAY DEBErIA SER MAS LARGO ?? 300ns??*/
            
           // printf("\nDEBUG pp2_load_program instruccion de pulso: tercer continue: bajo el bit 8 y levanto bit 9\n");

            result = pp2_write_instruction(pattern, 0, 0, delay, inst_code);

            //printf("\nTRANSFIRIENDO...\n");
            
            if(pp2_transfer_instruction() || result) return 1;
            
            /* por ultimo bajo el pulso 10 y subo el 16 para enviar el pulso. 
               envio el continue con el delay pedido tomando en cuenta que la 
               proxima instruccion tardara 160ns en cargarse */                               
            
            pattern = 0x8000 | (p_addr << 10);
                 
            delay = instruction_get_duration(inst); 
           
           // printf("\nDEBUG pp2_load_program instruccion de pulso: cuarto continue: bajo el bit 10 y levanto bit 16\n");

            result = pp2_write_instruction(pattern, 0, 0, delay, inst_code);
            
           // printf("\nTRANSFIRIENDO...\n");

            if(pp2_transfer_instruction() || result) return 1;
          
        }else if(instruction_get_type(inst) == LOOP_INST_CODE){

            data = instruction_get_data(inst);
            
            loop_level++;

           // printf("loop_level: %i\n",loop_level);
            
            assert(loop_level <= 3);
            
            inst_code = LAZO_PP2_CODE;
            
            delay = 2; /*el menor delay posible*/
            
            /* El campo pattern no se usa en la instruccion LAZO. */
            result = pp2_write_instruction(0, data, loop_level, delay, inst_code);
                                           
            if(pp2_transfer_instruction() || result) return 1;
                                                                                      
        }else if(instruction_get_type(inst) == ENDLOOP_INST_CODE){

            data = instruction_get_data(inst);

           // printf("\ndata: %X, loop_level: %i\n",data,loop_level-1);               
            
            inst_code = RETL_PP2_CODE;
            
            delay = 2; /*el menor delay posible*/
            /* El campo pattern no se usa en la instruccion RETL. */
            result = pp2_write_instruction(0, data, loop_level, delay, inst_code);

            if(pp2_transfer_instruction() || result) return 1;

            loop_level--;            

        }else if(instruction_get_type(inst) == DELAY_INST_CODE){

            pattern = 0x0000; /*Apago el bit 16 para que no envie el pulso*/

            inst_code = CONTINUE_PP2_CODE;

            delay = instruction_get_duration(inst);    

            result = pp2_write_instruction(pattern, 0, 0, delay, inst_code);
            
            if(pp2_transfer_instruction() || result) return 1;
                                           
        }else if(instruction_get_type(inst) == ACQUIRE_INST_CODE){

            pattern = 0x0010; /* activo el bit 4 para que active el AD */

            inst_code = CONTINUE_PP2_CODE;

            delay = 2;

            result = pp2_write_instruction(pattern, 0, 0, delay, inst_code);

            if(pp2_transfer_instruction() || result) return 1;

       /* }else if(instruction_get_type(inst) == END_INST_CODE){
            inst_code = FIN_PP2_CODE;  
            assert(loop_level == 0);
            result = pp2_write_instruction(0, 0, 0, 2, inst_code);
            if(pp2_transfer_instruction() || result) return 1;     */       

        }else{
            result = WRONG_INST_ERROR_CODE;
        }


        if(result == 1 || result == 2){
        /*1 = Puero ocupado , 2 = Codigo de instruccion no valido*/
			return result;
		}
    }  
           
    /*assert(loop_level == 0 || loop_level == -1);*/
    
    inst_code = FIN_PP2_CODE;

   // printf("\nDEBUG pp2_load_program instruccion de pulso: transifiriendo la instruccion de fin\n");

    result = pp2_write_instruction(0, 0, 0, 2, inst_code);

    //printf("\nTRANSFIRIENDO...\n");

    if(pp2_transfer_instruction() || result) return 1;  
    
    //printf("Termino la carga del programa con result = %u\n",result);

    return result;
}

/******************************************************************************/
