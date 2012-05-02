#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sexpr.h"
#include "symbol.h"
#include "interp.h"

unsigned int hash_symbol(char *sym)
{
    char *p = sym;
    int hash = 0;
    
    while(*p != '\0') {
        hash = (hash * 131) + *p;
        p++;
    }

    return (hash & 0x7FFFFFFF) % SYMBOL_TABLE_SIZE;
}

void init_symbol_table(symbol_table_t* table)
{
    for(int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        table->symbols[i] = NULL;
    }
}

sexpr_t* intern(char* sym)
{
    unsigned int hash = hash_symbol(sym);

    if(interp.sym_table.symbols[hash] == NULL) {
        char* s = malloc(strlen(sym) + 1);
        strcpy(s, sym);

        sexpr_t* sexp = mk_sym(s);
        sexpr_t* cons = mk_cons(sexp, NULL);
        interp.sym_table.symbols[hash] = cons;

        // printf("[sym] %s %p\n", sym, sexp);
        return sexp;
    } else {
        sexpr_t *p = interp.sym_table.symbols[hash];

        while(CDR(p) != NULL) {
            if(strcmp(SYM_VAL(CAR(p)), sym) == 0) {
                // printf("[sym] %s %p\n", sym, CAR(p));
                return CAR(p);
            }
            p = CDR(p);
        }

        if(strcmp(SYM_VAL(CAR(p)), sym) == 0) {
            // printf("[sym] %s %p\n", sym, CAR(p));
            return CAR(p);
        }

        char* s = malloc(strlen(sym) + 1);
        strcpy(s, sym);
        sexpr_t* sexp = mk_sym(s);
        sexpr_t* cons = mk_cons(sexp, NULL);

        SET_CDR(p, cons);

        // printf("[sym] %s %p\n", sym, sexp);
        return sexp;
    }

    return NULL;
}
