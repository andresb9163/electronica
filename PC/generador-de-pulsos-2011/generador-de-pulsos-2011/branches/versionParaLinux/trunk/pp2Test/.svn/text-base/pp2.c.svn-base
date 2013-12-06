#include "pp2.h"
#include <stdlib.h>
#include <assert.h>
#include "lpt_linux.h"
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

unsigned int build_instruction (unsigned char *instruction, 
                                unsigned int pattern, unsigned int data, 
                                int loop_level, unsigned int delay, 
                                unsigned int inst_code);

                                
/******************************************************************************/                                

unsigned int build_instruction (unsigned char *instruction, 
                                unsigned int pattern, unsigned int data, 
                                int loop_level, unsigned int delay, 
                                unsigned int inst_code){
    unsigned char aux[2] = {0,0};
    unsigned int result = 0;
      
    result = (inst_code == CONTINUE_INST_CODE || inst_code == LAZO_INST_CODE ||
              inst_code == RETL_INST_CODE || inst_code == FIN_INST_CODE );

    if(result){
        
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
        result = 2;                         /* Codigo de instruccion no valido */
    }
    
    return result;
}


unsigned int pp2_write_instruction( unsigned int pattern, unsigned int data, 
                                    int loop_level, unsigned int delay, 
                                    unsigned int inst_code){
    
    int i = 0;
    unsigned int result = 0;
    unsigned char instruction[INST_LENGTH] = {0,0,0,0,0,0,0,0}; 
  
   
    if(!lpt_is_busy(LPT1_ADDR)){
        result = build_instruction(instruction, pattern, data, loop_level, 
                                   delay, inst_code);
        if(result == 0){                               
            for (i=INST_LENGTH-1; i>=0; i--){
                result = lpt_send_byte(INSTRUCTION_REG_ADDR, instruction[i]);
                if(result != 0) break;
            }
        }
    }
	return result;
}


unsigned int pp2_full_reset(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT1_ADDR)){
		result = lpt_send_byte(COMMAND_REG_ADDR, FULL_RESET);
	}else{
		result = 1; /*Puerto ocupado*/
	}
	return result;
}

unsigned int pp2_charge_mode_enabled(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT1_ADDR)){
		result = lpt_send_byte(COMMAND_REG_ADDR, CHARGE_MODE);
	}else{
		result = 1; /*Puerto ocupado*/
	}
	return result;
}

unsigned int pp2_transfer_instruction(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT1_ADDR)){
		result = lpt_send_byte(TRANSFER_REG_ADDR, 0x00);
	}else{
		result = 1; /*Puerto ocupado*/
	}
	return result;
}


unsigned int pp2_microprocessor_mode_enabled(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT1_ADDR)){
		result = lpt_send_byte(COMMAND_REG_ADDR, MICRO_MODE);
	}else{
		result = 1; /*Puerto ocupado*/
	}
	return result;
}

unsigned int pp2_launch_pulse_sequense(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT1_ADDR)){
		result = lpt_send_byte(COMMAND_REG_ADDR, LAUNCH_PULSE_SEQ);
	}else{
		result = 1; /*Puerto ocupado*/
	}
	return result;
}




