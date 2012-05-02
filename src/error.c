#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

error_t* mk_error(char* msg, char* token)
{
    error_t* error = malloc(sizeof(error_t));
    error->msg = malloc(strlen(msg) + 1);
    error->token = malloc(strlen(token) + 1);
    
    strcpy(error->msg, msg);
    strcpy(error->token, token);

    return error;
}

void print_error(error_t* error)
{
    printf("Error. %s: %s\n", error->msg, error->token);
}
