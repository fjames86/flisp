
#include <stdio.h>
#include <stdlib.h>

#include "sys.h"
#include "gc.h"

void print_heap();
void print_val(void *val);
bool whitespace(char c);
bool digitp (char c);
bool integerp (char *str);
int parse_integer (char *str);
void refresh_buffer();
void next_word (char *dest);
type_cell *read_list ();
void *next_expr();


#define MAX_LINE 100
char buffer[MAX_LINE];
char *bufferp;

void refresh_buffer() {
	gets(buffer);
	bufferp = buffer;
}

void next_word (char *dest) {
	bool done;
	
	/* refresh the buffer if needed */
	while (*bufferp == '\0') {
		refresh_buffer();
	}

	while (whitespace(*bufferp)) {
		bufferp++;
	}

	if (*bufferp == '\0') {
		return next_word(dest);
	}
	
	done = FALSE;
	while (TRUE) {
		if (whitespace(*bufferp) || *bufferp == '\0') {
			break;
		} else if (*bufferp == '(' || *bufferp == ')' || *bufferp == '.') {
			if (!done) {
				*dest = *bufferp;
				dest++;
				bufferp++;
			}
			break;
		}

		*dest = *bufferp;
		done = TRUE;
		dest++;
		bufferp++;
	}
	*dest = '\0';
}

type_cell *read_list () {
	char word[MAX_LINE];
	type_cell **builder;
	bool first = TRUE;
	type_cell *top;
	
	top = NULL;
	builder = &top;
	while (TRUE) {
		next_word(word);

		if (strcmp (word, "(") == 0) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
			
			(*builder)->car = read_list();
			builder = (type_cell **)(&((*builder)->cdr));
		} else if (strcmp (word, ")") == 0) {
			*builder = NULL;
			break;
		} else if (strcmp (word, ".") == 0) {
			/* dotted list */
			if (first == TRUE) {
				/* error, dotted list not at end of list */
				printf ("Error: dotted list not at end of list.\n");
				top = NULL;
				break;
			} else {
				*builder = next_expr();
				next_word(word);
				if (strcmp(word, ")") != 0) {
					printf("Error: dotted list not at end of list\n");
					top = NULL;
				}
				break;
			}
		} else if (integerp (word) == TRUE) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
			(*builder)->car = gc_new_int (parse_integer (word));
			builder = (type_cell **)(&((*builder)->cdr));
		} else {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
			(*builder)->car = gc_new_string (word);
			builder = (type_cell **)(&((*builder)->cdr));
		}
	}
	
	return top;
}

void *next_expr() {
	char word[MAX_LINE];
	void *ret;
	type_cell *builder;
	void *next;
	
	next_word(word);
	
	if (strcmp(word, "(") == 0) {
		/* start of a list */
		ret = read_list();
	} else if (strcmp (word, ")") == 0) {
		/* end of a list. shoulnd't happen here */
		printf ("Error: unmatched closing paren\n");
		ret = NULL;
	} else if (strcmp (word, ".") == 0) {
		/* dotted end of list. shouldn't happen here */
		printf ("Error: dotted list not in list \n");
		return NULL;
	} else if (integerp (word) == TRUE) {
		/* integer */
		ret = (void *)gc_new_int (parse_integer (word));
	} else {
		/* string */
		ret = (void *)gc_new_string (word);
	}
	
	return ret;
}

#define HEAP_SIZE (1024*1024)

int main (int argc, char **argv) {
	void *heap;
	type_int *i;
	type_cell *c;
	type_string *s;
	char word[MAX_LINE];
	void *expr;

	/* create the heap */
	heap = calloc (HEAP_SIZE, sizeof(char));
	gc_init(heap, HEAP_SIZE);

	bufferp = "";

	while (TRUE) {		
		printf ("> ");
		expr = next_expr();
		print_val(expr);
		printf ("\n");
	}

	
#if 0
	i = gc_new_int (123);
	s = gc_new_string("hello");
	c = gc_new_cell();
	c->car = i;
	c->cdr = gc_new_cell();
	((type_cell *)c->cdr)->car = s;
	((type_cell *)c->cdr)->cdr = s;

	print_heap();
	
	print_val (c);
	printf ("\n");
	
	print_heap();

	gc_collect_init();
	gc_collect ((void *)&c);
	print_heap();
#endif
	
	free(heap);
}





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
					printf (" .");
				}
			} else {
				break;
			}
		}
		printf ("|\n");		
	}
	printf ("%08x\n", (unsigned int)free_p);
}

void print_val (void *val) {
	gc_type type;
	type_cell *c;
	bool printed;
	
	if (val == NULL) {
		printf ("NIL");
		return;
	}
	
	type = ((gc_tag *)val)->type;

	switch (type) {
	case TYPE_INT:
		printf("%d", ((type_int *)val)->i);
		break;
	case TYPE_STRING:
		printf("\"%s\"", ((type_string *)val)->str);
		break;
	case TYPE_CELL:
		c = (type_cell *)val;
		printf ("(");
		printed = FALSE;
		
		while (c != NULL) {
			if (printed) {
				printf (" ");
			} else {
				printed = TRUE;
			}
			print_val (c->car);
			
			c = c->cdr;
			/* check for dotted list */
			if ((c != NULL) && (((gc_tag *)c)->type != TYPE_CELL)) {
				printf (" . ");
				print_val (c);
				break;
			}			
		}

		printf (")");
		break;
	}
}

bool whitespace(char c) {
	switch (c) {
	case ' ':
	case '\t':
	case '\f':
	case '\n':
	case '\v':
		return TRUE;
	default:
		return FALSE;
	}
}

bool digitp (char c) {
	if (c >= '0' && c <= '9') {
		return TRUE;
	} else {
		return FALSE;
	}
}
	
bool integerp (char *str) {
	char *c = str;
	int d = 0;

	/* first char may be a +, - or of course a digit */
	if(!(*c == '+' || *c == '-' || digitp(*c))) return 0;

	if (isdigit(*c)) d = 1;
	
	c++;
	while(*c != '\0') {
		if(!isdigit(*c)) return 0;
		else d = 1;
		c++;
	}

	return d;
}

int parse_integer (char *str) {
	char *c;
	int ret, factor;

	c = str;
	while (*c != '\0') c++;
	c--;

	factor = 1;
	ret = 0;
	while (c >= str) {
		if (*c == '-') {
			ret = -ret;
			break;
		} else if (*c == '+') {
			break;
		}
		
		ret += (*c - '0')*factor;
		factor *= 10;
		c--;
	}

	return ret;
}
