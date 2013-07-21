
#include <stdio.h>
#include <stdlib.h>

#include "sys.h"
#include "value.h"
#include "gc.h"

void print_heap () {
	void *p;
	void *q;
	char c;
	printf ("heap: %d of %d (%d%%)\n",
			(int)(free_p - gc_working),
			gc_heap_size,
			(100*(int)(free_p - gc_working) / gc_heap_size));

	for (q = gc_working; q < free_p; q += 16) {
		printf ("%08x  ", (unsigned int)q);

		for (p = q; (p < q + 8); p++) {
			if (p < free_p) {
				printf ("%02x ", (unsigned char)(*(unsigned char *)p));
			} else {
				printf ("   ");
			}
		}
		printf (" ");
		for (p = q + 8; (p < q + 16); p++) {
			if (p < free_p) {
				printf ("%02x ", (unsigned char)(*(unsigned char *)p));
			} else {
				printf ("   ");
			}
		}
		printf (" |");
		for (p = q; (p < q + 16); p++) {
			if (p < free_p) {
				c = (char)(*(char *)p);
				if (c >= ' ' && c <= '~') {
					printf ("%c", c);
				} else {
					printf (".");
				}
			} else {
				break;
			}
		}
		printf ("|\n");		
	}
	printf ("%08x\n", (unsigned int)free_p);
}

#define HEAP_SIZE (1024*1024)

int main (int argc, char **argv) {
	void *h;
	value *v, *w, *x;
	void *tmp;
	
	h = calloc (HEAP_SIZE, sizeof(char));

	gc_init(h, HEAP_SIZE);
	
	v = gc_new ();
	v->data = (void *)(0x12345678);

	w = gc_new_string("abcde");	

	x = gc_new_cell();
	x = cons(v, w);
	x = acons("foo", w, x);
	
	v = gc_new_array(5);
	print_heap();
	
	gc_collect_init();
	gc_relocate(x);
	
	print_heap();

	gc_collect_init();
	print_heap();
	
	free(h);
}
