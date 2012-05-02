#include <stdio.h>

#include "cgreen/cgreen.h"

#include "read.h"
#include "error.h"

#define LENGTH(x) (sizeof(x)/sizeof(char*))

void test_tokenize()
{
    char *symbols[] = {"hello", "JMC", " something "};
    char *integers[] = {"0", "1", "123", " 456"};
    char *lparens[] = {"(", " ("};
    char *rparens[] = {")", "   \n)"};
    char *dots[] = {".", "\f."};
    char *sexprs[] = {"(cons 1 2) (car ", "(car a b)", "(fn () t)"};
    char *next = NULL;
    
    for(int i = 0; i < LENGTH(symbols); i++) {
        token_t token = tokenize(symbols[i], &next);
        printf("|%s| == %s (SYMBOL) %s\n", symbols[i], tokenstr(token), token.value);
    }
    
    for(int i = 0; i < LENGTH(integers); i++) {
        token_t token = tokenize(integers[i], &next);
        printf("|%s| == %s (INTEGER) %s\n", integers[i], tokenstr(token), token.value);
    }

    for(int i = 0; i < LENGTH(lparens); i++) {
        token_t token = tokenize(lparens[i], &next);
        printf("|%s| == %s (LPARENS) %s\n", lparens[i], tokenstr(token), token.value);
    }

    for(int i = 0; i < LENGTH(rparens); i++) {
        token_t token = tokenize(rparens[i], &next);
        printf("|%s| == %s (RPARENS) %s\n", rparens[i], tokenstr(token), token.value);
    }

    for(int i = 0; i < LENGTH(dots); i++) {
        token_t token = tokenize(dots[i], &next);
        printf("|%s| == %s (DOTS) %s\n", dots[i], tokenstr(token), token.value);
    }

    for(int i = 0; i < LENGTH(sexprs); i++) {
        tokenize(sexprs[i], &next);
        printf("!!%s!! next = !!%s!!\n", sexprs[i], next);
        char *p = next;
        tokenize(next, &next);
        printf("!!%s!! next = !!%s!!\n", p, next);
        p = next;
        tokenize(next, &next);
        printf("!!%s!! next = !!%s!!\n", p, next);
    }

}

void test_read()
{
    char *sexprs[] = {"a b c", "(cons 1 2)", "(cdr (quote a b c))", "(a b c d)", "(a . b)", "(cons 1 nil)", "(cons nil 2)", "(def inc (fn (x) (+ x 1)))", "(def x-y 12)", "(fn () t)" };
    /* char *sexprs_errors[] = {"(car (cdr (quote a b c)) 12", "13a b", "(car a b))"}; // "(a b . c)" invalid? */
    error_t* error = NULL;

    for(int i = 0; i < LENGTH(sexprs); i++) {
        error = NULL;
        printf("sexpr = %s\n", sexprs[i]);
        sexpr_t* res = read(sexprs[i], &error);
        assert_true(error == NULL);
        assert_true(res != NULL);
        print_sexpr(res);
        printf("\n"); 
    }

    /* for(int i = 0; i < LENGTH(sexprs_errors); i++) { */
    /*     cons_t* cons = read(sexprs_errors[i], &error); */
    /*     printf("reading |%s| = %d (expected 1) error = |%s|\n", sexprs_errors[i], cons == NULL, error); */
    /* } */
}

int main()
{
    TestSuite *suite = create_test_suite();
    
    /* test_tokenize(); */
    add_test(suite, test_read);

    return run_test_suite(suite, create_text_reporter());
}
