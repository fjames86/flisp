
IDIR=include
CC=gcc
WARN=
NOBUILTIN=-fno-builtin -nostdinc 
CFLAGS=-I$(IDIR) -g $(WARN)


ODIR=obj
SDIR=src

_DEPS = gc.h sys.h symbol.h types.h lists.h ht.h array.h flisp.h env.h eval.h procs.h error.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = gc.o sys.o symbol.o types.o main.o lists.o ht.o array.o flisp.o env.o eval.o procs.o error.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

flisp: $(OBJ)
	gcc -o $@ $^

$(ODIR)/%.o: ${SDIR}/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(NOBUILTIN)

${ODIR}/main.o: ${SDIR}/main.c ${DEPS}
	${CC} -c -o ${ODIR}/main.o ${SDIR}/main.c $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ 
