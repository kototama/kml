#include <stdio.h>
#include <string.h>

#include "sexpr.h"
#include "cgreen/cgreen.h"

void test_cons_int()
{
    sexpr_t* cons = mk_cons(mk_int(1), mk_int(2));

    assert_true(ATOM(CAR (cons)));
    assert_true(ATOM(CDR (cons)));
    assert_true(INT_VAL(CAR(cons)) == 1);
    assert_true(INT_VAL(CDR(cons)) == 2);
    
    print_sexpr(cons);
    printf("\n");

}

void test_cons_int_cons()
{
    sexpr_t* cons = mk_cons(mk_int(2), mk_int(3));
    sexpr_t* cons2 = mk_cons(mk_int(1), cons);

    assert_true(ATOM(CAR(cons2)));
    assert_true(!ATOM(CDR(cons2)));

    sexpr_t* cdr = CDR(cons2);
    assert_true(ATOM(CAR(cdr)));
    assert_true(ATOM(CDR(cdr)));
    assert_true(INT_VAL(CAR(cdr)) == 2);
    assert_true(INT_VAL(CDR(cdr)) == 3);
    
    print_sexpr(cons2);
    printf("\n");
}

void test_cons_sym_sym()
{
    sexpr_t* cons = mk_cons(mk_sym("list"), mk_sym("one"));

    print_sexpr(cons);
    printf("\n");

    assert_true(ATOM(CAR(cons)));
    assert_true(ATOM(CDR(cons)));
    assert_true(SYM(CAR(cons)));
    assert_true(SYM(CDR(cons)));
    assert_true(strcmp(SYM_VAL(CAR(cons)), "list") == 0);
    assert_true(strcmp(SYM_VAL(CDR(cons)), "one") == 0);
    
}


void test_make_cons_sym_cons()
{
    sexpr_t* cons = mk_cons(mk_sym("3"), NULL);
    sexpr_t* cons2 = mk_cons(mk_sym("2"), cons);
    sexpr_t* cons3 = mk_cons(mk_sym("1"), cons2);
    sexpr_t* cons4 = mk_cons(mk_sym("list"), cons3);
 
    print_sexpr(cons4);
    printf("\n");
}


void test_make_cons_sym_cons2()
{
    sexpr_t* cons = mk_cons(mk_sym("three"), NULL);
    sexpr_t* cons2 = mk_cons(mk_sym("two"), cons);
    sexpr_t* cons3 = mk_cons(mk_sym("one"), cons2);
    sexpr_t* cons4 = mk_cons(mk_sym("list"), cons3);
    sexpr_t* cons5 = mk_cons(cons4, mk_sym("last"));
    
    print_sexpr(cons5);
    printf("\n");
}

void test_make_cons_sym_cons3()
{
    sexpr_t* cons = mk_cons(mk_sym("three"), NULL);
    sexpr_t* cons2 = mk_cons(mk_sym("two"), cons);
    sexpr_t* cons3 = mk_cons(mk_sym("one"), cons2);
    sexpr_t* cons4 = mk_cons(mk_sym("list"), cons3);
    
    sexpr_t* cons5 = mk_cons(mk_sym("car"), mk_cons(cons4, NULL));
    
    print_sexpr(cons5);
    printf("\n");
}

void test_cons_nil()
{
    sexpr_t* cons = mk_cons(mk_sym("one"), NULL);

    print_sexpr(cons);

    assert_true(CDR(cons) == NULL);
}

int main()
{
    TestSuite *suite = create_test_suite();

    test_make_cons_sym_cons();
    test_make_cons_sym_cons2();
    test_make_cons_sym_cons3();
    
    add_test(suite, test_cons_int);
    add_test(suite, test_cons_int_cons);
    add_test(suite, test_cons_sym_sym);
    add_test(suite, test_cons_nil);
    
    return run_test_suite(suite, create_text_reporter());
}
