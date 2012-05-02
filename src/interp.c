#include <stdio.h>
#include <stdlib.h>

#include "interp.h"
#include "sexpr.h"

interp_t interp;

#define LENGTH(x) (sizeof(x)/sizeof(char*))

void init_interp()
{
    interp.free_list = NULL;
    interp.used_list = NULL;
    interp.nb_sexpr = 0;
    interp.nb_sexpr_since_gc = 0;
    init_symbol_table(&interp.sym_table);
    interp.cons_sym = intern("cons");
    interp.nil_sym = intern("nil");
    interp.quote_sym = intern("quote");
    interp.car_sym = intern("car");
    interp.cdr_sym = intern("cdr");
    interp.atom_sym = intern("atom");
    interp.t_sym = intern("t");
    interp.eq_sym = intern("eq");
    interp.def_sym = intern("def");
    interp.fn_sym = intern("fn");
    interp.if_sym = intern("if");
    interp.plus_sym = intern("+");
    interp.minus_sym = intern("-");
    interp.mul_sym = intern("*");
    interp.print_sym = intern("print");
    interp.macro_sym = intern("macro");
    interp.eval_sym = intern("eval");
    interp.env = mk_env();
}

sexpr_t* mk_env()
{
    char* builtins[] = {"quote", "car", "cdr", "atom", "eq", "def", "fn", "if",
                        "+", "*", "-", "print", "macro", "eval"};
    sexpr_t* env = mk_cons(mk_cons(intern("cons"), intern("cons")), NULL);

    sexpr_t* current = env;
    
    for(int i = 0; i < LENGTH(builtins); i++) {
        SET_CDR(current, mk_cons(mk_cons(intern(builtins[i]), intern(builtins[i])), NULL));
        current = CDR(current);
    }

    return env;
}
