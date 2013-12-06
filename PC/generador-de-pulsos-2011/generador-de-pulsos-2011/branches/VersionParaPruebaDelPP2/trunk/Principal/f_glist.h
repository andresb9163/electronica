#ifndef f_glist_H
#define f_glist_H

/* MODULO DE LISTA CIRCULAR DE FLOTANTES */


typedef struct t_f_glist *f_glist;

f_glist f_glist_create (void);

f_glist f_glist_destroy (f_glist fl);

void f_glist_add (f_glist fl, float f); 

float f_glist_next (f_glist fl);

int f_glist_length (f_glist fl);

float f_glist_nth(f_glist fl, unsigned int n);

void f_glist_print (f_glist fl, int times);

int f_glist_find (f_glist fl, float f);

#endif

