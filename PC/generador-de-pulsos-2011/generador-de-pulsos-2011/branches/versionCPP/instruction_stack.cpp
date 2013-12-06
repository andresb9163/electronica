#include "instruction_stack.h"
//#include <glib/glist.h>
#include "glistCPP.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct instruction_stack_s{
	GList *data;
	unsigned int count;
};

instruction_stack instruction_stack_create(void){
	instruction_stack result = NULL;
	result = (instruction_stack)calloc(1, sizeof(struct instruction_stack_s));
	result->data = NULL;
	result->count = 0;
	
	return result;
}

void instruction_stack_push(instruction_stack st, instruction inst){
	assert(st != NULL);
	st->data = g_list_prepend(st->data, inst);
	st->count++;
}

instruction instruction_stack_top(instruction_stack st){
	assert(st != NULL);
	return (instruction)g_list_nth_data(st->data, 0);
}

void instruction_stack_pop(instruction_stack st){
	instruction inst = NULL;
	
	assert(st != NULL);
	
	inst = (instruction)g_list_nth_data(st->data, 0);
	st->data = g_list_remove(st->data, inst);
	/*inst = instruction_destroy(inst);*/
	st->count--;
}

instruction_stack instruction_stack_destroy(instruction_stack st){
	unsigned int i = 0;
	
	assert(st != NULL);
	
	for(i=0;i<st->count;i++){
		instruction_destroy((instruction)g_list_nth_data(st->data, i));
	}
	g_list_free(st->data);
	free(st);
	st = NULL;
	return st;
}

bool instruction_stack_is_empty(instruction_stack st){
	assert(st != NULL);
	return (st->count == 0);
}

unsigned int instruction_stack_count(instruction_stack st){
	unsigned int result = 0;
	if(st != NULL){
		result = st->count;
	}
	return result;
}

void instruction_stack_print(instruction_stack st){
	unsigned int i = 0;
	
	assert(st != NULL);
	
	for(i=0;i<st->count;i++){
		instruction_print((instruction)g_list_nth_data(st->data, i));
	}
}

