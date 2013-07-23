
IDIR=include
CC=gcc
CFLAGS=-I$(IDIR) -c -fno-builtin -nostdinc -g

ODIR=obj
SDIR=src

_DEPS = gc.h sys.h symbol.h types.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = gc.o sys.o test.o symbol.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

test: $(OBJ)
	gcc -o $@ $^

${ODIR}/test.o: ${SDIR}/test.c ${DEPS}
	${CC} -c -g -o ${ODIR}/test.o ${SDIR}/test.c -I${IDIR}

$(ODIR)/%.o: ${SDIR}/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 