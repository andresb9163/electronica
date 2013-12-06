#ifndef MAIN_FUNCTIONS_H
#define MAIN_FUNCTIONS_H


/*
 * Recibe la direccion de la hoja de pulsos en 'pulse_sheet', la parsea(1), y 
 * genera dos archivos nuevos: 'phases_sheet' con todos los datos de cada fase 
 * separados por tab (una fase por linea)(2), 'inst_sheet' lo mismo pero con las 
 * instrucciones(3). 
 * Devuelve 0 si todo salio bien, de lo contrario el codigo de error 
 * correspondiente.
 */
unsigned int 
    main_parse_pulse_program 
        (char* pulse_sheet, char *phases_sheet, char* inst_sheet);


/*
 * Corre todo el programa 
 */
unsigned int main_run (char *pulse_sheet, char *delay_sheet, unsigned int mps, 
                       unsigned int kpc, unsigned int modo, unsigned int frec1,
                       unsigned int frec2, char *ad_output, int times );


#endif
