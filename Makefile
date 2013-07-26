
IDIR=include
CC=gcc
WARN=
NOBUILTIN=-fno-builtin -nostdinc 
CFLAGS=-I$(IDIR) -g $(WARN)


ODIR=obj
SDIR=src

_DEPS = gc.h sys.h symbol.h types.h lists.h ht.h array.h flisp.h env.h eval.h procs.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = gc.o sys.o symbol.o types.o test.o lists.o ht.o array.o flisp.o env.o eval.o procs.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

test: $(OBJ)
	gcc -o $@ $^

$(ODIR)/%.o: ${SDIR}/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(NOBUILTIN)

${ODIR}/test.o: ${SDIR}/test.c ${DEPS}
	${CC} -c -o ${ODIR}/test.o ${SDIR}/test.c $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ 
