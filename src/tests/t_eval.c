#include <stdio.h>

#include "eval.h"
#include "sexpr.h"
#include "symbol.h"
#include "interp.h"
#include "read.h"
#include "error.h"

#include "cgreen/cgreen.h"

#define LENGTH(x) (sizeof(x)/sizeof(char*))

void test_eval()
{
    char* exprs[] = {"(cons one two)", "(cons (cons one two) 3)", "(quote a)", "1",
                     "(cdr (quote (1 2 3)))", "(car (quote (1 2 3)))", "(atom 1)", "(atom (quote (1 2 3)))", \
                     "(atom two)"
    };
    
    sexpr_t* env_one = mk_cons(intern("one"), mk_int(1));
    sexpr_t* env_two = mk_cons(intern("two"), mk_int(2));

    sexpr_t* env = mk_cons(env_one, mk_cons(env_two, NULL));
    error_t* error = NULL;

    printf("env = ");
    print_sexpr(env);
    printf("\n\n");

    sexpr_list_t* roots = NULL;
    roots = cons_to_roots_list(roots, env);
        
    for(int i = 0; i < LENGTH(exprs); i++) {
        printf("%s", exprs[i]);
        printf(" -> ");
        error = NULL;
        sexpr_t* s = read(exprs[i], &error);
        assert_true(error == NULL);
        print_sexpr(eval(CAR(s), &env, roots, &error));
        printf("\n") ;
    }

}

int main()
{
    init_interp();

    TestSuite *suite = create_test_suite();
    add_test(suite, test_eval);
    
    return run_test_suite(suite, create_text_reporter());
}
