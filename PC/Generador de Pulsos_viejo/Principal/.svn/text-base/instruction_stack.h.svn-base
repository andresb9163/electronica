#ifndef _INSTRUCTION_STACK_H_
#define _INSTRUCTION_STACK_H_

#include "stdbool.h"
#include "instruction.h"

typedef struct instruction_stack_s* instruction_stack;

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
instruction_stack instruction_stack_create(void);

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
void instruction_stack_push(instruction_stack st, instruction inst);

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
instruction instruction_stack_top(instruction_stack st);

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
void instruction_stack_pop(instruction_stack st);

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
instruction_stack instruction_stack_destroy(instruction_stack st);

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
bool instruction_stack_is_empty(instruction_stack st);

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
unsigned int instruction_stack_count(instruction_stack st);

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
void instruction_stack_print(instruction_stack st);

#endif

