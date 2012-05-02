#ifndef EVAL_H
#define EVAL_H

#include "sexpr.h"
#include "error.h"
#include "gc.h"

sexpr_t* eval_list(sexpr_t* list, sexpr_t** env, sexpr_list_t* roots, error_t** error);
sexpr_t* eval(sexpr_t* sexpr, sexpr_t** env, sexpr_list_t* roots, error_t** error);

#endif
