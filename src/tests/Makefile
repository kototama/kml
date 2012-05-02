CINCLUDE=..
CFLAGS=-std=c99 -I$(CINCLUDE) -Wall -Werror -pedantic-errors -ggdb -I/usr/local/lib -lcgreen -lm
CC=clang

ALL: t_sexpr t_symbol t_read t_eval

C_SOURCES=$(CINCLUDE)/read.c  $(CINCLUDE)/sexpr.c $(CINCLUDE)/symbol.c $(CINCLUDE)/stack.c $(CINCLUDE)/eval.c \
$(CINCLUDE)/interp.c $(CINCLUDE)/primitives.c $(CINCLUDE)/error.c $(CINCLUDE)/gc.c

t_read: t_read.c $(C_SOURCES)
t_sexpr: t_sexpr.c $(C_SOURCES)
t_symbol: t_symbol.c $(C_SOURCES)
t_eval: t_eval.c $(C_SOURCES)

clean:
	$(RM) t_read t_sexpr t_symbol t_eval
