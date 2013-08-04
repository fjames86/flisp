
#include "system.h"

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

int strlen(char *str)
{
	/* This loops through character array 'str', returning how
	 *  many characters it needs to check before it finds a 0.
	 *  In simple words, it returns the length in bytes of a string */
	char *p = str;
	unsigned int counter = 0;
	while (*p != '\0') {
		p++;
		counter++;
	}
	return counter;
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

/* This is a very simple main() function. All it does is sit in an
 *  infinite loop. This will be like our 'idle' loop */
int main()
{
	int i;
	
	/* You would add commands after here */
	init_video ();
	puts ("Welcome to Frank's OS! Copyright Frank James " __DATE__);

	gdt_install ();
	idt_install ();
	isrs_install ();
	irq_install ();
	__asm__ __volatile__ ("sti");
	timer_install ();
	keyboard_install ();

	/* ...and leave this loop in. There is an endless loop in
	 *  'start.asm' also, if you accidentally delete this next line */
	for (;;);

	return 0;
}

