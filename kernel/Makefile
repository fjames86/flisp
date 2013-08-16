
CC=gcc
#AS=nasm
AS=/home/frank/Downloads/nasm-2.10.01/nasm
CFLAGS  := -Wall -fno-stack-protector -fno-builtin -nostdinc -g -c -I../libflisp/include -I../lisp/ -I../libflisp/libs/fdlibm-5.2/

DEPS= system.h
OBJS= start.o system.o screen.o gdt.o idt.o isrs.o irq.o timer.o kb.o flisp_compat.o

all: floppy.img kernel.bin

floppy.img: kernel.bin
	./make-floppy.sh

kernel.bin: $(OBJS)
	ld -T link.ld -o kernel.bin $(OBJS) -L../ -L../libflisp/libs/fdlibm-5.2/ -lflisp -lm
	@echo Done!

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

start.o: 
	$(AS) -f elf -o start.o start.asm

clean:
	rm -f *.o *.bin floppy.img
