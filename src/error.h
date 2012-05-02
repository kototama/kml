#ifndef ERROR_H
#define ERROR_H

typedef struct error_s {
    char *msg;
    char *token;
} error_t;

error_t* mk_error(char* msg, char* token);
void print_error(error_t* error);

#endif
