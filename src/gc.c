#include <stdlib.h>
#include <stdio.h>

#include "gc.h"
#include "interp.h"
#include "sexpr.h"

#define MAX_SEXPR 10000
#define GC_THRESHOLD 100

sexpr_list_t* cons_to_roots_list(sexpr_list_t* roots, sexpr_t* sexpr)
{
    sexpr_list_t* t = malloc(sizeof(sexpr_list_t));
    t->sexpr = sexpr;
    t->next = roots;
    
    return t;
}

void mark(sexpr_t* sexpr)
{
    sexpr->flags = sexpr->flags | GC_F;

    /* printf("[visiting] %p\n", sexpr); */
    /* print_sexpr(sexpr); */
    /* printf("\n"); */
        
    if(CONS(sexpr)) {
        if(CAR(sexpr) != NULL) {
            mark(CAR(sexpr));
        }
        if(CDR(sexpr) != NULL) {
            mark(CDR(sexpr));
        }
    }
}

void sweep()
{
    sexpr_list_t* current = interp.used_list;
    sexpr_list_t* previous = NULL;
    
    // TODO: why is (args) freed?
    // (on the closure.lisp example)

    while(current != NULL) {
        /* printf("[examining] %p\n", current->sexpr); */
        /* print_sexpr(current->sexpr); */
        /* printf("\n"); */
        if(!GC(current->sexpr)) {
            /* printf("[freeing] %p\n", current->sexpr); */
            /* print_sexpr(current->sexpr); */
            /* printf("\n"); */
            // TODO: free symbols
            if(previous == NULL) {
                interp.used_list = current->next;
            } else {
                // remove current from the used list
                previous->next = current->next;
            }
            // assign current to the free list
            current->next = interp.free_list;
            interp.free_list = current;
        }

        current->sexpr->flags &= ~GC_F;
        previous = current;
        current = current->next;
    }
}

void mark_roots(sexpr_list_t* roots)
{
    sexpr_list_t* current = roots;
    while(current != NULL) {
        /* printf("[root] = \n\n"); */
        /* print_sexpr(current->sexpr); */
        /* printf("\n\n"); */
        mark(current->sexpr);
        current = current->next;
    }
}

void gc_collect(sexpr_list_t* roots)
{
    // printf("[gc] %d\n", interp.nb_sexpr_since_gc);
    if(interp.nb_sexpr_since_gc > GC_THRESHOLD) {
        // printf(" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        /* printf("mark\n"); */
        mark_roots(roots);
        /* printf("sweep\n"); */
        sweep();
        interp.nb_sexpr_since_gc = 0;
        // printf(" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    }
}

sexpr_t* gc_malloc()
{
    //    return malloc(sizeof(sexpr_t));
    sexpr_t *s;

    if(interp.free_list != NULL) {
        s = interp.free_list->sexpr;

        /* printf("---------- reusing sexpr\n"); */
        /* print_sexpr(s); */
        /* printf("\n"); */

        SET_CAR(s, NULL);
        SET_CDR(s, NULL);
        interp.free_list = interp.free_list->next;
        
        s->flags &= ~GC_F;
        return s;
    } else if(interp.nb_sexpr < MAX_SEXPR) {
        s = malloc(sizeof(sexpr_t));
        // printf("[creating] %p\n", s);
        sexpr_list_t* t = malloc(sizeof(sexpr_list_t));
        interp.nb_sexpr++;
        interp.nb_sexpr_since_gc++;
        t->sexpr = s;
        t->next = interp.used_list;
        interp.used_list = t;

        return s;
    } else {
        printf("You ate all the memory :-(. Exiting.\n");
        exit(-1);
    }

    return NULL;
}
