#include <stdlib.h>
#include <stdio.h>

#include "interp.h"
#include "primitives.h"
#include "sexpr.h"

// returns the associated value of x or NULL if not value was found
sexpr_t* assoc(sexpr_t* x, sexpr_t* list)
{
    sexpr_t* p = list;

    while(p != NULL) {
        if(SYM_VAL(CAR(CAR(p))) == SYM_VAL(x)) {
            return CDR(CAR(p));
        }

        p = CDR(p);
    }

    return NULL;
}

// pairs((a b), (1 2)) -> ((a 1), (b 2)) 
sexpr_t* pair(sexpr_t* symbols, sexpr_t *values)
{
    sexpr_t* current_symbol = symbols;
    sexpr_t* current_value = values;
    sexpr_t* head = NULL;
    sexpr_t* current_pair = NULL;

    while(current_symbol != NULL && current_value != NULL) {
        if(head == NULL) {
            head = mk_cons(mk_cons(CAR(current_symbol), CAR(current_value)), NULL);
            current_pair = head;
        } else {
            SET_CDR(current_pair, mk_cons(mk_cons(CAR(current_symbol), CAR(current_value)), NULL));
            current_pair = CDR(current_pair);
        }

        current_value = CDR(current_value);
        current_symbol = CDR(current_symbol);
    }

    return head;
}

sexpr_t* append(sexpr_t* list, sexpr_t *rest)
{
    if(list == NULL) {
        return rest;
    }
    sexpr_t* current = list;
    while(CDR(current) != NULL) {
        current = CDR(current);
    }

    SET_CDR(current, rest);
    return list;
}
