#include "util.h"
#include "instruction_stack.h"
#include <assert.h>
#include <stdio.h>


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
			instruction_phase_add_shift(inst, i+1);
			instruction_list_add(result, inst);
		}
	}

	return result;
}

void convert_loops(instruction_sheet inst_sheet){
	instruction inst = NULL;
	instruction_list loop_inst_list = NULL, stretched_loop_list = NULL;
	instruction_stack st = instruction_stack_create();

	assert(inst_sheet != NULL);

	instruction_sheet_first_instruction(inst_sheet);
	inst = instruction_sheet_get_current_instruction(inst_sheet);
	while (inst != NULL){
		instruction_sheet_print(inst_sheet);
		getchar();
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

