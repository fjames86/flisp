
#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "multiboot.h"
#include "flisp.h"

/* system.c */
static void * __stack_chk_guard = NULL;

extern void *memcpy(void *dest, void *src, int count);
extern void *memset(void *dest, unsigned char val, int count);
extern void *memsetw(void *dest, unsigned short val, int count);
extern void int_to_string (char *buffer, int i);
extern void print_int (int i);
extern void print_uint (unsigned int i);
extern void print_hex (unsigned int i);
extern char *string_upcase (char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);
extern void __stack_chk_guard_setup();
extern void __attribute__((noreturn)) __stack_chk_fail();
extern void get_usable_memory (void **start, size_t *length, multiboot_info_t *mbt);
extern void print_memory (multiboot_info_t *mbt);

/* screen.c */
extern void scroll();
extern void move_csr();
extern void cls();
extern void putch(unsigned char c);
extern void puts(unsigned char *str);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

/* gdt.h */
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();

/* idt.h */
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

/* This defines what the stack looks like after an ISR was running */
struct regs
{
	unsigned int gs, fs, es, ds;      /* pushed the segs last */
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
	unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
	unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};

/* isrs */
extern void isrs_install();
extern void fault_handler(struct regs *r);

/* irq */
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_remap(void);
extern void irq_install();
extern void irq_handler(struct regs *r);

/* timer */
extern void timer_phase(int hz);
extern void timer_handler(struct regs *r);
extern void timer_install();
extern void timer_wait(int ticks);

/* keynoard */
extern void keyboard_handler(struct regs *r);
extern void keyboard_install ();
extern char getch ();
extern void gets (char *buffer, unsigned int n);


/* flisp compat */
extern void rebuffer ();

#endif
