
IDIR=libflisp/include
LDIR=libflisp/libs
CC=gcc
WARN=-Wall
NOBUILTIN=-fno-builtin -nostdinc 
CFLAGS=-I$(IDIR) -I$(LDIR) -Ilisp -Ilibflisp/libs/fdlibm-5.2/ -g $(WARN)


ODIR=libflisp/obj
SDIR=libflisp/src
LDIR=lisp

_DEPS = gc.h sys.h symbol.h types.h lists.h ht.h array.h flisp.h env.h eval.h procs.h error.h reader.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS)) 

_OBJ = gc.o sys.o symbol.o types.o lists.o ht.o array.o flisp.o env.o eval.o procs.o error.o reader.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ)) lisp/flisp-core.o

_LISP = core.lisp lists.lisp macros.lisp math.lisp structs.lisp
LISP = $(patsubst %,$(LDIR)/%,$(_LISP))

all: libm.a libflisp.a flisp kernel/floppy.img

kernel/floppy.img: libflisp.a
	cd kernel; make -f Makefile
	cd kernel; ./make-floppy.sh
	cp kernel/floppy.img .

libflisp.a: $(OBJ)
	ar rcs libflisp.a $(OBJ)

libm.a:
	cd libflisp/libs/fdlibm-5.2/; make

flisp: $(ODIR)/main.o libflisp.a 
	gcc -o $@ $(ODIR)/main.o -L. -Libflisp/libs/fdlibm-5.2/ -lflisp -lm

$(ODIR)/%.o: ${SDIR}/%.c $(DEPS)
	$(CC) -c -o $@ $< ${CFLAGS} $(NOBUILTIN)

${ODIR}/main.o: ${SDIR}/main.c ${DEPS}
	${CC} -c -o ${ODIR}/main.o ${SDIR}/main.c ${CFLAGS}

lisp/flisp-core.o: ${LISP}
	cd lisp; ./make-core.sh
	${CC} -c -o lisp/flisp-core.o lisp/flisp-core.c

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ flisp libflisp.a floppy.img lisp/flisp-core.c lisp/flisp-core.o
	cd kernel; make -f Makefile clean
	cd libflisp/libs/fdlibm-5.2; make clean

