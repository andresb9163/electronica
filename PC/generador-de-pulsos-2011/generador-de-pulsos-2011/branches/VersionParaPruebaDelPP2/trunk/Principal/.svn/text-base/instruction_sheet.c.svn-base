#include "instruction_sheet.h"
#include "instruction.h"
#include "instruction_stack.h"
#include "instruction_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct inst_sheet_s {
    GList* instruction_list;
    GList* current_inst;
    GList* phase_list;
    GList* current_phase;
    unsigned int times; /*Numero de veces que se realiza el experimento*/
    char *pulse_sheet_path;
};


instruction_list get_loops_insts(instruction initial_inst, instruction final_inst, instruction_sheet is);
void instruction_sheet_replace(instruction_sheet is, instruction initial, instruction final, instruction_list replacement);
instruction_list repeat_insts(instruction_list list, unsigned int n_times);


instruction_sheet instruction_sheet_create(void){
    instruction_sheet inst_sheet = calloc(1, sizeof(struct inst_sheet_s));
    inst_sheet->instruction_list = NULL;
    inst_sheet->current_inst = NULL;
    inst_sheet->current_phase = NULL;
    inst_sheet->phase_list = NULL;
    inst_sheet->times = 0;

    return inst_sheet;

}

instruction_sheet instruction_sheet_destroy(instruction_sheet inst_sheet){

    unsigned int i = 0;
    assert(inst_sheet != NULL);

    if (inst_sheet != NULL){
    
        for(i = 0; i < g_list_length(inst_sheet->instruction_list); i++){
            instruction_destroy(g_list_nth_data(inst_sheet->instruction_list,i));
        }
    
        g_list_free(inst_sheet->instruction_list);

        for(i = 0; i < g_list_length(inst_sheet->phase_list); i++){
            phase_destroy(g_list_nth_data(inst_sheet->phase_list,i));
        }
        
        g_list_free(inst_sheet->phase_list);

        free(inst_sheet);
        inst_sheet = NULL;
    }

    return inst_sheet;
    
}

void instruction_sheet_add_phase(instruction_sheet inst_sheet, phase ph){
    assert(inst_sheet != NULL);
    
    inst_sheet->phase_list = g_list_append(inst_sheet->phase_list, ph);
}

void instruction_sheet_add_instruction(instruction_sheet inst_sheet, instruction inst){
    assert(inst_sheet != NULL); 

    inst_sheet->instruction_list = g_list_append(inst_sheet->instruction_list, inst);
}

void instruction_sheet_print(instruction_sheet inst_sheet){

    unsigned int i = 0;
    instruction inst = NULL;
	if(inst_sheet != NULL){

		/*i = system("cls");*/
		i = 0;
		printf("Instruction Sheet\n");
		if(inst_sheet->current_inst == NULL){
			printf("C_I -> NULL\n");
		}
        while(i<g_list_length(inst_sheet->instruction_list)){
        	inst = g_list_nth_data(inst_sheet->instruction_list, i);
        	if(inst_sheet->current_inst){
				if(inst == (inst_sheet->current_inst)->data){
					printf("C_I ->");
				}
        	}
            instruction_print(inst);
            i++;
        }
        i = 0;
        while(i<g_list_length(inst_sheet->phase_list)){
            phase_print(g_list_nth_data(inst_sheet->phase_list, i), 1);
            i++;
        }
    }
}

instruction instruction_sheet_get_nth_instruction(instruction_sheet inst_sheet, unsigned int n){
	instruction result = NULL;
	
	assert(inst_sheet != NULL);
	
	result = g_list_nth_data(inst_sheet->instruction_list, n);
	
	return result;
}

unsigned int instruction_sheet_instruction_count(instruction_sheet inst_sheet){
	unsigned int count = 0;
	
	assert(inst_sheet != NULL);
	
	count = g_list_length(inst_sheet->instruction_list);
	
	return count;
}

unsigned int instruction_sheet_phase_count(instruction_sheet inst_sheet){
	unsigned int count = 0;
	
	assert(inst_sheet != NULL);
	
	count = g_list_length(inst_sheet->phase_list);
	
	return count;
}


void instruction_sheet_set_times(instruction_sheet inst_sheet, unsigned int times){

    assert(inst_sheet != NULL);
    inst_sheet->times = times;

}



unsigned int instruction_sheet_get_times(instruction_sheet inst_sheet){

    assert(inst_sheet != NULL);
    return inst_sheet->times;

}

char *instruction_get_pulse_sheet_path(instruction_sheet inst_sheet){

    assert(inst_sheet != NULL);
    return inst_sheet->pulse_sheet_path;
}

void instruction_set_pulse_sheet_path(instruction_sheet inst_sheet, char* path){
	assert(inst_sheet != NULL);
	assert(path != NULL);
	
    inst_sheet->pulse_sheet_path = path;
}

void instruction_sheet_delete_nth_instruction(instruction_sheet inst_sheet, unsigned int n){
	GList *l_aux = NULL;
	instruction inst = NULL;
	
	assert(inst_sheet != NULL);
	assert(n < instruction_sheet_instruction_count(inst_sheet));
	
	if(inst_sheet->current_inst == NULL){
		inst_sheet->current_inst = g_list_first(inst_sheet->instruction_list);
	}else if ((inst_sheet->current_inst)->data == g_list_nth_data(inst_sheet->instruction_list, n)){
		inst_sheet->current_inst = g_list_next(inst_sheet->current_inst);
	}
	
	l_aux = g_list_nth(inst_sheet->instruction_list, n);
	inst_sheet->instruction_list = g_list_remove(inst_sheet->instruction_list, l_aux);
	inst = g_list_nth_data(l_aux, 0);
	inst = instruction_destroy(inst);
	g_list_free(l_aux);
}

unsigned int instruction_sheet_remove_instructions
		(instruction_sheet is,unsigned int from, unsigned int to){
	
	GList *inst_list = NULL;
	unsigned int i = 0;
	unsigned int result = 0;
		
	assert(is != NULL);
	assert(from < to && to < instruction_sheet_instruction_count(is));	
	
	is->current_inst = g_list_nth(is->instruction_list, to+1);
	
	
	for(i = from; i <= to; i++){
			inst_list = g_list_nth(is->instruction_list, from);
			is->instruction_list = g_list_remove_link(is->instruction_list, inst_list);
			inst_list->data = instruction_destroy(inst_list->data);
			g_list_free(inst_list);
	} 
			
	return result;	
}

unsigned int instruction_sheet_insert_instructions
               (instruction_sheet is, GList* instructions, unsigned int from){
       
       unsigned int i = 0, length = 0;
       instruction pointer = NULL;
   
       length = g_list_length(instructions);
       
       for(i = 0; i < length; i++){
           pointer = g_list_nth_data(instructions, i);
           is->instruction_list = g_list_insert(is->instruction_list, pointer, from+i);        
       }
       
       return 0;
}

void instruction_sheet_first_instruction(instruction_sheet is){
	assert(is != NULL);
	is->current_inst = g_list_first(is->instruction_list);
}


void instruction_sheet_next_instruction(instruction_sheet is){
	assert(is != NULL);
	
	if(is->current_inst != NULL)
		is->current_inst = g_list_next(is->current_inst);
}


void instruction_sheet_last_instruction(instruction_sheet is){
	assert(is != NULL);
	
	if(is->current_inst != NULL)
		is->current_inst = g_list_last(is->instruction_list);
}

instruction instruction_sheet_get_current_instruction(instruction_sheet is){
	instruction result = NULL;
	
	assert(is != NULL);
	
	if(is->current_inst != NULL){
		result = (is->current_inst)->data;
	}
	
	return result;
}

void instruction_sheet_prev_instruction(instruction_sheet is){
	assert(is != NULL);

	if(is->current_inst != NULL)
		is->current_inst = g_list_previous(is->current_inst);
}

void instruction_sheet_enum_instructions(instruction_sheet inst_sheet){
	unsigned int i = 0;

	assert(inst_sheet != NULL);

	for(i=0; i<instruction_sheet_instruction_count(inst_sheet);i++){
		instruction_set_number(instruction_sheet_get_nth_instruction(inst_sheet, i), i);
	}
}


void instruction_sheet_couple_instructions(instruction_sheet inst_sheet){
	instruction inst = NULL;
	instruction_stack st = instruction_stack_create();

	assert(inst_sheet != NULL);

	instruction_sheet_first_instruction(inst_sheet);
	inst = instruction_sheet_get_current_instruction(inst_sheet);
	while (inst != NULL){
		if((instruction_get_type(inst) == LOOP_INST_CODE)){
			instruction_stack_push(st, inst);
		}else if(instruction_get_type(inst) == ENDLOOP_INST_CODE){
				instruction_set_data(inst, instruction_get_number(instruction_stack_top(st)));
				instruction_stack_pop(st);
		}
		instruction_sheet_next_instruction(inst_sheet);
		inst = instruction_sheet_get_current_instruction(inst_sheet);
	}
	st = instruction_stack_destroy(st);
}

phase instruction_sheet_get_nth_phase(instruction_sheet inst_sheet, unsigned int n){
	phase result = NULL;
	
	assert(inst_sheet != NULL);
	
	result = g_list_nth_data(inst_sheet->phase_list, n);
	
	return result;
}

void instruction_sheet_set_delay(instruction_sheet inst_sheet, unsigned int type, unsigned int id, unsigned int delay){
    unsigned int i = 0;
    unsigned int count = 0;
    instruction inst = NULL;
    
    assert(inst_sheet != NULL);
    
    count = instruction_sheet_instruction_count(inst_sheet);
    
    for(i=0; i < count; i++){
        inst = instruction_sheet_get_nth_instruction(inst_sheet, i);
        if((instruction_get_type(inst) == type) && (instruction_get_id(inst) == id)){
            instruction_set_duration(inst, delay);
        }
    }
}

phase instruction_sheet_get_phase(instruction_sheet inst_sheet, unsigned int id){

    int i = 0, cota = 0;
    phase result = NULL, p = NULL;
    
    assert(inst_sheet != NULL);
    cota = g_list_length(inst_sheet->phase_list);
    
    
    for(i = 0; i < cota; i++){
        p = g_list_nth_data(inst_sheet->phase_list, i);
        if(phase_id(p) == id) 
            result = p;
    }
    
    return result;
}

void instruction_sheet_convert_loops(instruction_sheet inst_sheet){
	instruction inst = NULL;
	instruction_list loop_inst_list = NULL, stretched_loop_list = NULL;
	instruction_stack st = instruction_stack_create();

	assert(inst_sheet != NULL);

	instruction_sheet_first_instruction(inst_sheet);
	inst = instruction_sheet_get_current_instruction(inst_sheet);
	while (inst != NULL){
		if((instruction_get_type(inst) == LOOP_INST_CODE)){
			instruction_stack_push(st, inst);
			instruction_sheet_next_instruction(inst_sheet);
		}else if(instruction_get_type(inst) == ENDLOOP_INST_CODE){
			if(instruction_is_phase_shifted(instruction_stack_top(st))){
				instruction_sheet_next_instruction(inst_sheet);
				loop_inst_list = get_loops_insts(instruction_stack_top(st), inst, inst_sheet);
				stretched_loop_list = repeat_insts(loop_inst_list, -instruction_get_data(instruction_stack_top(st)));
				instruction_sheet_replace(inst_sheet, instruction_stack_top(st), inst, stretched_loop_list);
				instruction_stack_pop(st);
			}else{
				instruction_sheet_next_instruction(inst_sheet);
				instruction_stack_pop(st);
			}
		}else{
			instruction_sheet_next_instruction(inst_sheet);
		}
		inst = instruction_sheet_get_current_instruction(inst_sheet);
	}
	st = instruction_stack_destroy(st);
}


instruction_list get_loops_insts(instruction initial_inst, instruction final_inst, instruction_sheet is){
	bool found = false;
	unsigned int i = 0;
	instruction_list result = instruction_list_create();

	while((i < instruction_sheet_instruction_count(is)) && !found){
		found = (initial_inst == instruction_sheet_get_nth_instruction(is, i));
		i++;
	}
	if(found){
		while((i < instruction_sheet_instruction_count(is)) &&
			(instruction_sheet_get_nth_instruction(is, i) != final_inst)){
			instruction_list_add(result, instruction_sheet_get_nth_instruction(is, i));
			i++;
		}
	}

	return result;
}

void instruction_sheet_replace(instruction_sheet is, instruction initial, instruction final, instruction_list replacement){
	unsigned int from = 0;
	unsigned int to = 0;

	while((from < instruction_sheet_instruction_count(is)) &&
			(instruction_sheet_get_nth_instruction(is, from) != initial)){
		from++;
	}

	to = from;

	while((to < instruction_sheet_instruction_count(is)) &&
			(instruction_sheet_get_nth_instruction(is, to) != final)){
		to++;
	}

	instruction_sheet_remove_instructions(is, from, to);

	instruction_sheet_insert_instructions(is, instruction_list_get_list(replacement), from);

}

instruction_list repeat_insts(instruction_list list, unsigned int n_times){
	instruction_list result = instruction_list_create();
	unsigned int i = 0, j = 0;
	instruction inst = NULL;

	for(i=0; i < n_times; i++){
		for(j=0; j < instruction_list_length(list); j++){
			inst = instruction_clone(instruction_list_nth_item(list, j));
			instruction_phase_add_shift(inst, i);
			instruction_list_add(result, inst);
		}
	}

	return result;
}
void instruction_sheet_config_instructions(instruction_sheet inst_sheet, char *path){
    /*generate_configuration_sheet(inst_sheet, "config");
    set_delay_values_from_stdin(inst_sheet);*/
     
    set_delay_values_from_file(inst_sheet, path);
    instruction_sheet_convert_loops(inst_sheet);
    instruction_sheet_enum_instructions(inst_sheet);
    instruction_sheet_couple_instructions(inst_sheet); 
}
