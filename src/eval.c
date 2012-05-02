#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "sexpr.h"
#include "symbol.h"
#include "interp.h"
#include "primitives.h"
#include "error.h"
#include "gc.h"


sexpr_t* eval_list(sexpr_t* list, sexpr_t** env, sexpr_list_t* roots, error_t** error)
{
    sexpr_t* head = NULL;
    sexpr_t *current_e = NULL;
    sexpr_t* result;
    sexpr_t* current = list;

    while(current != NULL) {
        result = eval(CAR(current), env, roots, error);

        if(*error != NULL) {
            return NULL;
        }
        
        if(head == NULL) {
            current_e = mk_cons(result, NULL);
            head = current_e;
        } else {
            SET_CDR(current_e, mk_cons(result, NULL));
            current_e = CDR(current_e);
        }

        roots = cons_to_roots_list(roots, current_e);
        current = CDR(current);
    }

    return head;
}

sexpr_t* eval(sexpr_t* sexpr, sexpr_t** env, sexpr_list_t* roots, error_t** error)
{
    if(sexpr == NULL) {
        return interp.nil_sym;
    }

    /* printf("[eval]\n"); */
    /* print_sexpr(sexpr); */
    /* printf("\n"); */
    roots = cons_to_roots_list(roots, sexpr);
    gc_collect(roots);
    
    if(ATOM(sexpr)) {
        if(SYM(sexpr)) {
            if(interp.t_sym == sexpr) {
                return interp.t_sym;
            }
            if(interp.nil_sym == sexpr) {
                return interp.nil_sym;
            }
            sexpr_t* val = assoc(sexpr, *env);
            if(val == NULL) {
                *error = mk_error("Undefined symbol", SYM_VAL(sexpr));
            }

            return val;
        }
        if(INT(sexpr)) {
            return sexpr;
        }
    } else if(ATOM(CAR(sexpr))) {
        if(SYM(CAR(sexpr))) {
            // quote
            if(interp.quote_sym == CAR(sexpr)) {
                if(CDR(sexpr) == NULL) {
                    *error = mk_error("Missing quote argument", "");
                    return NULL;
                }
                if(CDR(CDR(sexpr)) != NULL) {
                    *error = mk_error("Too many arguments for quote", "");
                    return NULL;
                }
                return CAR(CDR(sexpr));
            }
            // atom
            if(interp.atom_sym == CAR(sexpr)) {
                if(ATOM(eval(CAR(CDR(sexpr)), env, roots, error))) {
                    return interp.t_sym;
                }
                return interp.nil_sym;
            }
            // eq
            if(interp.eq_sym == CAR(sexpr)) {
                // TODO check nb args
                sexpr_t* e1 = eval(CAR(CDR(sexpr)), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }

                roots = cons_to_roots_list(roots, e1);
                sexpr_t* e2 = eval(CAR(CDR(CDR(sexpr))), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }
                if(INT(e1) && INT(e2)) {
                    if(INT_VAL(e1) == INT_VAL(e2)) {
                        return interp.t_sym;
                    }
                    return interp.nil_sym;
                }
                if(e1 == e2) {
                    return interp.t_sym;
                }
                return interp.nil_sym;
            }
            // if
            if(interp.if_sym == CAR(sexpr)) {
                sexpr_t* e1 = eval(CAR(CDR(sexpr)), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }
                if(e1 == NULL) {
                    printf("ERRROROROR\n");
                }
                if(e1 == interp.nil_sym) {
                    return eval(CAR(CDR(CDR(CDR(sexpr)))), env, roots, error);
                } else {
                    return eval(CAR(CDR(CDR(sexpr))), env, roots, error);
                }
            }
            // car
            if(interp.car_sym == CAR(sexpr)) {
                sexpr_t* e1 = eval(CAR(CDR(sexpr)), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }
                if(e1 == interp.nil_sym) {
                    return interp.nil_sym;
                }
                return CAR(e1);
            }
            // cdr
            if(interp.cdr_sym == CAR(sexpr)) {
                sexpr_t* e1 = eval(CAR(CDR(sexpr)), env, roots, error);

                if(*error != NULL) {
                    return NULL;
                }
                if(e1 == interp.nil_sym) {
                    return interp.nil_sym;
                }
                sexpr_t *res = CDR(e1);
                if(res == NULL) {
                    return interp.nil_sym;
                }
                return res;
            }
            // +
            if(interp.plus_sym == CAR(sexpr)) {
                sexpr_t* e1 = eval(CAR(CDR(sexpr)), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }

                roots = cons_to_roots_list(roots, e1); 
                sexpr_t* e2 = eval(CAR(CDR(CDR(sexpr))), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }
                if(INT(e1) && INT(e2)) {
                    return mk_int(INT_VAL(e1) + INT_VAL(e2));
                }

                *error = mk_error("Arguments for '+' are not integers", "");
                return NULL;
            }
            // -
            if(interp.minus_sym == CAR(sexpr)) {
                sexpr_t* e1 = eval(CAR(CDR(sexpr)), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }
                roots = cons_to_roots_list(roots, e1);
                sexpr_t* e2 = eval(CAR(CDR(CDR(sexpr))), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }
                if(INT(e1) && INT(e2)) {
                    return mk_int(INT_VAL(e1) - INT_VAL(e2));
                }

                *error = mk_error("Arguments for '-' are not integers", "");
                return NULL;
            }
            if(interp.mul_sym == CAR(sexpr)) {
                sexpr_t* e1 = eval(CAR(CDR(sexpr)), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }

                roots = cons_to_roots_list(roots, sexpr); 
                sexpr_t* e2 = eval(CAR(CDR(CDR(sexpr))), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }
                if(INT(e1) && INT(e2)) {
                    return mk_int(INT_VAL(e1) * INT_VAL(e2));
                }

                *error = mk_error("Arguments for '*' are not integers", "");
                return NULL;
            }
            // cons
            if(interp.cons_sym == CAR(sexpr)) {
                sexpr_t* e1 = eval(CAR(CDR(sexpr)), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }
                roots = cons_to_roots_list(roots, e1);
                sexpr_t* e2 = eval(CAR(CDR(CDR(sexpr))), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }
                return mk_cons(e1 == interp.nil_sym ? NULL : e1, e2 == interp.nil_sym ? NULL : e2);
            }
            // def
            if(interp.def_sym == CAR(sexpr)) {
                sexpr_t* arg = CAR(CDR(CDR(sexpr)));
                roots = cons_to_roots_list(roots, arg);
                sexpr_t* val = eval(arg, env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }
                
                *env = mk_cons(mk_cons(intern(SYM_VAL(CAR(CDR(sexpr)))), val), *env);
                return val;
            }
            // print
            if(interp.print_sym == CAR(sexpr)) {
                sexpr_t* e1 = eval(CAR(CDR(sexpr)), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }
                print_sexpr(e1);
                printf("\n");

                return e1;
            }
            // fn
            if(interp.fn_sym == CAR(sexpr)) {
                return mk_fn(sexpr, *env);
            }
            // macro
            if(interp.macro_sym == CAR(sexpr)) {
                return mk_macro(sexpr);
            }
            //eval
            if(interp.eval_sym == CAR(sexpr)) {
                sexpr_t* e1 = eval(CAR(CDR(sexpr)), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }

                roots = cons_to_roots_list(roots, e1);
                return eval(e1, env, roots, error);
            }
            // else resolves first variable

            sexpr_t* fn = eval(CAR(sexpr), env, roots, error);
            if(*error != NULL) {
                return NULL;
            }

            // eval fn
            if(FN(fn)) {
                sexpr_t* fn_code = CAR(CDR(CDR(CAR(fn))));
                sexpr_t* captured_env = CDR(fn);
                sexpr_t* arguments = eval_list(CDR(sexpr), env, roots, error);
                if(*error != NULL) {
                    return NULL;
                }

                sexpr_t* pairs = pair(CAR(CDR(CAR(fn))), arguments);
                sexpr_t* eval_env = append(pairs, captured_env);

                // append the function itself to the env, roots, for recursive calls
                eval_env = mk_cons(mk_cons(CAR(sexpr), fn), eval_env);
                
                /* printf("fn code=\n"); */
                /* print_sexpr(fn_code); */
                /* printf("\n"); */
                roots = cons_to_roots_list(roots, eval_env);
                return eval(fn_code, &eval_env, roots, error);
            }

            // eval macro
            if(MACRO(fn)) {
                sexpr_t* macro_code = CAR(CDR(CDR(CAR(fn))));
                sexpr_t* pairs = pair(CAR(CDR(CAR(fn))), CDR(sexpr));
                sexpr_t* eval_env = append(pairs, *env);

                roots = cons_to_roots_list(roots, eval_env);
                sexpr_t* transformed_code = eval(macro_code, &eval_env, roots, error);

                printf("tr code=\n");
                print_sexpr(transformed_code);
                printf("\n");
                
                if(*error != NULL) {
                    return NULL;
                }

                return eval(transformed_code, env, roots, error);
            }
            
            // else primitives
            sexpr_t* arguments = eval_list(CDR(sexpr), env, roots, error);
            if(*error != NULL) {
                return NULL;
            }
            sexpr_t* to_eval = mk_cons(fn, arguments);
            return eval(to_eval, env, roots, error);
        }
    } else if(CAR(CAR(sexpr)) == interp.fn_sym) {
        // executes an anonymous function

        sexpr_t* fn = CAR(sexpr);
        sexpr_t* fn_code = CAR(CDR(CDR(fn)));
        sexpr_t* arguments = eval_list(CDR(sexpr), env, roots, error);
        if(*error != NULL) {
            return NULL;
        }
        
        sexpr_t* l = pair(CAR(CDR(fn)), arguments);
        l = append(l, *env);

        roots = cons_to_roots_list(roots, l);
        return eval(fn_code, &l, roots, error);
    }

    print_sexpr(sexpr);
    printf("\n");
    *error = mk_error("Invalid expression", "");

    return NULL;
}
