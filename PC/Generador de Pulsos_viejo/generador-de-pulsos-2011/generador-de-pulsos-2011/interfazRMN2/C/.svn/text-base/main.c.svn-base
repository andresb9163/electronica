#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "instruction_sheet.h"
#include "instruction.h"
#include "interface.h"
#include "pp2.h"
#include "dds.h"
#include "parser.h"
#include <math.h>
#include "ad.h"
#include "main_functions.h"
#include <string.h>

int main ( int argc, char *argv[]){

    int result = 0;

    if(!strcmp(argv[1],"mainParsePulseProgram")){
        main_parse_pulse_program (argv[2], argv[3], argv[4]);
    }else{
        return 1;
    } 

    printf("\n>>>  TERMINANDO EL PROGRAMA CON RESULT = %i  <<<\n",result);   
    return 0;        
}
