#ifndef LOAD_H
#define LOAD_H

#include "sexpr.h"
#include "error.h"

sexpr_t* load_file(char* pathname, sexpr_t** env, error_t** error);
char* slurp(char* pathname);
    
#endif
