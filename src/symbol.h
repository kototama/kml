#ifndef SYMBOL_H
#define SYMBOL_H

#include "sexpr.h"

// 100 entries in the hashtable. We are not limited to 100 symbols since
// symbols with the same hash are grouped in bucket
#define SYMBOL_TABLE_SIZE 100

typedef struct symbol_table_s {
    sexpr_t* symbols[SYMBOL_TABLE_SIZE];
} symbol_table_t;

unsigned int hash_symbol(char* sym);
void init_symbol_table(symbol_table_t* table);
sexpr_t* intern(char* sym);

#endif
