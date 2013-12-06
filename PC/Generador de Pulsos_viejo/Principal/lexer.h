#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>   
#include "stdbool.h"

#include "bstrlib.h"


#define UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWER "abcdefghijklmnopqrstuvwxyz"
#define DIGIT "0123456789"
#define BLANK "\t\n\r "
#define ALPHA UPPER LOWER
#define ALPHANUMERIC UPPER LOWER DIGIT

typedef struct _lexer_t Lexer;


Lexer *lexer_new (FILE *f);

void lexer_destroy (Lexer *self);

bstring lexer_item (Lexer *self);

bool lexer_is_started (const Lexer *self);


bool lexer_is_off (const Lexer *self);



void lexer_next (Lexer *self, const char *charset);

void lexer_next_to (Lexer *self, const char *charset);

void lexer_next_char (Lexer *self, const char *charset);

void lexer_skip (Lexer *self, const char *charset);


void lexer_skip_to (Lexer *self, const char *charset);


#endif

