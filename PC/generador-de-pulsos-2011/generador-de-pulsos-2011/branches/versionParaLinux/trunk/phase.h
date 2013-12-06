#ifndef PHASE_H
#define PHASE_H

/* Este tipo representa una fase en el programa. 
 * Las fases pueden tomar diferentes valores 
 * correspondientes a los introducidos con la 
 * función 'phase_add'. (se extraen de manera circular).
 * La implementacion esta basada en el modulo de lista
 * circular de flotantes 'f_glist'.
 */

typedef struct t_phase *phase;


phase phase_create(unsigned int id);

phase phase_destroy(phase p);

void phase_print (phase p, int times);

unsigned int phase_id (phase p);

/* phase_add: agrega un valor de phase 'degree' a la 
 * lista de valores posibles para la fase p
 */
void phase_add_value(phase p, float degrees);


/* phase_next_value: devuelve el siguiente valor de 
 * entre los asignados a la fase p
 */
float phase_next_value(phase p);

void set_base_address(phase p, unsigned int BaseAddress);

unsigned int phase_get_mem_address(phase p, unsigned int shift);

unsigned int phase_count_values(phase p);

/*REVISAR!!!!!!!!!!!!!!!*/
unsigned int phase_nth_value(phase p, unsigned int n);

#endif
