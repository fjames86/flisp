
IDIR=include
CC=gcc
CFLAGS=-I$(IDIR) -c -fno-builtin -nostdinc -g

ODIR=obj
SDIR=src

_DEPS = gc.h list.h symbol.h sys.h value.h ht.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = gc.o list.o sys.o value.o test.o ht.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

test: $(OBJ)
	gcc -o $@ $^

${ODIR}/test.o: ${SDIR}/test.c ${DEPS}
	${CC} -c -o ${ODIR}/test.o ${SDIR}/test.c -I${IDIR}

$(ODIR)/%.o: ${SDIR}/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 