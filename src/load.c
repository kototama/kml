#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#include "load.h"
#include "error.h"
#include "read.h"
#include "eval.h"
#include "gc.h"

char* slurp(char* pathname)
{
    struct stat st;
    
    if(stat(pathname, &st) == -1) {
        return NULL;
    }

    FILE* fp = fopen(pathname, "r");
    if(fp == NULL) {
        return NULL;
    }

    char* buffer = malloc(st.st_size);
    fread(buffer, 1, st.st_size, fp);

    return buffer;
}

sexpr_t* load_file(char* pathname, sexpr_t** env, error_t** error)
{
    char *content = slurp(pathname);
    if(content == NULL) {
        *error = mk_error("Invalid file", pathname);
        return NULL;
    }

    sexpr_t* sexp = read(content, error);
    // free(content); :bug some token reused?
    if(*error != NULL) {
        return NULL;
    }

    sexpr_list_t* roots_list = NULL;
    
    while(sexp != NULL) {
        roots_list = cons_to_roots_list(roots_list, sexp);
        roots_list = cons_to_roots_list(roots_list, *env);

        eval(CAR(sexp), env, roots_list, error);
        
        if(*error != NULL) {
            return NULL;
        }
        sexp = CDR(sexp);
    }

    return NULL;
}

