CFLAGS=-std=c99
CFLAGS=-std=c99 -I$(CINCLUDE) -Wall -Werror -pedantic-errors -ggdb -I/usr/local/lib
CC=clang

ALL: kml
kml: kml.c sexpr.c symbol.c stack.c interp.c eval.c primitives.c read.c error.c load.c gc.c

clean:
	$(RM) kml ; cd tests; make clean; cd -

