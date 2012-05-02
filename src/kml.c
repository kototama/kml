#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "interp.h"
#include "read.h"
#include "sexpr.h"
#include "eval.h"
#include "symbol.h"
#include "error.h"
#include "load.h"

#define BUF_SIZE 8000
char buffer[BUF_SIZE];

#define CORE_PATHNAME "./core/core.lisp"

int repl()
{
    sexpr_t* sexpr = NULL;
    error_t *error = NULL;

    load_file(CORE_PATHNAME, &interp.env, &error);
    if(error != NULL) {
        print_error(error);
        return EXIT_FAILURE;
    }

    sexpr_list_t* roots_list = cons_to_roots_list(NULL, interp.env);
    
    printf("\nWelcome to KISS My Lisp.\n\n");
    for(;;) {
        printf("kml> ");
        fgets(buffer, BUF_SIZE, stdin);
        error = NULL;
        sexpr = read(buffer, &error);
        if(error != NULL) {
            print_error(error);
        } else if(sexpr != NULL && sexpr != interp.nil_sym) {
            error = NULL;
            sexpr = eval(CAR(sexpr), &interp.env, roots_list, &error);
            if(error != NULL) {
                print_error(error);
            } else {
                print_sexpr(sexpr);
                printf("\n");
            } 
        }
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    init_interp();

    if(argc == 1) {
        printf("Usage: rlwrap ./kml [-repl] [FILE].\n");
        return EXIT_SUCCESS;
    }
    if(strcmp(argv[1], "-repl") == 0) {
        return repl();
    }

    error_t* error = NULL;
    sexpr_t* env = mk_env();
    load_file(CORE_PATHNAME, &env, &error);
    if(error != NULL) {
        print_error(error);
        return EXIT_FAILURE;
    }
    
    load_file(argv[1], &env, &error);
    if(error != NULL) {
        print_error(error);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
