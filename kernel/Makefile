
CFLAGS  := -Wall -fno-stack-protector -fno-builtin -nostdinc -g -c -I../libflisp/include

DEPS= system.h
OBJS= start.o system.o screen.o gdt.o idt.o isrs.o irq.o timer.o kb.o 

all: floppy.img kernel.bin

floppy.img: kernel.bin
	./make-floppy.sh

kernel.bin: $(OBJS)
	ld -T link.ld -o kernel.bin $(OBJS) -L../ -lflisp
	@echo Done!

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

start.o: 
	nasm  -f elf -o start.o start.asm

clean:
	rm -f *.o *.bin floppy.img
