#include <stdio.h>

#include "symbol.h"
#include "interp.h"

#define LENGTH(x) (sizeof(x)/sizeof(char*))

void test_hash_symbol()
{
    char* symbols[]= {"a", "aa", "aaa", "list", "car", "nil", "somethinglong", "somethinglonh", "somethinglongg",
                      "cdr", "take", "cons", "b"};
    
    for(int i = 0; i < LENGTH(symbols); i++) {
        printf("|%s| %u\n",  symbols[i],  hash_symbol(symbols[i]));
    } 
} 

void test_intern()
{
    char* symbols[] = {"cons", "cons", "car", "car", "b", "somethinglong", "somethinglong", "cdr", "cdr"};

    for(int i = 0; i < LENGTH(symbols); i++) {
        sexpr_t *s = intern(symbols[i]);
        print_sexpr(s);
        printf("|%s| = %p\n", symbols[i], (void*)s);
    }
    
}
    
int main()
{
    init_interp();
    
    /* test_hash_symbol(); */
    test_intern();
    
    return 0;
}
