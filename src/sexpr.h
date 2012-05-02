#ifndef SEXPR_H
#define SEXPR_H

#include <inttypes.h>

#define GC_F    0x80 // 0b10000000
#define SYM_F   0x00 // 0b00000000
#define INT_F   0x10 // 0b00010000
#define FN_F    0x30 // 0b00110000
#define MACRO_F 0x70 // 0b00111000
#define CONS_F  0x40 // 0b01000000
#define TYPE_F  0x70 // 0b01110000
#define CAR_F   0xFFFFFFFF00000000
#define CDR_F   0x00000000FFFFFFFF

typedef struct sexpr_s {
    uint8_t flags;
    // if sexpr_s is a cons, val represents two pointers pointing to the CAR and to the CDR
    // the CAR pointer is stored in the highest bits, the CDR pointer in the lowest
    int64_t val;
} sexpr_t;

#define ENC_CAR_CDR(x, y) (((((int64_t)(uintptr_t)(x)) << 32) | (((int64_t)(uintptr_t)(y)) & CDR_F))) 
#define ENC_SYM(x) ((int64_t)(uintptr_t)(x))
#define INT(x) (((x->flags) & TYPE_F) == INT_F)
#define SYM(x) (((x->flags) & TYPE_F) == SYM_F)
#define FN(x) (((x->flags) & TYPE_F) == FN_F)
#define MACRO(x) (((x->flags) & TYPE_F) == MACRO_F)
#define INT_VAL(x) ((int) (x->val))
#define SYM_VAL(x) ((char*) (uintptr_t) x->val)
#define ATOM(x) (((x->flags) & TYPE_F) != CONS_F)
#define CONS(x) (!ATOM(x))
#define CAR(x) ((sexpr_t*) (uintptr_t) ((x->val & CAR_F) >> 32))
#define CDR(x) ((sexpr_t*) (uintptr_t) (x->val & CDR_F))
#define SET_CAR(c, v) (c->val = ENC_CAR_CDR(v, CDR(c)))
#define SET_CDR(c, y) (c->val = ENC_CAR_CDR(CAR(c), y))
#define GC(x) (((x->flags) & GC_F) == GC_F)

sexpr_t* mk_sym(char *sym);
sexpr_t* mk_int(int i);
sexpr_t* mk_cons(sexpr_t* car, sexpr_t* cdr);
sexpr_t* mk_fn(sexpr_t* fn, sexpr_t* env);
sexpr_t* mk_macro(sexpr_t* macro);

void print_sexpr(sexpr_t* sexpr);

#endif
