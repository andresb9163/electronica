#include "instruction.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pp2.h"

struct instruction_s{
    unsigned int id;
    unsigned int type;
    int data;
    unsigned int duration;
    unsigned int number;
    int phase_shift;
};

instruction instruction_create(unsigned int id, int t, int d){

	instruction inst = NULL;

	inst = calloc(1, sizeof(struct instruction_s));
    inst-> id = id;
    inst-> type = t;
    inst-> data = d;
    inst-> duration = 0;
    inst-> number = 0;
	inst-> phase_shift = 0;
    return inst;
}


instruction instruction_destroy(instruction inst){
    
    assert(inst != NULL);
    
    if (inst != NULL){
        free(inst);
        inst = NULL;
    }
    
    return inst;
}


void instruction_set_duration(instruction inst, unsigned int d){
    assert(inst != NULL);
    inst->duration = d;
}


unsigned int instruction_get_duration(instruction inst){
    assert(inst != NULL);
    return inst->duration;
}


void instruction_print(instruction inst){
    printf("Instruction:\n");
    printf("            id: %u\n", inst->id);

    switch(inst->type){
        case LOOP_INST_CODE:{  printf("            type: LOOP\n");
                    break;}
        case ACQUIRE_INST_CODE:{  printf("            type: ACQUIERE\n");
                    break;}
        case PULSE_INST_CODE:{  printf("            type: PULSE\n");
                    break;}
        case DELAY_INST_CODE:{  printf("            type: DELAY\n");
                    break;}
        case ENDLOOP_INST_CODE:{  printf("            type: ENDLOOP\n");
                    break;}
        case END_INST_CODE:{  printf("            type: END\n");
                    break;}
        default:{   printf("Error valor de type no valido\n");
                }
    }
    
    printf("            data: %i\n", inst->data);
    printf("            duration: %u\n", inst->duration);
    printf("            number: %u\n", inst->number);
    printf("            phase_shift: %u\n", inst->phase_shift);
    printf("\n");    
}


int instruction_get_data(instruction inst){
	assert(inst != NULL);
	return inst->data;
}

unsigned int instruction_get_type(instruction inst){
	assert(inst != NULL);
	return inst->type;
}

unsigned int instruction_get_id(instruction inst){
	assert(inst != NULL);
	return inst->id;
}

bool instruction_is_phase_shifted(instruction inst){
	assert(inst != NULL);
	return ((inst->data) < 0);
}

/*Indica que hay que agregar un corrimiento de valor 'shift'
sobre la lista de fases para esta instruccion*/
void instruction_phase_add_shift(instruction inst, int shift){

   assert(inst != NULL);
   inst->phase_shift += shift;
}

unsigned int instruction_get_number(instruction inst){
	assert(inst != NULL);
	return inst->number;
}

int instruction_phase_get_shift(instruction inst){
   assert(inst != NULL);
   return inst->phase_shift;
}

void instruction_set_phase_shift(instruction inst, bool shifted){
	assert(inst != NULL);
	if(shifted){
		if(inst->data >= 0){
			inst->data = (inst->data)*(-1);
		}
	}else if(inst->data < 0){
		inst->data = (inst->data)*(-1);
	}
}

void instruction_set_number(instruction inst, unsigned int n){
	assert(inst != NULL);
	inst->number = n;
}

instruction instruction_clone(instruction inst){
	instruction result = NULL;
	
	assert(inst != NULL);
	
	result = instruction_create(instruction_get_id(inst), 
							  instruction_get_type(inst), 
							  instruction_get_data(inst));
	instruction_set_duration(result, instruction_get_duration(inst));
	instruction_set_number(result, instruction_get_number(inst));
	instruction_phase_add_shift(result, instruction_phase_get_shift(inst));
	
	return result;
}

void instruction_set_data(instruction inst, unsigned int n){
	assert(inst != NULL);

	inst->data = n;

}

unsigned int get_instruction_code(instruction inst){
    unsigned int result = 0;
    
    assert(inst != NULL);
    
    switch(instruction_get_type(inst)){
		case LOOP_INST_CODE:{ result = LAZO_PP2_CODE;
				break;}
		case ACQUIRE_INST_CODE:{result = 0x00;
								/*En este caso no interesa la instruccion ya /
								 * que se debe disparar el conversor AD*/
				break;}
		case PULSE_INST_CODE:{result = CONTINUE_PP2_CODE;
				break;}
		case DELAY_INST_CODE:{result = CONTINUE_PP2_CODE;
				break;}
		case ENDLOOP_INST_CODE:{result = RETL_PP2_CODE;
				break;}
		case END_INST_CODE:{result = FIN_PP2_CODE;
				break;}
	}
    return  result;
}

