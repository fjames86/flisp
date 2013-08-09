
#include "flisp.h"

bool eq (void *val1, void *val2) {
	gc_type t1, t2;
	bool ret = FALSE;

	t1 = get_type(val1);
	t2 = get_type(val2);

	if (t1 == t2) {
		switch (t1) {
		case TYPE_INT:
			ret = (((type_int *)val1)->i == ((type_int *)val2)->i);
			break;
        case TYPE_SYMBOL:
            ret = (CAST(type_symbol *, val1)->sym == CAST(type_symbol *, val2)->sym);
            break;
		default:
			ret = (val1 == val2);
		}
	}
	return ret;
}

bool eql(void *val1, void *val2) {
	gc_type t1, t2;
	bool ret = FALSE;
	t1 = get_type(val1);
	t2 = get_type(val2);

	if (t1 == t2) {
		switch (t1) {
		case TYPE_INT:
			ret = (((type_int *)val1)->i == ((type_int *)val2)->i);
			break;
		case TYPE_DOUBLE:
			ret = (((type_double *)val1)->d == ((type_double *)val2)->d);
			break;
        case TYPE_SYMBOL:
            ret = (CAST(type_symbol *, val1)->sym == CAST(type_symbol *, val2)->sym);
            break;
		default:
			ret = eq(val1, val2);
		}
	}

	return ret;
}


bool equal(void *val1, void *val2) {
	gc_type t1, t2;
	bool ret = FALSE;

	t1 = get_type(val1);
	t2 = get_type(val2);

	if ((t1 == TYPE_INT || t1 == TYPE_DOUBLE) && (t2 == TYPE_INT || t2 == TYPE_DOUBLE)) {
		ret = number_equal(val1, val2);
    } else if (t1 == TYPE_SYMBOL && t2 == TYPE_SYMBOL) {
        ret = (CAST(type_symbol *, val1)->sym == CAST(type_symbol *, val2)->sym);
	} else if (t1 == TYPE_STRING && t2 == TYPE_STRING) {
		ret = strcmp(CAST(type_string *, val1)->str, CAST(type_string *, val2)->str);
	} else if (t1 == TYPE_ARRAY && t2 == TYPE_ARRAY) {
		ret = array_equal(val1, val2);
	} else if (t1 == TYPE_CELL && t2 == TYPE_CELL) {
		ret = cell_equal(val1, val2);
	} else {
		ret = eq(val1, val2);
	}
	return ret;
}



bool number_equal (void *val1, void *val2) {
	gc_type t1, t2;
	bool ret = FALSE;

	t1 = get_type(val1);
	t2 = get_type(val2);

	if (t1 == TYPE_INT && t2 == TYPE_INT) {
		ret = (CAST(type_int *, val1)->i == CAST(type_int *, val2)->i);
	} else if (t1 == TYPE_DOUBLE && t2 == TYPE_DOUBLE) {
		ret = (CAST(type_double *, val1)->d == CAST(type_double *, val2)->d);
	} else if (t1 == TYPE_INT && t2 == TYPE_DOUBLE) {
		ret = ((double)CAST(type_int *, val1)->i == CAST(type_double *, val2)->d);
	} else if (t2 == TYPE_INT && t1 == TYPE_DOUBLE) {
		ret = ((double)CAST(type_int *, val2)->i == CAST(type_double *, val1)->d);
	} else {
		ret = FALSE;
	}
	return ret;
}

bool cell_equal(void *val1, void *val2) {
	gc_type t1, t2;
	bool ret = FALSE;

	t1 = get_type(val1);
	t2 = get_type(val2);

	if (t1 == TYPE_NULL && t2 == TYPE_NULL) {
		ret = TRUE;
	} else if (t1 != TYPE_CELL || t2 != TYPE_CELL) {
		ret = FALSE;
	} else {
		ret = equal(CAST(type_cell *, val1)->car, CAST(type_cell *, val2)->car) &&
			equal(CAST(type_cell *, val1)->cdr, CAST(type_cell *, val2)->cdr);

	}
	return ret;
}

bool array_equal(void *val1, void *val2) {
	gc_type t1, t2;
	bool ret;
	type_array *a1, *a2;
	size_t i;

	t1 = get_type(val1);
	t2 = get_type(val2);

	if (t1 != TYPE_ARRAY || t2 != TYPE_ARRAY) {
		ret = FALSE;
	} else {
		a1 = CAST(type_array *, val1);
		a2 = CAST(type_array *, val2);

		ret = (a1->size == a2->size);

		for(i=0; i < a1->size; i++) {
			ret = ret && equal(a1->data[i], a2->data[i]);
			if (!ret) {
				break;
			}
		}
	}
	return ret;
}

double cast_to_double (void *val) {
	double d;
	gc_type t = get_type(val);
	
	switch(t) {
	case TYPE_INT:
		d = (double)CAST(type_int *, val)->i;
		break;
	case TYPE_DOUBLE:
		d = CAST(type_double *, val)->d;
		break;
	default:
		error ("Not a number", "CAST-TO-DOUBLE");
		d = 0.0;
	}
	return d;
}

void print_list (type_cell *l) {
    bool printed = FALSE;

    putch ('(');
    while (l != NULL) {
        if (get_type(l) != TYPE_CELL) {
            print_string (" . ");
            print_object (l);
            break;
        } else {
            if (printed) {
                putch (' ');
            }
            print_object (cell_car (l));
            printed = TRUE;
            l = l->cdr;
        }
    }
    putch (')');
}


void print_object (void *obj) {
	type_cell *c;
	size_t i;
	gc_type t = get_type (obj);

	switch (t) {
	case TYPE_NULL:
		print_string ("NIL");
		break;
	case TYPE_INT:
		print_int (CAST(type_int *, obj)->i);
		break;
	case TYPE_DOUBLE:
		print_double (CAST(type_double *, obj)->d, 5);
		break;
	case TYPE_SYMBOL:
		print_string (CAST(type_symbol *, obj)->sym);
		break;
	case TYPE_STRING:
		putch ('"');
		print_string (CAST(type_string *, obj)->str);
		putch ('"');
		break;
	case TYPE_HT:
		print_string ("#<HASH-TABLE ");
		print_hex ((size_t)obj);
		print_string (" :SIZE ");
		print_int (CAST(type_ht *, obj)->size);
		print_string (" :FILL ");
		print_int (CAST(type_ht *, obj)->fill);
		print_string (">");
		break;
	case TYPE_CLOSURE:
		print_string ("#<CLOSURE ");
		print_int ((size_t)obj);
		print_string (">");
		break;
	case TYPE_PROC:
		print_string ("#<PROC ");
		print_hex ((size_t)obj);
		print_string (">");
		break;
	case TYPE_ARRAY:
		print_string ("#(");
		for (i=0; i < CAST(type_array *, obj)->size; i++) {
			if (i > 0) {
				print_string (" ");
			}
			print_object (CAST(type_array *, obj)->data[i]);
		}
		print_string (")");
		break;
	case TYPE_CELL:
        print_list (obj);
        break;
    default:
      print_string ("#<UNKNOWN "); print_int (t); print_string (">");
        error ("Unknown type", "PRINT");
	}
}

void print_object_nice (void *obj) {
	type_cell *c;
	size_t i;
	bool printed;
	gc_type t = get_type (obj);

	switch (t) {
	case TYPE_NULL:
		print_string ("NIL");
		break;
	case TYPE_INT:
		print_int (CAST(type_int *, obj)->i);
		break;
	case TYPE_DOUBLE:
		print_double (CAST(type_double *, obj)->d, 5);
		break;
	case TYPE_SYMBOL:
		print_string (CAST(type_symbol *, obj)->sym);
		break;
	case TYPE_STRING:
		print_string (CAST(type_string *, obj)->str);
		break;
	case TYPE_HT:
		print_string ("#<HASH-TABLE ");
		print_hex ((size_t)obj);
		print_string (">");
		break;
	case TYPE_CLOSURE:
		print_string ("#<CLOSURE ");
		print_hex ((size_t)obj);
		print_string (">");
		break;
	case TYPE_PROC:
		print_string ("#<PROC ");
		print_hex ((size_t)obj);
		print_string (">");
		break;
	case TYPE_ARRAY:
		print_string ("#(");
		for (i=0; i < CAST(type_array *, obj)->size; i++) {
			if (i > 0) {
				print_string (" ");
			}
			print_object (CAST(type_array *, obj)->data[i]);
		}
		print_string (")");
		break;
	case TYPE_CELL:
        print_list (obj);
        break;
    default:
      print_string ("#<UNKNOWN "); print_int (t); print_string (">");
      error ("Unknown type", "PRINC");
	}
}

/*
 * formated output
 * ~S print readable object
 * ~A print human readable objet
 * ~% newlie
 * ~~ ~ char
 * ~<other> error?
 */
void format (type_string *string, type_cell *args) {
  if (string && string->str) {
    format_ (string->str, args);
  }
}

void format_ (char *fstr, type_cell *args) {
	while (*fstr != '\0') {
		if (*fstr == '~') {
			/* dispatch char */
			fstr++;
			switch (*fstr) {
			case '%':
				putch ('\n');
				break;
			case '~':
				putch ('~');
				break;
			case 'S':
			case 's':
				print_object (cell_car(args));
				args = cell_cdr(args);
				break;
			case 'A':
			case 'a':
				print_object_nice (cell_car(args));
			    args = cell_cdr(args);
				break;
			default:
				error ("Unknown format dispatch char", "FORMAT");
			}
		} else {
			putch (*fstr);
		}

		fstr++;
	}
}


void flisp_repl (bool quit) {
	type_cell *errs;
	void *expr;

	memset (reader_buffer, '\0', MAX_LINE);
	reader_bufferp = reader_buffer;
	while (TRUE) {
		print_string ("\n> ");
		expr = next_expr();

		/* if a list and car eq to 'QUIT then leave */		
		if (quit && 
		    get_type(expr) == TYPE_CELL &&
		    get_type(cell_car(expr)) == TYPE_SYMBOL &&
		    eq(cell_car(expr), intern("QUIT"))) {
		  break;
		}
		
		error_clear();
		expr = eval(expr, &toplevel);
		print_errors ();

		error_clear ();
		print_object (expr);
		print_errors ();

		gc_collect_init();
		gc_collect ((void **)&(toplevel.special));
		gc_collect ((void **)&(toplevel.lexical));
		
	}
}


/* load code from buffer */

void load_core () {	
	void (*rbf)();
	
	/* need to temporarily rebind refresh_buffer_f to our new function */
	rbf = refresh_buffer_f;
	refresh_buffer_f = &rebuffer_core;

	rebuffer_corep = flisp_core_source;
	while (rebuffer_corep != NULL) {
		eval(next_expr(), &toplevel);
	}	

	refresh_buffer_f = rbf;
	(*refresh_buffer_f)();
}

void rebuffer_core () {
	int i = 0;

	reader_bufferp = reader_buffer;
	while (*rebuffer_corep != '\0' && *rebuffer_corep != '\n' && i < MAX_LINE-1) {
		*reader_bufferp = *rebuffer_corep;
		rebuffer_corep++;
		reader_bufferp++;
		i++;
	}
	*reader_bufferp = '\0';
	
	reader_bufferp = reader_buffer;

	/* signal the end has been reached */
	if (*rebuffer_corep == '\0') {
		rebuffer_corep = NULL;
	}
}
