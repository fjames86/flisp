
IDIR=include
CC=gcc
CFLAGS=-I$(IDIR) -fno-builtin -nostdinc -g

ODIR=obj
SDIR=src

_DEPS = gc.h sys.h symbol.h types.h lists.h ht.h array.h flisp.h env.h eval.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = gc.o sys.o symbol.o types.o test.o lists.o ht.o array.o flisp.o env.o eval.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

test: $(OBJ)
	gcc -o $@ $^

$(ODIR)/%.o: ${SDIR}/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

${ODIR}/test.o: ${SDIR}/test.c ${DEPS}
	${CC} -c -o ${ODIR}/test.o ${SDIR}/test.c -I${IDIR} -g

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ 
