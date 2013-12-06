/* Este tipo representa una fase en el programa. 
 * Las fases pueden tomar diferentes valores 
 * correspondientes a los introducidos con la 
 * función 'phase_add'. (se extraen de manera circular).
 */

#include "phase.h"
#include <stdio.h>
#include <stdlib.h>
#include "f_glist.h"
#include <assert.h>

#define DEBUG_ON
#define RAM_RANGE 16 /* Espacio de ram: 0   phase_value = phase_nth_value(p, m); .. RAM_RANGE - 1 */	
	


struct t_phase {
    unsigned int id;
    f_glist values;
	unsigned int baseAddr;
};


phase phase_create(unsigned int id){
 
    phase result = NULL;
    
    result = calloc(1,sizeof(struct t_phase));
    if (result != NULL){
        result->id = id;
        result->values = f_glist_create();
    }
#ifdef DEBUG__ON    
    else{
        printf("Error, no se pudo reservar memoria para la phase %i.\n", id);
    }
#endif

    return result;
}



phase phase_destroy(phase p){

    assert(p != NULL);
    p->values = f_glist_destroy(p->values);
    free(p);
    p = NULL;
    
    return p;
}



unsigned int phase_id (phase p){
    assert(p!=NULL);
    return p->id;
}



/* phase_add: agrega un valor de phase 'degree' a la 
 * lista de valores posibles para la fase p
 */
void phase_add_value(phase p, float degrees){
    
    assert(p != NULL);
    f_glist_add (p->values, degrees);

}



/* phase_next_value: devuelve el siguiente valor de 
 * entre los asignados a la fase p
 */
float phase_next_value(phase p){

    assert(p!=NULL);
    return f_glist_next(p->values); 
}

void phase_print (phase p, int times){
    
    assert(p!=NULL);
    printf("Phase%u:\n", p->id);
    f_glist_print(p->values, times);

}


	
void set_base_address(phase p, unsigned int baseAddress){

    /* PRE: */
    assert(p != NULL);
	assert(baseAddress < RAM_RANGE);
	
	p->baseAddr = baseAddress;

}

unsigned int phase_get_mem_address(phase p, unsigned int shift){
	unsigned int result = 0,
	             index = 0,
				 len = 0;
	
	/* PRE: */
	assert(p != NULL);
	/* Desde 0 <= shift < N° of Samples */
	
	len = f_glist_length(p->values);
	assert(len != 0);
	
	index = shift % len;
	result = p->baseAddr + index;
	
	return result;
	
}

/*

Por ejemplo:
Ph5 = 3 8 6 4
Supongamos que la dirección base de la fase PH5 es 12:
por lo tanto, tenemos en la ram:
12: 3
13: 8
14: 6
15: 4

Contando la iteración i-ésima partiendo desde 0, en la 6°
iteración PH5 debería tomar el valor 6,
Iteración: 0 1 2 3 4 5 6 7
Valor:     3 8 6 4 3 8 6

6 % 4 = 2 -> index = 2. Por lo tanto toma el valor 2

Si quieren que la iteración comience desde 1, ponemos la siguiente linea:
iteración--;

*/

unsigned int phase_count_values(phase p){
	assert(p != NULL);
	return f_glist_length(p->values);
}

/*REVISAR!!!!!!!!!!!!!!!*/
unsigned int phase_nth_value(phase p, unsigned int n){
	assert(p != NULL);
	n = n % phase_count_values(p);
	printf("%u \n%X\n",n,f_glist_nth(p->values, n));
	return f_glist_nth(p->values, n);
}
