#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bstring/bstrlib.h"
#include "instruction_sheet.h"
#include "parser.h"
#include "phase.h"

#include "lexer.h"

#define PHASES "phases:"
#define PROGRAM "program:"
#define NEWLINE "\n"
#define ASSIGN "="

#define PHASE "ph"
#define PULSE "p"
#define DELAY "d"
#define ACQUIRE "acquire"

#define LOOP "loop"
#define TIMES "times"
#define PHASES_SHIFT "phases_shift"
#define ENDLOOP "endloop"

#define LPARENT "("
#define RPARENT ")"

bstring get_line(Lexer *l);
bool consume_spaces(Lexer *l);
bool line_begin_with (bstring line, const char* pattern);
static bool is_blank_line(Lexer *l);

bool is_program_line(bstring line);
bool is_phase_line(bstring line);

bool phase_header(bstring line);
bool program_header(bstring line);

bool parse_phase(bstring line, instruction_sheet sheet);
bool parse_pulse(bstring line, instruction_sheet sheet);
bool parse_delay(bstring line, instruction_sheet sheet);
bool parse_loop(bstring line, instruction_sheet sheet);
bool parse_endloop(bstring line, instruction_sheet sheet);
bool parse_acquire(bstring line, instruction_sheet sheet);



instruction_sheet parse (char *pulse_program){

    FILE *file = NULL;
    Lexer *l = NULL;
    bstring item = NULL;
    bool parse_ok = true;
    instruction_sheet sheet = NULL;
    unsigned int balanced_loop = 0;

    file = fopen(pulse_program, "r");
    if (file == NULL){
        printf("El archivo no existe\n");
        return NULL;
    }

    l = lexer_new(file);
    assert(l != NULL);

    sheet = instruction_sheet_create();
    assert(sheet != NULL);
    
    
    
    parse_ok = false;

    item = get_line(l);
    if (item == NULL) printf("Archivo vacío\n");
    else parse_ok = phase_header(item);


    if (parse_ok){
        item = get_line(l);
        while(item != NULL && is_phase_line(item) && parse_ok){
            parse_ok = parse_phase(item, sheet);
            bdestroy(item);
            item = get_line(l);
        }
    }else{
        printf("No se encuentra 'phases:'\n");
    }

    if(item != NULL) parse_ok = program_header(item);
    if (parse_ok){
        item = get_line(l);
        while(item != NULL && is_program_line(item) && parse_ok){
            if (line_begin_with(item, PULSE)){
                parse_ok = parse_pulse(item, sheet);
            }
        
            if (line_begin_with(item, DELAY)){
                parse_ok = parse_delay(item, sheet);
            }
        
            if (line_begin_with(item, LOOP)){
                balanced_loop++;
                parse_ok = parse_loop(item, sheet);
            }
                
            if (line_begin_with(item, ENDLOOP)){
                
                parse_ok = parse_endloop(item, sheet);
                if (parse_ok) parse_ok = balanced_loop > 0;
                balanced_loop--;
            }
            
            if (line_begin_with(item, ACQUIRE)){
                parse_ok = parse_acquire(item, sheet);
            }
            
            if (!parse_ok) printf("Error en la linea:%s\n", item->data);
        
            bdestroy(item);
            item = get_line(l);
        }

        if (item != NULL){
            parse_ok = false;
            printf("Error en el bloque 'program'\n");
        }
    }else{
        printf("No se encuentra 'program:'\n");
    }
    

    if (!parse_ok){
        sheet = instruction_sheet_destroy(sheet);
    }
    
    return sheet;
}

bool consume_spaces(Lexer *l){
#define SPACE " "
#define TAB "\t"
#define WHITESPACES SPACE TAB
    assert(l != NULL);
    assert(!lexer_is_off(l));
    
    lexer_skip(l, WHITESPACES);
    
    return !lexer_is_off(l);
}


bstring get_line(Lexer *l){
    bstring item = NULL;
    bool parse_ok = true;
    
    assert(l != NULL);
    assert(!lexer_is_off(l));

    lexer_skip(l, BLANK);
    if (!lexer_is_off(l)) parse_ok = true;
    else parse_ok = false;
    
    if (parse_ok){
        lexer_next_to(l, NEWLINE);
        if (!lexer_is_off(l)){
            item = lexer_item(l);
            bltrimws(item);
            brtrimws(item);
            if (blength(item) == 0){
                bdestroy(item);
                item = NULL;
            }
        }
    }
    
    return item;
}

bool line_begin_with(bstring line, const char* pattern){
    
    bstring dummy = NULL;
    bool result = true;

    assert (line != NULL);
    assert (pattern != NULL);
    
    dummy = bfromcstr(pattern);
    assert(dummy != NULL);
    
    result = binstr(line, 0, dummy) == 0;
    bdestroy(dummy);
    
    return result;

}

bool is_program_line(bstring line){
    
    bool result = true;
    
    /* PRE: */
    assert(line != NULL);
    
    result =  line_begin_with(line, PULSE) ||
              line_begin_with(line, DELAY) ||
              line_begin_with(line, LOOP)  ||
              line_begin_with(line, ENDLOOP) ||
              line_begin_with(line, ACQUIRE);
    
    return result;
}

bool is_phase_line(bstring line){

    bool result = true;
    
    /* PRE: */
    assert(line != NULL);
    
    result =  line_begin_with(line, PHASE);
    
    return result;
}

bool phase_header(bstring line){
    assert (line != NULL);
    
    return biseqcstr(line, PHASES);
}



bool program_header(bstring line){
    assert (line != NULL);
    
    return biseqcstr(line, PROGRAM);
}

bool parse_phase(bstring line, instruction_sheet sheet){
    
    bstring item = NULL;
    
    int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    
    bool parse_ok = true;
    phase ph = NULL;
    
    assert(line != NULL);
    assert(line_begin_with(line, PHASE));
    
    line_len = blength(line);
    
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    /* PRE: */
    lexer_next(l, PHASE);
    item = lexer_item(l);
    assert(biseqcstr(item, PHASE));
    bdestroy(item);
    
    lexer_next(l, DIGIT);
    parse_ok = !lexer_is_off(l);
    
    /* phase phase_create(unsigned int id); */
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        item = lexer_item(l);
        if (blength(item) == 0) parse_ok = false;
        else{
            unsigned int phase_id = 0;
            phase_id = atoi((const char*) item->data);
            ph = phase_create(phase_id);
        }
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        parse_ok = consume_spaces(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        lexer_next(l, ASSIGN);
        parse_ok = !lexer_is_off(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        item = lexer_item(l);
        parse_ok = biseqcstr(item, ASSIGN);
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        parse_ok = consume_spaces(l);
    }

    if (parse_ok){
        do{
            /* -- INVARIANTE: -- */
            /* La cinta està abierta */
            assert(!lexer_is_off(l)); 
            /* El próximo caracter a leer es distinto de BLANK */
            lexer_next(l, BLANK);
            item = lexer_item(l);
            assert(blength(item) == 0);
            bdestroy(item);
            
            lexer_next(l, DIGIT);
            parse_ok = !lexer_is_off(l);
            if (parse_ok){
                assert(!lexer_is_off(l));
                item = lexer_item(l);
                if (blength(item) == 0) parse_ok = false;
                else{
                    unsigned int phase_value = 0;
                    
                    phase_value = atoi((const char*)item->data);
                    phase_add_value(ph, (float) phase_value);
                
                }
                bdestroy(item);
            }

            consume_spaces(l);
        }while(!lexer_is_off(l) && parse_ok);
    
    }
    
    if (parse_ok)
        instruction_sheet_add_phase(sheet, ph);

    return parse_ok;
}

bool parse_pulse(bstring line, instruction_sheet sheet){

    bstring item = NULL;
    int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    bool parse_ok = true;
    unsigned int pulse_id = 0,
                 pulse_param = 0;
    int pulse_type = 2;
    
    instruction instr = NULL;
    
    assert(line != NULL);
    assert(line_begin_with(line, PULSE));
    
    line_len = blength(line);
    
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    /* PRE: */
    lexer_next(l, PULSE);
    item = lexer_item(l);
    assert(biseqcstr(item, PULSE));
    bdestroy(item);
    
    lexer_next(l, DIGIT);
    parse_ok = !lexer_is_off(l);
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        item = lexer_item(l);
        if (blength(item) == 0) parse_ok = false;
        else pulse_id = atoi((const char*) item->data);
            
        
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        lexer_next(l, LPARENT);
        parse_ok = !lexer_is_off(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));        
        item = lexer_item(l);
        parse_ok = biseqcstr(item, LPARENT);
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));    
        lexer_next(l, PHASE);
        parse_ok = !lexer_is_off(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));        
        item = lexer_item(l);
        parse_ok = biseqcstr(item, PHASE);
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        lexer_next(l, DIGIT);
        parse_ok = !lexer_is_off(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        item = lexer_item(l);
        if (blength(item) == 0) parse_ok = false;
        else pulse_param = atoi((const char*) item->data);
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        lexer_next(l, RPARENT);
        parse_ok = !lexer_is_off(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        item = lexer_item(l);
        parse_ok = biseqcstr(item, RPARENT);
        bdestroy(item);
    }
    
    if (parse_ok)
        instr = instruction_create(pulse_id, pulse_type, pulse_param);
        
    if (parse_ok)
        parse_ok = is_blank_line(l);
    
    if (parse_ok)
        instruction_sheet_add_instruction(sheet, instr);
    
    lexer_destroy(l);
    fclose(fstream);
    
    return parse_ok;

}

bool parse_endloop(bstring line, instruction_sheet sheet){
    bstring item = NULL;
    unsigned int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    instruction instr = NULL;
    
    bool parse_ok = true;

    /* PRE: */
    assert(sheet != NULL);
    assert(line != NULL);
    assert(line_begin_with(line, ENDLOOP));

    line_len = blength(line);
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    lexer_next(l, LOWER);
    parse_ok = !lexer_is_off(l);
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        
        item = lexer_item(l);
        parse_ok = biseqcstr(item, ENDLOOP);
        bdestroy(item);
    }
    
    
    if (parse_ok) 
        parse_ok = is_blank_line(l);

    if (parse_ok){
        instr = instruction_create(0, ENDLOOP_INST_CODE, 0);
        instruction_sheet_add_instruction(sheet, instr);
    }
        
    return parse_ok;

}

bool parse_loop(bstring line, instruction_sheet sheet){
    bstring item = NULL;
    unsigned int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    instruction instr = NULL;
    unsigned int instr_param = 0;
    bool shifting_phases = true;
    bool parse_ok = true;

    /* PRE: */
    assert(sheet != NULL);
    assert(line != NULL);
    assert(line_begin_with(line, LOOP));

    line_len = blength(line);
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    lexer_next(l, LOWER);
    parse_ok = !lexer_is_off(l);
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        
        item = lexer_item(l);
        parse_ok = biseqcstr(item, LOOP);
        bdestroy(item);
    }
    
    if (parse_ok)
        parse_ok = consume_spaces(l);
    
    if (parse_ok){
            lexer_next(l, DIGIT);
            parse_ok = !lexer_is_off(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        
        item = lexer_item(l);
        if (blength(item) == 0) parse_ok = false;
        else instr_param = atoi((const char*)item->data);
        bdestroy(item);
    }
    
   if (parse_ok)
        parse_ok = consume_spaces(l);
   
   if (parse_ok){
            lexer_next(l, LOWER);
            parse_ok = !lexer_is_off(l);
    }
   
   if (parse_ok){
        assert(!lexer_is_off(l));
        
        item = lexer_item(l);
        parse_ok = biseqcstr(item, TIMES);
        bdestroy(item);
    }
   
    shifting_phases = false;
    if (parse_ok)
        shifting_phases = consume_spaces(l);
    
    if (shifting_phases){
        lexer_next(l, LOWER "_");
        shifting_phases = !lexer_is_off(l);
    }
    
    if (shifting_phases){
        item = lexer_item(l);
        if (blength(item) == 0) parse_ok = false;
        else shifting_phases = biseqcstr(item, PHASES_SHIFT);
        bdestroy(item);
    }
    
    if (shifting_phases)
        instr_param *= -1;
        
    if (parse_ok) 
        parse_ok = is_blank_line(l);

    
    
    if (parse_ok){
        instr = instruction_create(0, LOOP_INST_CODE, instr_param);
        instruction_sheet_add_instruction(sheet, instr);
    }
    
    return parse_ok;

}


bool parse_delay(bstring line, instruction_sheet sheet){

    bstring item = NULL;
    int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    instruction instr = NULL;
    bool parse_ok = true;
    
    /* PRE: */
    assert(sheet != NULL);
    assert(line != NULL);
    assert(line_begin_with(line, DELAY));
    
    line_len = blength(line);
    
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    /* PRE: */
    lexer_next(l, DELAY);
    item = lexer_item(l);
    assert(biseqcstr(item, DELAY));
    bdestroy(item);
    
    lexer_next(l, DIGIT);
    parse_ok = !lexer_is_off(l);
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        
        item = lexer_item(l);
        if (blength(item) == 0) parse_ok = false;
        else{
            unsigned int delay_id = 0;
                        
            delay_id = atoi((const char*) item->data);
            instr = instruction_create(delay_id, DELAY_INST_CODE, 0);
        }
        bdestroy(item);
    }
    
    if (parse_ok) 
        parse_ok = is_blank_line(l);

    if (parse_ok)
        instruction_sheet_add_instruction(sheet, instr);
    
    lexer_destroy(l);
    fclose(fstream);
       
    return parse_ok;

}

static bool is_blank_line(Lexer *l){
    bstring item = NULL;
    bool result = true;
    
    /* PRE: */
    assert(l != NULL);
    
    if(!lexer_is_off(l)){
        lexer_next_to(l, NEWLINE);
        if (!lexer_is_off(l)){
            item = lexer_item(l);
            bltrimws(item);
            brtrimws(item);
            
            result = blength(item) == 0;            
            
            bdestroy(item);   
        }
    }

    return result;

}


bool parse_acquire(bstring line, instruction_sheet sheet){
    
    bstring item = NULL;
    int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    instruction instr = NULL;
    bool parse_ok = true;
    
    /* PRE: */
    assert(sheet != NULL);
    assert(line != NULL);
    assert(line_begin_with(line, ACQUIRE));
    
    line_len = blength(line);
    
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    /* PRE: */
    lexer_next(l, ACQUIRE);
    item = lexer_item(l);
    assert(biseqcstr(item, ACQUIRE));
    bdestroy(item);
    
    instr = instruction_create(0, ACQUIRE_INST_CODE, 0);
    instruction_sheet_add_instruction(sheet, instr);
    
    /*lexer_next(l, DIGIT);
    parse_ok = !lexer_is_off(l);
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        
        item = lexer_item(l);
        if (blength(item) == 0) parse_ok = false;
        else{
            unsigned int delay_id = 0;
                        
            delay_id = atoi((const char*) item->data);
            instr = instruction_create(delay_id, DELAY_INST_CODE, 0);
        }
        bdestroy(item);
    }
    
    if (parse_ok) 
        parse_ok = is_blank_line(l);

    if (parse_ok)
        instruction_sheet_add_instruction(sheet, instr);
    */
    lexer_destroy(l);
    fclose(fstream);
       
    return parse_ok;

}
