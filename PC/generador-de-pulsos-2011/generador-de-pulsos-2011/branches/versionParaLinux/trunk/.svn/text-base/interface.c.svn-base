#include "interface.h"

#include <stdlib.h>
#include <assert.h>
#include "lexer.h"



#define INITIAL_LEN 30
#define INCREMENT   20

#define SUFFIX "_conf"

struct array_t{
	int* items;
	unsigned int len;
	unsigned int max_len;
 };

typedef struct array_t *iarray;
 

 /* Funciones Auxiliares */
static iarray iarray_create(void);
static void iarray_add(iarray array, int value);
static bool iarray_checkvalue(iarray, int value);
static iarray iarray_destroy(iarray array);
static unsigned int iarray_len(iarray array);
static int iarray_nth_item(iarray array, unsigned int i);

static void fill_iarray(instruction_sheet sheet, iarray delay_list, iarray pulse_list);
static bool get_id_and_delay(Lexer *l, unsigned int* type,unsigned int* id, unsigned int* delay);
 

 /*
  Genera un archivo con nombre: sheet->filename ++ "_config",
  en el mismo path donde se encuentra 'sheet', con el siguiente
  formato:
  
  sheet: archivoprueba
  ----------------------------
  phases:
	ph1 = 1 2 3 4
	ph4 = 7 8
  program:
	d1
	p1(ph4)
	d2
	d1
	p1(ph5)
	p5(ph1)
  ----------------------------
  
  Generar�a:
  configuration_sheet: archivoprueba_conf
  -----------------------------
  d1:
  p1:
  d2:
  p5:
  -----------------------------
  
 */

static void fill_iarray(instruction_sheet sheet, iarray delay_list, iarray pulse_list){
    unsigned int len = 0,
	             i = 0;
    instruction aux = NULL;
    unsigned int instruction_id = 0;
    
    /* PRE: */
    assert(sheet != NULL);
    assert(delay_list != NULL);
    assert(pulse_list != NULL);
    
    len = instruction_sheet_instruction_count(sheet);
    assert(len != 0);

    for(i = 0; i < len; i++){
		
		aux = instruction_sheet_get_nth_instruction(sheet, i);
		assert(aux != NULL);
        
        instruction_id = instruction_get_id(aux);
        
		if (instruction_get_type(aux) == PULSE_INST_CODE)
			if (!iarray_checkvalue(pulse_list, instruction_id))
                iarray_add(pulse_list, instruction_id);
			
		if (instruction_get_type(aux) == DELAY_INST_CODE)
			if (!iarray_checkvalue(delay_list, instruction_id))
				iarray_add(delay_list, instruction_id);
	
    
    }

}
 
 
void generate_configuration_sheet(instruction_sheet sheet, const char* filepath){

	unsigned int len = 0,
	             i = 0,
				 line_len = 0;
	FILE *f = NULL;
	bstring filenameconf = NULL,
	        line = NULL;
	iarray delay_list = NULL,
	       pulse_list = NULL;
    bstring instr_id_str = NULL;
           
    unsigned int instruction_id = 0;
	
	
	assert(sheet != NULL);
	
	len = instruction_sheet_instruction_count(sheet);
    assert(len != 0);
	
	filenameconf = bfromcstr(filepath);
	assert(filenameconf != NULL);
	
	bcatcstr(filenameconf, SUFFIX);
	
	f = fopen((const char*)filenameconf->data, "w");
	delay_list = iarray_create();
	pulse_list = iarray_create();
	
	    
    fill_iarray(sheet, delay_list, pulse_list);
    
    for(i = 0; i < iarray_len(pulse_list); i++){
        
        instruction_id = iarray_nth_item(pulse_list, i);
        
        
        instr_id_str = bformat("%i", instruction_id);
        assert(instr_id_str);
        
        line = bfromcstr("");
        
        bcatcstr(line, "p");
        bconcat(line, instr_id_str);
        bcatcstr(line, ":\n");
        
        line_len = blength(line);
        fwrite((void*)line->data, 1, line_len, f);
        
        bdestroy(line);
        bdestroy(instr_id_str);
    }
    
    for(i = 0; i < iarray_len(delay_list); i++){
        
        instruction_id = iarray_nth_item(delay_list, i);
        instr_id_str = bformat("%i", instruction_id);
        assert(instr_id_str);
        
        line = bfromcstr("");
        
        bcatcstr(line, "d");
        bconcat(line, instr_id_str);
        bcatcstr(line, ":\n");
        
        line_len = blength(line);
        fwrite((void*)line->data, 1, line_len, f);
        
        bdestroy(line);
        bdestroy(instr_id_str);
    }
    

        
	
	delay_list = iarray_destroy(delay_list);
	pulse_list = iarray_destroy(pulse_list);
	fclose(f);
	
 }
 
 bool set_delay_values_from_stdin(instruction_sheet sheet){
    iarray delay_list = NULL,
           pulse_list = NULL;
    unsigned int i = 0,
                 instruction_id = 0,
                 instruction_delay = 0,
                 j = 0;
    bool result = 0;
    
    /* PRE: */
    assert(sheet != NULL);
    
    delay_list = iarray_create();
	pulse_list = iarray_create();
	
	    
    fill_iarray(sheet, delay_list, pulse_list);
    for(i = 0; i < iarray_len(pulse_list); i++){
        instruction_id = iarray_nth_item(pulse_list, i);
        printf("p%i:", instruction_id);
        j = scanf("%u", &instruction_delay);
        instruction_sheet_set_delay(sheet, PULSE_INST_CODE, instruction_id, instruction_delay);
    }
 
    for(i = 0; i < iarray_len(delay_list); i++){
        instruction_id = iarray_nth_item(delay_list, i);
        printf("d%i:", instruction_id);
        j = scanf("%u", &instruction_delay);
        instruction_sheet_set_delay(sheet, DELAY_INST_CODE, instruction_id, instruction_delay);
    }
 
    return result;
 
 }
 
 bool set_delay_values_from_file(instruction_sheet sheet, const char* filename){
	FILE *f = NULL;
	Lexer *l = NULL;
	bool result = true;
	unsigned int instruction_id = 0,
                 delay = 0;
	
	
	f = fopen((const char*)filename, "r");
	if (f == NULL) result = false;
	
	if(!result)printf("No se ha encontrado el archivo\n");
	
    if (result){
        unsigned int instruction_type = 0;
    
        l = lexer_new(f);
        assert(l != NULL);
    
        while (!lexer_is_off(l) && result){
            result = get_id_and_delay(l, &instruction_type, &instruction_id, &delay);
            if (result) printf("TYPE: %i ID:%i DELAY%i\n", instruction_type, instruction_id, delay);
            instruction_sheet_set_delay(sheet, instruction_type, instruction_id, delay);        
        }
    }
	
    
    
	return result;
	
	
	
	
 
 }
 
static bool get_id_and_delay(Lexer *l, unsigned int* type,unsigned int* id, unsigned int* delay){
	bool result = true;
	
	bstring item = NULL;
	
	assert(l != NULL);
	assert(!lexer_is_off(l));
	
	lexer_next(l, LOWER);
	result = !lexer_is_off(l);
	
	if (result){
		item = lexer_item(l);
		if (biseqcstr(item, "p")) *type = PULSE_INST_CODE;
		else if (biseqcstr(item, "d")) *type = DELAY_INST_CODE;
		else result = false;
		bdestroy(item);
	}
	
	if (result){
		lexer_next(l, DIGIT);
		result = !lexer_is_off(l);
	}
	
	if (result){
		item = lexer_item(l);
		if (blength(item) == 0) result = false;
		else *id = atoi((const char*)item->data);
		bdestroy(item);
	}
	
	if (result){
		lexer_next(l, ":");
		result = !lexer_is_off(l);
	}
	
	if (result){
		item = lexer_item(l);
		result = biseqcstr(item, ":");
		bdestroy(item);
	}
	
	if (result){
		lexer_next(l, DIGIT);
		result = !lexer_is_off(l);
	}
	
	if (result){
		item = lexer_item(l);
		if (blength(item) == 0) result = false;
		else *delay = atoi((const char*)item->data);
		bdestroy(item);
	}
    
    if (result){
        lexer_next_to(l, LOWER);
        result = !lexer_is_off(l);
    }
	
	

	return result;

}















 
 
 
static iarray iarray_create(void){
	iarray result = NULL;
	
	result = (iarray)calloc(1, sizeof(struct array_t));
	assert(result != NULL);
	
	result->items = (int*) calloc(INITIAL_LEN, sizeof(int));
	assert(result->items != NULL);
	
	result->len = 0;
	result->max_len = INITIAL_LEN;
	
	return result;
 
 }
 
 static void iarray_add(iarray array, int value){
 
	assert(array != NULL);
	
	if (array->len == array->max_len){
		array->max_len = array->max_len + INCREMENT;
		array->items = (int*) realloc(array->items, array->max_len * sizeof(int));
	}
	
	array->items[array->len] = value;
	array->len++;
	
}
 static bool iarray_checkvalue(iarray array, int value){

	bool result = true;
	unsigned int i = 0;
	
	result = false;
	for (i = 0; i < array->len && !result; i++)
		if (array->items[i] == value) result = true;
	
	return result;

}

static unsigned int iarray_len(iarray array){
    
    assert(array != NULL);
    
    return array->len;

}

static int iarray_nth_item(iarray array, unsigned int i){

    assert(array != NULL);
    assert(i < array->len);
    
    return array->items[i];

}

 static iarray iarray_destroy(iarray array){
	
	assert(array != NULL);
	
	free(array->items);
	free(array);
	
	return NULL;

 }
