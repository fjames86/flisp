
#include "system.h"


/* use this to store status of control, alt and shift keys */
#define KEY_SHIFT       2
#define KEY_ALT         3
#define KEY_CONTROL     4
#define KEY_NUM         5
#define KEY_SCROLL      6
#define KEY_CAPS        7


/* KBDUS means US Keyboard Layout. This is a scancode table
 *  used to layout a standard US keyboard. I have left some
 *  comments in to give you an idea of what key is what, even
 *  though I set it's array index to 0. You can change that to
 *  whatever you want using a macro, if you wish! */
unsigned char kbdus[128] =
	{
		0,  27, '1', '2', '3', '4', '5', '6', '7', '8',/* 9 */
		'9', '0', '-', '=', '\b',/* Backspace */
		'\t',/* Tab */
		'q', 'w', 'e', 'r',/* 19 */
		't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',/* Enter key */
		KEY_CONTROL,/* 29   - Control */
		'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',/* 39 */
		'\'', '`',   KEY_SHIFT,/* Left shift */
		'#', 'z', 'x', 'c', 'v', 'b', 'n',/* 49 */
		'm', ',', '.', '/',   KEY_SHIFT,/* Right shift */
		'*',
		KEY_ALT,/* Alt */
		' ',/* Space bar */
		KEY_CAPS,/* Caps lock */
		0,/* 59 - F1 key ... > */
		0,   0,   0,   0,   0,   0,   0,   0,
		0,/* < ... F10 */
		KEY_NUM,/* 69 - Num lock*/
		KEY_SCROLL,/* Scroll Lock */
		0,/* Home key */
		0,/* Up Arrow */
		0,/* Page Up */
		'-',
		0,/* Left Arrow */
		0,
		0,/* Right Arrow */
		'+',
		0,/* 79 - End key*/
		0,/* Down Arrow */
		0,/* Page Down */
		0,/* Insert Key */
		0,/* Delete Key */
		0,   0,   '\\',
		0,/* F11 Key */
		0,/* F12 Key */
		0,/* All other keys are undefined */
	};

unsigned char kbdus_shift[128] =
	{
		0,  27, '!', '"', '#', '$', '%', '^', '&', '*',/* 9 */
		'(', ')', '_', '+', '\b',/* Backspace */
		'\t',/* Tab */
		'Q', 'W', 'E', 'R',/* 19 */
		'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',/* Enter key */
		0,/* 29   - Control */
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',/* 39 */
		'@', '~',   0,/* Left shift */
		'~', 'Z', 'X', 'C', 'V', 'B', 'N',/* 49 */
		'M', '<', '>', '?',   0,/* Right shift */
		'*',
		0,/* Alt */
		' ',/* Space bar */
		0,/* Caps lock */
		0,/* 59 - F1 key ... > */
		0,   0,   0,   0,   0,   0,   0,   0,
		0,/* < ... F10 */
		0,/* 69 - Num lock*/
		0,/* Scroll Lock */
		0,/* Home key */
		0,/* Up Arrow */
		0,/* Page Up */
		'-',
		0,/* Left Arrow */
		0,
		0,/* Right Arrow */
		'+',
		0,/* 79 - End key*/
		0,/* Down Arrow */
		0,/* Page Down */
		0,/* Insert Key */
		0,/* Delete Key */
		0,   0,   '|',
		0,/* F11 Key */
		0,/* F12 Key */
		0,/* All other keys are undefined */
	};


#define KEYSTAT_SHIFT   0x1
#define KEYSTAT_ALT     0x2
#define KEYSTAT_CONTROL 0x4
#define KEYSTAT_NUM     0x8
#define KEYSTAT_SCROLL  0x10
#define KEYSTAT_CAPS    0x20
int keystatus = 0;

char kb_key = 0;

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r)
{
	unsigned char scancode;
	char key;
	
	/* Read from the keyboard's data buffer */
	scancode = inportb(0x60);

	/* If the top bit of the byte we read from the keyboard is
	 *  set, that means that a key has just been released */
	if (scancode & 0x80) {
			/* You can use this one to see if the user released the
			 *  shift, alt, or control keys... */
		scancode = scancode & 0x7F;
		key = kbdus[scancode];
		switch (key) {
		case KEY_SHIFT:
			keystatus = keystatus ^ KEYSTAT_SHIFT;
			break;
		case KEY_CONTROL:
			keystatus = keystatus ^ KEYSTAT_CONTROL;
			break;	  
		case KEY_ALT:
			keystatus = keystatus ^ KEYSTAT_ALT;
			break;
		case KEY_CAPS:
			keystatus = keystatus ^ KEYSTAT_CAPS;
			break;
		case KEY_NUM:
			keystatus = keystatus ^ KEYSTAT_NUM;
			break;
		case KEY_SCROLL:
			keystatus = keystatus ^ KEYSTAT_SCROLL;
			break;
		}	   
	} else {
			/* Here, a key was just pressed. Please note that if you
			 *  hold a key down, you will get repeated key press
			 *  interrupts. */

			/* Just to show you how this works, we simply translate
			 *  the keyboard scancode into an ASCII value, and then
			 *  display it to the screen. You can get creative and
			 *  use some flags to see if a shift is pressed and use a
			 *  different layout, or you can add another 128 entries
			 *  to the above layout to correspond to 'shift' being
			 *  held. If shift is held using the larger lookup table,
			 *  you would add 128 to the scancode when you look for it */
		
		key = kbdus[scancode];
		switch (key) {
		case KEY_SHIFT:
			keystatus = keystatus | KEYSTAT_SHIFT;
			break;
		case KEY_CONTROL:
			keystatus = keystatus | KEYSTAT_CONTROL;
			break;	  
		case KEY_ALT:
			keystatus = keystatus | KEYSTAT_ALT;
			break;
		case KEY_CAPS:
			keystatus = keystatus | KEYSTAT_CAPS;
			break;
		case KEY_NUM:
			keystatus = keystatus | KEYSTAT_NUM;
			break;
		case KEY_SCROLL:
			keystatus = keystatus | KEYSTAT_SCROLL;
			break;
		}	   
		
		if (keystatus & KEYSTAT_SHIFT) {
			key = kbdus_shift[scancode];
		} else {		
			key = kbdus[scancode];
		}

		/* putch(key); */
		kb_key = key;
	}
}

char getch () {
	char ret;
	while (kb_key == 0);
	ret = kb_key;
	kb_key = 0;
	return ret;
}

/* read a maximum of n-1 characters or until '\n' is encountered */
void gets (char *buffer, unsigned int n) {
	char key;
	char *p;
	unsigned int counter = 0;

	key = 0;
	p = buffer;
	do {
		/* get an input character */
		key = getch ();

		/* check for backspace */
		if (key == '\b') {
		  /* only allow backspacing if there is more than 1 char typed */
		  if (counter > 0) {
			putch ('\b');
			putch (' ');
			putch ('\b');
			p--;
			counter--;
		  }		  
		} else if (key == '\n') {
			*p = '\0';
			break;
		} else {
			if (counter < (n - 1)) {
				putch (key);
				*p = key;
				p++;
				counter++;
			}
		}
	} while (key != '\n');
	putch ('\n');
}

void keyboard_install () {

	irq_install_handler (1, keyboard_handler);

}
