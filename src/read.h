#ifndef READ_H
#define READ_H

#include "sexpr.h"
#include "error.h"

typedef enum token_type_e { LPAREN, RPAREN, DOT, SYMBOL, INTEGER, END, ERROR, BEGIN } token_type_t;

typedef struct token_s {
    token_type_t type;
    char* value;
} token_t;

token_t tokenize(char* str, char** next);
sexpr_t* read(char* str, error_t** error);
char* tokenstr(token_t token);

#endif

