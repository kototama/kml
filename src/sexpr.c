#include <stdio.h>
#include <stdlib.h>

#include "sexpr.h"
#include "symbol.h"
#include "gc.h"

void print_atom(sexpr_t* s)
{
    if(s == NULL) {
        printf("nil");
    } else if(INT(s)) {
        printf("%d", INT_VAL(s));
    } else if(SYM(s)) {
        printf("%s", SYM_VAL(s));
    } else if(FN(s)) {
        printf("<fn: ");
        print_sexpr(CAR(s));
        printf(">");
    } else if(MACRO(s)) {
        printf("<macro: ");
        print_sexpr(CAR(s));
        printf(">");
    }
}

void print_cons_helper(sexpr_t* s, int in_list)
{
    if(!in_list) {
        printf("(");
    }
    
    if(CAR(s) == NULL || ATOM(CAR(s))) {
        print_atom(CAR(s));
    } else {
        print_cons_helper(CAR(s), 0);
    }
    
    if(CDR(s) != NULL) {
        if(ATOM(CDR(s))) {
            printf(" . ");
            print_atom(CDR(s));
        } else {
            printf(" ");
            print_cons_helper(CDR(s), 1);
        }
    }

    if(!in_list) {
        printf(")");
    }
}

void print_cons(sexpr_t* s)
{
    print_cons_helper(s, 0);
}

void print_sexpr(sexpr_t* s)
{
    if(s == NULL) {
        return;
    }
    if(ATOM(s)) {
        print_atom(s);
    } else {
        print_cons(s);
    }
}

sexpr_t* mk_cons(sexpr_t* car, sexpr_t* cdr)
{
    sexpr_t* s = gc_malloc();

    s->flags = CONS_F;
    s->val = ENC_CAR_CDR(car, cdr);
    
    return s;
}

sexpr_t* mk_sym(char* sym)
{
    sexpr_t *s = gc_malloc();

    s->flags = SYM_F;
    s->val = ENC_SYM(sym);
        
    return s;
}

sexpr_t* mk_int(int i)
{
    sexpr_t *s = gc_malloc();

    s->flags = INT_F;
    s->val = i;
        
    return s;
}

sexpr_t* mk_fn(sexpr_t* fn, sexpr_t* env)
{
    sexpr_t *s = gc_malloc();

    s->flags = FN_F;
    s->val = ENC_CAR_CDR(fn, env);
        
    return s;
}

sexpr_t* mk_macro(sexpr_t* macro)
{
    sexpr_t *s = gc_malloc();
    
    s->flags = MACRO_F;
    s->val = ENC_CAR_CDR(macro, NULL);
        
    return s;
}

