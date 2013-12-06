#include "instruction_list.h"
#include <glib-2.0/glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct t_inst_list{
	GList *list;
	unsigned int count;
};

instruction_list instruction_list_create (void){
	instruction_list il = NULL;
        
    il = calloc(1, sizeof(struct t_inst_list));
    assert(il != NULL);
	il->count = 0;
    return il;
}

instruction_list instruction_list_destroy (instruction_list il){
	unsigned int i = 0;
    
    assert(il != NULL);
    
    for(i = 0; i < g_list_length(il->list); i++){
        instruction_destroy(g_list_nth_data(il->list,i));
    }   
    g_list_free(il->list);
    free(il);
    il = NULL;
    
    return il;
}

void instruction_list_add (instruction_list il, instruction i){
    assert(il != NULL);
    
    il->list = g_list_append(il->list,i);
    il->count++;
}

unsigned int instruction_list_length (instruction_list il){
	assert(il != NULL);
	return il->count;
}

void instruction_list_print (instruction_list il, unsigned int times){
	unsigned int i = 0, j = 0;
	
	assert(il != NULL);
		
	for(i=0; i < times; i++){
		for(j=0; j<il->count; j++){
			instruction_print(g_list_nth_data(il->list, j));
		}
	}
}

instruction instruction_list_nth_item(instruction_list il, unsigned int n){
	assert(il != NULL);
	
	return g_list_nth_data(il->list, n);
}


GList *instruction_list_get_list(instruction_list il){
	
	assert(il != NULL);
	
	return il->list;
}
