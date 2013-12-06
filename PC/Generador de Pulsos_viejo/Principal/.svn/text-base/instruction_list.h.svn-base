#ifndef _INSTRUCTION_LIST_
#define _INSTRUCTION_LIST_

#include "instruction.h"
#include <glib/glist.h>

typedef struct t_inst_list *instruction_list;

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
instruction_list instruction_list_create (void);

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
instruction_list instruction_list_destroy (instruction_list il);

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
void instruction_list_add (instruction_list il, instruction i); 

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
unsigned int instruction_list_length (instruction_list il);

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
void instruction_list_print (instruction_list il, unsigned int times);

/****
	*Requiere: 
	*Modifica: 
	*Retorna: 
*****/
instruction instruction_list_nth_item(instruction_list il, unsigned int n);

GList *instruction_list_get_list(instruction_list il);



#endif

