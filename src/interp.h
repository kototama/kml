#ifndef INTERP_H
#define INTERP_H

#include <inttypes.h>

#include "sexpr.h"
#include "symbol.h"
#include "gc.h"

typedef struct interp_s {
    sexpr_t* cons_sym;
    sexpr_t* nil_sym;
    sexpr_t* quote_sym;
    sexpr_t* car_sym;
    sexpr_t* cdr_sym;
    sexpr_t* atom_sym;
    sexpr_t* t_sym;
    sexpr_t* eq_sym;
    sexpr_t* def_sym;
    sexpr_t* fn_sym;
    sexpr_t* if_sym;
    sexpr_t* plus_sym;
    sexpr_t* minus_sym;
    sexpr_t* mul_sym;
    sexpr_t* print_sym;
    sexpr_t* macro_sym;
    sexpr_t* eval_sym;
    symbol_table_t sym_table;
    sexpr_list_t* free_list;
    sexpr_list_t* used_list;
    sexpr_t* env;
    uint32_t nb_sexpr;
    uint32_t nb_sexpr_since_gc;
} interp_t;

void init_interp();
sexpr_t* mk_env();

extern interp_t interp;

#endif
