#include <stdlib.h>
#include <stdio.h>

#include "interp.h"
#include "stack.h"
#include "symbol.h"

void stack_init(sexpr_stack_t* stack)
{
    stack->index = -1;
}

void stack_push_sexpr(sexpr_stack_t* stack, sexpr_t* sexpr)
{
    stack->index++;
    stack->sexprs[stack->index] = sexpr;
}

void stack_mark(sexpr_stack_t* stack)
{
    stack->index++;
    stack->sexprs[stack->index] = NULL;
}

int stack_recons_sexpr(sexpr_stack_t* stack)
{
    int prev_mark_idx = -1;
    for(int i = stack->index; i >= 0; i--) {
        if(stack->sexprs[i] == NULL) {
            prev_mark_idx = i;
            break;
        }
    }

    if(prev_mark_idx == -1) {
        return -1;
    }

    sexpr_t* cons = NULL;
    sexpr_t* top_cons = NULL;

    for(int i = prev_mark_idx + 1; i <= stack->index; i++) {
        sexpr_t* sexpr = stack->sexprs[i];
        if(ATOM(sexpr)) {
            if(cons == NULL) {
                if(INT(sexpr)) {
                    cons = mk_cons(mk_int(INT_VAL(sexpr)), NULL);
                    top_cons = cons;
                } else if(SYM(sexpr)) {
                    cons = mk_cons(intern(SYM_VAL(sexpr)), NULL);
                    top_cons = cons;
                }
            } else {
                if(INT(sexpr)) {
                    SET_CDR(cons, mk_cons(mk_int(INT_VAL(sexpr)), NULL));
                } else if(SYM(sexpr)) {
                    SET_CDR(cons, mk_cons(intern(SYM_VAL(sexpr)), NULL));
                }

                cons = CDR(cons);
            }
        } else {
            if(cons == NULL) {
                cons = mk_cons(sexpr, NULL);
                top_cons = cons;
            } else {
                SET_CDR(cons, mk_cons(sexpr, NULL));
                cons = CDR(cons);
            }
        }
    }

    stack->sexprs[prev_mark_idx] = top_cons == NULL ? interp.nil_sym : top_cons;
    stack->index = prev_mark_idx;

    return 0;
}

sexpr_t* stack_top(sexpr_stack_t* stack)
{
    return stack->sexprs[stack->index];
}
