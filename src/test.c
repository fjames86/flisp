
#include <stdio.h>
#include <stdlib.h>

#include "sys.h"
#include "gc.h"
#include "lists.h"
#include "ht.h"
#include "env.h"
#include "eval.h"

#define __FLISP_VERSION__ "0.1"


void print_heap(size_t topbytes);
void print_val(void *val);
bool whitespace(char c);
bool digitp (char c);
bool integerp (char *str);
bool doublep (char *str);
int parse_integer (char *str);
double parse_double (char *str);
void refresh_buffer();
void next_word (char *dest);
type_cell *read_list ();
void *next_expr();
type_string *read_string();
void save_image (char *fname);
void load_image (char *fname);
void load_file (char *fname);

#define MAX_LINE 100
FILE *readfile;
char buffer[MAX_LINE];
char *bufferp;

#define HEAP_SIZE (1024*1024)
#define SYMTAB_SIZE (1024*1024)

environment toplevel;

int main (int argc, char **argv) {
	void *heap;
	type_int *i;
	type_cell *c;
	type_string *s;
	char word[MAX_LINE];
	void *expr;
	char *strtab;
	type_ht *ht;
	type_array *arr;
	char *symt;

	printf("Welcome to flisp " __FLISP_VERSION__ " Copyright Frank James " __DATE__ "\n");
	
	/* create the heap */
	heap = calloc (HEAP_SIZE, sizeof(char));
	gc_init(heap, HEAP_SIZE);
	
	bufferp = "";
	readfile = stdin;
	
	symt = calloc (SYMTAB_SIZE, sizeof(char));
	symbol_init (symt, SYMTAB_SIZE);

    env_init(&toplevel);

	/*
	save_image ("flisp.core.bin");
	*/
	
	while (TRUE) {
		printf ("\n> ");
		expr = next_expr();
        print_val(eval(expr, &toplevel)); 

		gc_collect_init();
        gc_collect ((void **)&(toplevel.special));
        gc_collect ((void **)&(toplevel.lexical));
	}

	free(heap);
    return 0;
}


void print_heap (size_t topbytes) {
	void *p, *q;
	char c;
	printf ("heap: %d of %d (%d%%)\n",
			(int)(free_p - gc_working),
			gc_heap_size,
			(100*(int)(free_p - gc_working) / gc_heap_size));

	for (q = max(free_p - topbytes, gc_working); q < free_p; q += 16) {
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

void print_val (void *val) {
	gc_type type;
	type_cell *c;
	bool printed;
	size_t i;

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

        /* first check it's a quote, if not then print a list */
        if (eq (cell_car(c), intern("QUOTE"))) {
            printf("'"); 
            print_val(cell_cadr(c));
            break;
        }

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
	case TYPE_SYMBOL:
		printf ("%s", ((type_symbol *)val)->sym);
		break;
	case TYPE_DOUBLE:
		printf ("%1lf", ((type_double *)val)->d);
		break;
    case TYPE_HT:
		printf("#<HT %p :FILL %d>", val, ((type_ht *)val)->fill);
		break;
	case TYPE_ARRAY:
		printf ("#(");
		for(i=0; i < ((type_array *)val)->size; i++) {
			if (i > 0) printf (" ");
			print_val(((type_array *)val)->data[i]);
		}
		printf (")");		
		break;
    case TYPE_PROC:
        printf("#<PROC %p>", CAST(type_proc *, val)->proc);
        break;
	case TYPE_CLOSURE:
		printf("#<CLOSURE %p>", val);
		break;
	default:
		printf("#<UNKNOWN %d>", type);		
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

double parse_double (char* str){
	double res = 0.0, factor = 1.0;
	int point_seen, d, exp;
	
 	if (*str == '-') {
		str++;
		factor = -1.0;
	}
	
	for (point_seen = 0; *str; str++) {
		if (*str == '.') {
			point_seen = 1;
			continue;
		} else if (*str == 'e' || *str == 'E') {
			str++;
			exp = parse_integer(str);
			if (exp > 0) {
				for(d = 0; d < exp; d++) {
					factor *= 10.0;
				}				
			} else {
				for (d=0; d < -exp; d++) {
					factor /= 10.0;
				}
			}
			break;
		}
		
		d = *str - '0';
		if (d >= 0 && d <= 9){
			if (point_seen) {
				factor /= 10.0;
			}
			res = res * 10.0 + (double)d;
		}
	}
	
	return res * factor;
}

bool doublep (char *str) {
	char *c = str;
	bool point = FALSE;
	bool exp = FALSE;
	
	while (*c == ' ') c++;

	if(!(*c == '+' || *c == '-' || *c == '.' || digitp(*c))) return FALSE;

	if(*c == '.') point = TRUE;
	
	c++;
	while(*c != '\0') {
		/* point must occur before exp */
		if(*c == '.') {
			if(exp == 0) point = TRUE;
			else return FALSE;
		} else if(*c == 'e' || *c == 'E') {
			if(exp == FALSE) {
				c++;
				return integerp (c);
			} else {
				return FALSE;
			}
		} else if (!digitp(*c)) {
			return FALSE;
		} 
		c++;
	}
	if(point == TRUE)
		return TRUE;
	else
		return FALSE;
}


void refresh_buffer() {
	/*gets(buffer);*/
	if (feof(readfile) != 0) {
		printf("end of file reached. changing to reading from stdin\n");
		fclose(readfile);
		readfile = stdin;
	}
	fgets(buffer, MAX_LINE, readfile);
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

	if (*bufferp == '\0' || *bufferp == '\n') {
		return next_word(dest);
	}
	
	done = FALSE;
	while (TRUE) {
		if (whitespace(*bufferp) || *bufferp == '\0') {
			break;
		} else if (*bufferp == '(' || *bufferp == ')' || *bufferp == '"' || *bufferp == '\'') {
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
	type_cell *top, *cell;
	
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
        } else if (strcmp (word, "'") == 0) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
            cell = cons(next_expr(), NULL);
            (*builder)->car = cons(intern("QUOTE"), cell);
			builder = (type_cell **)(&((*builder)->cdr));
		} else if (strcmp (word, "\"") == 0) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
			(*builder)->car = read_string();
			builder = (type_cell **)(&((*builder)->cdr));
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
		} else if (doublep (word) == TRUE) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
			(*builder)->car = gc_new_double (parse_double (word));
			builder = (type_cell **)(&((*builder)->cdr));
		} else {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
			string_upcase(word);
			(*builder)->car = intern (word);
			builder = (type_cell **)(&((*builder)->cdr));
		}
	}
	
	return top;
}

/* read until an unescaped " char is encountered. */
type_string *read_string () {
	char *strbuff1, *strbuff2;
	size_t i;
	size_t buffsize = MAX_LINE;
	type_string *ret;
	bool escape;
	
	strbuff1 = gc_malloc(buffsize);
	i = 0;
	escape = FALSE;
	while (TRUE) {
		if (escape == TRUE) {
			switch (*bufferp) {
			case '\0':
				strbuff1[i] = '\n';
				/* this is the end of line, so refresh */
				refresh_buffer();
			    break;
			case 'n':
				strbuff1[i] = '\n';
				bufferp++;
				break;
			case 't':
				strbuff1[i] = '\t';
				bufferp++;
				break;
			case 'f':
				strbuff1[i] = '\f';
				bufferp++;
				break;
			case 'r':
				strbuff1[i] = '\r';
				bufferp++;
				break;
			case 'v':
				strbuff1[i] = '\v';
				bufferp++;
				break;
			case 'b':
				strbuff1[i] = '\b';
				bufferp++;
				break;
			case '\\':
				strbuff1[i] = '\\';
				bufferp++;
				break;
			case '"':
				strbuff1[i] = '"';
				bufferp++;
				break;
			default:
				/* not an official escape char, so just ignore it */
				strbuff1[i] = *bufferp;
				bufferp++;
			}
			i++;
			escape = FALSE;
		} else if (*bufferp == '\0') {
			/* end of line without an escape, error */
			ret = NULL;
			printf("Error: unterminated string detected.\n");
			bufferp++;
			break;
		} else if (*bufferp == '"') {
			strbuff1[i] = '\0';
			bufferp++;			
			break;
		} else if (*bufferp == '\\') {
			escape = TRUE;
			bufferp++;
		} else {
			/* put the char into the buffer */
			strbuff1[i] = *bufferp;
			bufferp++;
			i++;

			escape = FALSE;
		}
						
		/* need to increase the buffer and copy across */
		if (i >= buffsize) {
			buffsize += MAX_LINE;
			strbuff2 = gc_malloc(buffsize);
			strcpy(strbuff2, strbuff1);
			strbuff1 = strbuff2;
		}
	}

	ret = gc_new_string(strbuff1);
	return ret;
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
    } else if (strcmp (word, "'") == 0) {
        /* quote read macro */
        builder = cons(next_expr(), NULL);
        builder = cons(intern("QUOTE"), builder);
        ret = builder;
	} else if (strcmp (word, "\"") == 0) {
		/* quote, read string */
		ret = read_string();
	} else if (integerp (word) == TRUE) {
		/* integer */
		ret = (void *)gc_new_int (parse_integer (word));
	} else if (doublep (word) == TRUE) {
		/* double */
		ret = (void *)gc_new_double (parse_double (word));
	} else {
		/* if all else fails, intern a ssyobl */
		string_upcase(word);		
		ret = (void *)intern(word);
	}

	printf("Read in "); print_val (ret); printf("\n");
	return ret;
}

/*
 * write all information to a file
 * first write the symbol table and then the contents of the heap
 */
void save_image (char *fname) {
	FILE *f;
	size_t size, i;
	char *p;
	
	f = fopen(fname, "wb");
	if (f != NULL) {
		if (fseek(f, 0, SEEK_SET) == 0) {

			/* write the number of strings */
			i = symbol_table - symbol_p;
			fwrite (&i, sizeof(size_t), 1, f);
			
			/* first write the strings */
			fwrite ((void *)string_table, sizeof(char), (string_table_p - string_table), f);

			/* now print the heap */
			fwrite ((void *)gc_working, sizeof(char), gc_heap_size / 2, f);
		}
		
		fclose(f);
	}
}


void load_image (char *fname) {
	size_t n, i;
	FILE *f;
	char buffer[MAX_LINE];
	
	f = fopen (fname, "rb");
	if (f != NULL) {

		/* first intern the symbols. get the number of strings and then read them */		
		fread (&n, sizeof(size_t), 1, f);
		for(i = 0; i < n; i++) {
			fscanf(f, "%s", buffer);
			intern(buffer);
		}

		/* now load the heap */
		
		fclose(f);
	}
}

void load_file(char *fname) {
	FILE *f, *tmp;
	void *expr;
	
	f = fopen(fname, "r");
	if (f != NULL) {
		tmp = readfile;
		readfile = f;

		do {
			expr = next_expr();
			if (readfile == stdin) {
				/* must've hit the end of the file */
				break;
			}
			eval(expr, &toplevel);			
		} while (expr != NULL);		

		readfile = tmp;
	}
}
