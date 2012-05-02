#ifndef STACK_H
#define STACK_H

#include "sexpr.h"

#define SEXPR_STACK_SIZE 20000

typedef struct sexpr_stack {
    int index;
    sexpr_t* sexprs[SEXPR_STACK_SIZE];
} sexpr_stack_t;

void stack_init(sexpr_stack_t* stack);
void stack_push_sexpr(sexpr_stack_t* stack, sexpr_t* sexpr);
void stack_mark(sexpr_stack_t* stack);
int stack_recons_sexpr(sexpr_stack_t* stack);
sexpr_t* stack_top(sexpr_stack_t* stack);

#endif
