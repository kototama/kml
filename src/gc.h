#ifndef GC_H
#define GC_H

#include "sexpr.h"

typedef struct sexpr_list_s {
    sexpr_t* sexpr;
    struct sexpr_list_s* next;
} sexpr_list_t;

sexpr_t* gc_malloc();
sexpr_list_t* cons_to_roots_list(sexpr_list_t* roots, sexpr_t* sexpr);
void gc_collect(sexpr_list_t* roots);

#endif
