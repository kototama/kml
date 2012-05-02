#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "read.h"
#include "sexpr.h"
#include "stack.h"
#include "symbol.h"
#include "error.h"

token_t mk_token(token_type_t type, char *value)
{
    token_t token;

    token.type = type;
    token.value = value;
    
    return token;
}

char* substr(char* str, size_t offset)
{
    char *s = malloc(offset + 1);
    strncpy(s, str, offset);
    return s;
}

token_t tokenize(char *str, char **next)
{
    char *p = str;
    char *begin_token = NULL;

    while(isspace(*p)) {
        // skip whitespaces
        p++;
    }
    begin_token = p;

    while(*p != '\0') {
        if(*p == '(') {
            *next = ++p;
            return mk_token(LPAREN, "(");
        }
        if(*p == ')') {
            *next = ++p;
            return mk_token(RPAREN, ")");
        }
        if(*p == '.') {
            *next = ++p;
            return mk_token(DOT, ".");
        }
        if(isdigit(*p)) {
            while(*++p != '\0') {
                if(!isdigit(*p)) {
                    if(isspace(*p) || (*p == '(') || (*p == ')')) {
                        *next = p;
                        char *s = substr(begin_token, p - begin_token);
                        return mk_token(INTEGER, s);
                    }
                    *next = p;
                    return mk_token(ERROR, NULL);
                }
            }
            *next = p;
            char *s = substr(begin_token, p - begin_token);
            return mk_token(INTEGER, s);
        }
        if(isalpha(*p) || *p == '-' || *p == '*' || *p == '/' || *p == '+') {
            while(*++p != '\0') {
                if(!isalpha(*p) && !isdigit(*p) && *p != '-' && *p != '*' && *p != '/' && *p != '+') {
                    if(isspace(*p) || (*p == '(') || (*p == ')')) {
                        *next = p;
                        
                        char *s = substr(begin_token, p - begin_token);
                        return mk_token(SYMBOL, s);
                    }
                    *next = p;
                    return mk_token(ERROR, NULL);
                }
            }
            *next = p;
            char *s = substr(begin_token, p - begin_token);
            return mk_token(SYMBOL, s);
        }
        // no matched, then error
        return mk_token(ERROR, NULL);
        
        // TODO comments

        p++;
    }

    *next = p;
    return mk_token(END, NULL);
}

char* tokenstr(token_t token)
{
    switch(token.type) {
    case LPAREN:
        return "LPAREN";
        break;
    case RPAREN:
        return "RPAREN";
        break;
    case DOT:
        return "DOT";
        break;
    case SYMBOL:
        return "SYMBOL";
        break;
    case INTEGER:
        return "INTEGER";
        break;
    case END:
        return "END";
        break;
    case ERROR:
        return "ERROR";
        break;
    default:
        return "<unknown>";
    }
}

// returns NULL if nothing was read without setting an error
sexpr_t* read(char* str, error_t** error)
{
    char *next = NULL;
    char *s = str;
    token_t token;
    int paren = 0;
    sexpr_stack_t stack;

    stack_init(&stack);
    stack_mark(&stack);
    
    do {
        token = tokenize(s, &next);
        if(token.type == ERROR || token.type == END) {
            break;
        }
        if(token.type == LPAREN) {
            paren++;
            stack_mark(&stack);
        } else if(token.type == RPAREN) {
            paren--;
            if(paren < 0) {
                // too much closing paren
                token.type = ERROR;
                break;
            }
            
            stack_recons_sexpr(&stack);
            /* printf("recons:\n"); */
            /* print_sexpr(stack_top(&stack)); */
            /* printf("\n"); */
        }
        
        if(token.type == SYMBOL) {
            // printf("adding '%s'\n", token.value);
            stack_push_sexpr(&stack, intern(token.value));
        } else if(token.type == INTEGER) {
            // printf("adding '%s'\n", token.value);
            stack_push_sexpr(&stack, mk_int(atoi(token.value)));
        }

        s = next;
        
    } while(token.type != ERROR && *next != '\0');

    if(token.type == ERROR) {
        *error  = mk_error("Invalid syntax", s);
        return NULL;
    }

    if(paren != 0) {
        *error = mk_error("Unbalanced parenthesis", s);
        return NULL;
    }

    stack_recons_sexpr(&stack); 
    return stack_top(&stack);
}

