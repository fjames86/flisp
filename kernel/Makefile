
CFLAGS  := -Wall -fno-stack-protector -fno-builtin -nostdinc -g -c -I../include
LDFLAGS := -nostdlib -Wl,-N -Wl,-Ttext -Wl,100000

kernel.bin:	start.o	system.o screen.o gdt.o idt.o isrs.o irq.o timer.o kb.o 
	ld -T link.ld -o kernel.bin start.o system.o screen.o gdt.o idt.o isrs.o irq.o timer.o kb.o # ../obj/reader.o ../obj/gc.o
	@echo Done!

kernel2.bin: start.asm system.c screen.c start.o gdt.o idt.o isrs.o irq.o timer.o kb.o
	gcc -o kernel.bin $(CFLAGS) start.o system.c screen.c gdt.c idt.c isrs.c irq.c timer.c kb.c $(LDFLAGS)


start.o: start.asm
	nasm  -f elf -o start.o start.asm

system.o: system.c system.h
	gcc $(CFLAGS) -o system.o system.c

screen.o: screen.c system.h
	gcc  $(CFLAGS)  -o screen.o screen.c

gdt.o: gdt.c system.h
	gcc  $(CFLAGS) -o gdt.o gdt.c

idt.o: idt.c system.h
	gcc  $(CFLAGS) -o idt.o idt.c

isrs.o: isrs.c system.h
	gcc  $(CFLAGS) -o isrs.o isrs.c

irq.o: irq.c system.h
	gcc  $(CFLAGS) -o irq.o irq.c

timer.o: timer.c system.h
	gcc  $(CFLAGS) -o timer.o timer.c

kb.o: kb.c system.h
	gcc  $(CFLAGS) -o kb.o kb.c



clean:
	rm -f *.o *.bin
