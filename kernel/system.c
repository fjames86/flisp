
#include "system.h"

/* already defined in libflisp */
#if 0
/* You will need to code these up yourself!  */
void *memcpy(void *dest, void *src, int count)
{
	/* Add code here to copy 'count' bytes of data from 'src' to
	 *  'dest', finally return 'dest' */
	unsigned char *p = (unsigned char *)dest;
	unsigned char *s = (unsigned char *)src;
	while (count > 0) {
		*p = *s;
		p++;
		s++;
		count--;
	}
	return dest;
}

void *memset(void *dest, unsigned char val, int count)
{
	/* Add code here to set 'count' bytes in 'dest' to 'val'.
	 *  Again, return 'dest' */
	unsigned char *p = (unsigned char *)dest;	
	while (count > 0) {
		*p = val;
		p++;
		count--;
	}
	return dest;
}
#endif

void *memsetw(void *dest, unsigned short val, int count)
{
	/* Same as above, but this time, we're working with a 16-bit
	 *  'val' and dest pointer. Your code can be an exact copy of
	 *  the above, provided that your local variables if any, are
	 *  unsigned short */
	unsigned short *p = (unsigned short *)dest;
	while (count > 0) {
		*p = val;
		p++;
		count--;
	}
	return dest;
}

void int_to_string (char *buffer, int i) {
	if (i < 0) {
		*buffer = '-';
		buffer++;
		i = -i;
	}

	if (i == 0) {
		*buffer = '\0';
	} else {
		if (i / 10 != 0) {
			int_to_string (buffer++, i / 10);
		}
		*buffer = '0' + i % 10;
	}
}

void print_uint (unsigned int i) {
  if (i / 10 != 0) {
    print_int (i / 10);
  }
  putch ('0' + (i % 10));
}

void print_ulong (unsigned long l) {
  print_uint (l >> 32);
  print_uint ((unsigned int)(l & 0x00000000FFFFFFFFL));
}

/* We will use this later on for reading from the I/O ports to get data
 *  from devices such as the keyboard. We are using what is called
 *  'inline assembly' in these routines to actually do the work */
unsigned char inportb (unsigned short _port)
{
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
 *  will be used in the next tutorial for changing the textmode cursor
 *  position. Again, we use some inline assembly for the stuff that simply
 *  cannot be done in C */
void outportb (unsigned short _port, unsigned char _data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}



 
void __stack_chk_guard_setup()
{
  unsigned char * p;
  p = (unsigned char *) &__stack_chk_guard;
 
  /* If you have the ability to generate random numbers in your kernel then use them,
     otherwise for 32-bit code: */
  *p =  0x00000aff;
}
 
void __attribute__((noreturn)) __stack_chk_fail()
{ 
  /* put your panic function or similar in here */
  unsigned char * vid = (unsigned char *)0xB8000;
  vid[1] = 7;
  for(;;)
	  vid[0]++;
}


/* find usable memory */
void get_usable_memory (void **start, size_t *length, multiboot_info_t *mbt) {
  multiboot_memory_map_t *mmap = mbt->mmap_addr;
  unsigned int len, type, addr_low;

  while (mmap < mbt->mmap_addr + mbt->mmap_length) {
    type = mmap->type;
    addr_low = mmap->addr.base_addr_low >> 32;
    len = mmap->length.length_low >> 32;
    
    /* found the first block of useable memory */
    if (type == 1 && addr_low > 0) {
      *start = addr_low;
      *length = len;
      return;
    }
    
    mmap = (multiboot_memory_map_t *)((unsigned int)mmap + mmap->size + sizeof(unsigned int));
  }

  /* no memory found */
  *start = NULL;
  *length = 0;
}


void print_memory (multiboot_info_t *mbt) {
  multiboot_memory_map_t *mmap = mbt->mmap_addr;

  puts ("Type   Address               length    \n");
  while (mmap < mbt->mmap_addr + mbt->mmap_length) {
    print_uint (mmap->type);
    puts ("   ");

    puts (" 0x");
    print_hex (mmap->addr.base_addr_low >> 32);

    puts (" 0x");
    print_hex (mmap->length.length_low >> 32);

    puts ("\n");
    
    mmap = (multiboot_memory_map_t *)((unsigned int)mmap + mmap->size + sizeof(unsigned int));
  }
  
}

#define HEAP_START 0x00000000
#define HEAP_SIZE  (1024 * 1024)
#define SYMTAB_START (HEAP_START + HEAP_SIZE)
#define SYMTAB_SIZE (1024*1024)
#define MIN_MEMORY (HEAP_SIZE + SYMTAB_SIZE)

/* reserve 1mb for the kernel */
#define KERNEL_SIZE 0x100000

/* This is a very simple main() function. All it does is sit in an
 *  infinite loop. This will be like our 'idle' loop */
int main(multiboot_info_t *mbt, unsigned int magic)
{
	int i;
	void *heap, *symt, *memstart;
	unsigned int heaplength, symtlength, memlength;
	type_cell *c;
	void *expr;
	char word[MAX_LINE], *ch;

	/* You would add commands after here */
	init_video ();
	puts ("Welcome to flisp-os, Frank's little operating system! Copyright Frank James " __DATE__ "\n");

	gdt_install ();
	idt_install ();
	isrs_install ();
	irq_install ();
	__asm__ __volatile__ ("sti");
	timer_install ();
	keyboard_install ();

	/* print the memory */
#if 0
	print_memory (mbt);
	puts ("Press any key to continue\n");
#endif

	/* set the meory */
	get_usable_memory (&memstart, &memlength, mbt);
	puts ("Found memory starting at ");
	print_hex (memstart);
	puts (" of length ");
	print_hex (memlength);
	puts ("\n");

	if (memlength == 0) {
	  puts ("No usable memory found, out of luck!\n");
	  for (;;);
	} 

	if (memlength < KERNEL_SIZE) {
	  puts ("Not even enough memory for the kernel?\n");
	  for (;;);
	}

	memstart += KERNEL_SIZE;
	memlength -= KERNEL_SIZE;

	if (memlength < MIN_MEMORY) {
	  puts ("Insufficient memory available, out of luck!\n");
	}
	
	puts ("Using memory starting at ");
	print_hex (memstart);
	puts (" length ");
	print_hex (memlength);
	puts ("\n");

	/* set up the heap and symbol table */
	symt = memstart;
	symtlength = SYMTAB_SIZE;
	heap = memstart + symtlength;
	heaplength = memstart + memlength - heap;

	/* setup the flisp environment */
	puts ("Setting up heap at "); print_hex (heap); 
	puts (" of length "); print_hex (heaplength); puts ("\n");
	gc_init(heap, heaplength);

	puts ("Setting up symbol table at "); print_hex (symt); 
	puts (" of length "); print_hex (symtlength); puts ("\n");
	symbol_init (symt, symtlength);
	
	puts ("Environment init... \n");
	env_init (&toplevel);

	refresh_buffer_f = &rebuffer;
	memset (reader_buffer, '\0', MAX_LINE);
	reader_bufferp = reader_buffer;

	/* load flisp core */
	load_core ();
	
	/* enter the flisp repl */
	flisp_repl (FALSE);

	/* ...and leave this loop in. There is an endless loop in
	 *  'start.asm' also, if you accidentally delete this next line */
	for (;;) {
	}

	return 0;
}

