#include <stdio.h>
#include <glib/glist.h>
#include "f_glist.h"
#include <assert.h>
#include <stdlib.h>

typedef struct t_cell *cell;
typedef GList *glist;

struct t_cell{
    unsigned int value;
};


struct t_f_glist{
    glist list;
};


f_glist f_glist_create (void){
    f_glist fl = NULL;
    fl = calloc(1,sizeof(struct t_f_glist));
    assert(fl != NULL);
   
    return fl;
}


f_glist f_glist_destroy (f_glist fl){
    
    unsigned int i = 0;
    
    assert(fl != NULL);
    
    for(i = 0; i< g_list_length(fl->list); i++){
        free(g_list_nth_data(fl->list,i));      
    }   
    g_list_free(fl->list);
    free(fl);
    fl = NULL;
    
    return fl;
}


void f_glist_add (f_glist fl, float f){
 
    cell c = NULL;
    
    c = calloc(1, sizeof(struct t_cell));
    c->value = f;
    fl->list = g_list_append(fl->list,c);
}
 
        
   

float f_glist_next (f_glist fl){
    
    float result = 0;
    glist aux = NULL;
    
    assert(fl != NULL);
    assert(fl->list != NULL);
    
    result = ((cell)(g_list_nth_data (fl->list, 0)))->value;        
    
    aux = g_list_next(fl->list);
    
    if(aux == NULL){
        aux = g_list_first(fl->list);
    }
       
    fl->list = aux;
    
    return result;
}


int f_glist_length (f_glist fl){

    assert(fl != NULL);
    return g_list_length(fl->list);

}


float f_glist_nth(f_glist fl, unsigned int n){

	assert(fl != NULL);
	assert(f_glist_length(fl) > (int)n);
	return ((cell)g_list_nth_data(fl->list,n))->value;

}

void f_glist_print (f_glist fl, int times){

    int i = 0, j = 0, len = 0;
    
    len = f_glist_length(fl);
     
    for(i = 0; i < times; i++){
        printf("[ ");
        for( j = 0; j < len; j++){
            printf("%f ", f_glist_next(fl));
        }
        printf("]\n");
    }
}

/* Devuelve la posicion de la primera coincidencia con f o -1 si no se 
/* encuentra el valor. */
int f_glist_find (f_glist fl, float f){

    int i = 0, cota = 0;
    cota = g_list_length(fl->list);
    while(i < cota){
        if ((cell)g_list_nth(fl,i))->value == f)
            return i;
        i++;
    }
    return -1;
}


