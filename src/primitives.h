#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "sexpr.h"

sexpr_t* assoc(sexpr_t* x, sexpr_t* list);
sexpr_t* pair(sexpr_t* symbols, sexpr_t *values);
sexpr_t* append(sexpr_t* list, sexpr_t *rest);

#endif
