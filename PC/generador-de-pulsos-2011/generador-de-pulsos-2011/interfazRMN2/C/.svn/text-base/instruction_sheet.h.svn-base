#ifndef INSTRUCTION_SHEET_H
#define INSTRUCTION_SHEET_H

#include "instruction.h"
#include "phase.h"
#include <glib/glist.h>



typedef struct inst_sheet_s *instruction_sheet;

/****
    *Requiere:Nada
    *Modifica: Nada
    *Retorna: Una nueva instacia de hoja de instrucciones
*****/
instruction_sheet instruction_sheet_create(void);

/****
    *Requiere: Nada
    *Modifica: Elimina por completo una hoja entera de instrucciones
    *Retorna: NULL
*****/

instruction_sheet instruction_sheet_destroy(instruction_sheet inst_sheet);

/****
    *Requiere: La fase ph debe ser no NULL
    *Modifica: Se aumenta la longitud de la lista en uno y se agrega ph al final de la lista de fases
    *Retorna: Una hoja de instrucciones con una fase mas
*****/

void instruction_sheet_add_phase(instruction_sheet inst_sheet, phase ph);

/****
    *Requiere: La instruccion inst debe ser no NULL
    *Modifica: Se aumenta la longitud de la lista en uno y se agrega inst al final de la lista de instrucciones
    *Retorna: Una hoja de instrucciones con una instruccion mas
*****/

void instruction_sheet_add_instruction(instruction_sheet inst_sheet, instruction inst);

/****Imprime por pantalla toda la estructura de la hoja de instrucciones inst_sheet
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: Nada
*****/

void instruction_sheet_print(instruction_sheet inst_sheet);

/****
    *Requiere: inst_sheet != NULL
    *Modifica: Nada
    *Retorna: Un dato de tipo instruction que esta alojado en la posicion 
    * 		  n-esima de inst_sheet comenzando desde las posicion 0.
    * 		  En caso de un error que no viole la precondicion devuelve NULL
*****/
instruction instruction_sheet_get_nth_instruction(instruction_sheet inst_sheet, unsigned int n);

/****
    *Requiere: inst_sheet != NULL
    *Modifica: Nada
    *Retorna: La cantidad de intrucciones almacenadas en isnt_sheet
*****/
unsigned int instruction_sheet_instruction_count(instruction_sheet inst_sheet);

/****
    *Requiere: inst_sheet != NULL
    *Modifica: Nada
    *Retorna: La cantidad de intrucciones almacenadas en isnt_sheet
*****/
unsigned int instruction_sheet_phase_count(instruction_sheet inst_sheet);


/****
    *Requiere: inst_sheet != NULL
    *Modifica: el campo times (cantidad de repeticiones)
    *Retorna: nada
*****/
void instruction_sheet_set_times(instruction_sheet inst_sheet, unsigned int times);


/****
    *Requiere: inst_sheet != NULL
    *Modifica: nada
    *Retorna: el campo times (cantidad de repeticiones)
*****/
unsigned int instruction_sheet_get_times(instruction_sheet inst_sheet);

/****
    *Requiere: inst_sheet != NULL
    *Modifica: nada
    *Retorna: el campo path (ruta de la hoja de pulsos)
*****/
char *instruction_get_pulse_sheet_path(instruction_sheet inst_sheet);

/****
    *Requiere: inst_sheet != NULL y path != NULL
    *Modifica: Setea el valor del campo path de la hoja de pulsos
    *Retorna: Nada
*****/
void instruction_set_pulse_sheet_path(instruction_sheet inst_sheet, char* path);

/****
    *Requiere: 
    *Modifica: 
    *Retorna: Nada
*****/
void instruction_sheet_delete_nth_instruction(instruction_sheet inst_sheet, unsigned int n);


/****
    *Requiere: 
    *Modifica: 
    *Retorna: Nada
*****/
unsigned int instruction_sheet_remove_instructions(instruction_sheet is,unsigned int from, unsigned int to);

/****
    *Requiere: 
    *Modifica: 
    *Retorna: Nada
*****/
unsigned int instruction_sheet_insert_instructions(instruction_sheet is, GList *instructions, unsigned int from);

/****
    *Requiere: 
    *Modifica: 
    *Retorna: Nada
*****/		
void instruction_sheet_first_instruction(instruction_sheet is);

/****
    *Requiere: 
    *Modifica: 
    *Retorna: Nada
*****/
void instruction_sheet_next_instruction(instruction_sheet is);

/****
    *Requiere: 
    *Modifica: 
    *Retorna: Nada
*****/
void instruction_sheet_last_instruction(instruction_sheet is);

/****
    *Requiere: 
    *Modifica: 
    *Retorna: Nada
*****/
instruction instruction_sheet_get_current_instruction(instruction_sheet is);

/****
    *Requiere:
    *Modifica:
    *Retorna: Nada
*****/
void instruction_sheet_prev_instruction(instruction_sheet is);

/****
    *Requiere:
    *Modifica:
    *Retorna: Nada
*****/
void instruction_sheet_enum_instructions(instruction_sheet inst_sheet);

/****
    *Requiere:
    *Modifica:
    *Retorna: Nada
*****/
void instruction_sheet_couple_instructions(instruction_sheet inst_sheet);

/****
    *Requiere:
    *Modifica:
    *Retorna: Nada
*****/
phase instruction_sheet_get_nth_phase(instruction_sheet inst_sheet, unsigned int n);

/****
    *Requiere:
    *Modifica:
    *Retorna: Nada
*****/
void instruction_sheet_set_delay(instruction_sheet inst_sheet, unsigned int type, unsigned int id, unsigned int delay);

/****
    *Requiere:
    *Modifica:
    *Retorna: Nada
*****/
phase instruction_sheet_get_phase(instruction_sheet inst_sheet, unsigned int id);


void instruction_sheet_convert_loops(instruction_sheet inst_sheet);

void instruction_sheet_config_instructions(instruction_sheet inst_sheet, char *path);
#endif


