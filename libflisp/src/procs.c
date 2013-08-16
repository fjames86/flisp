
/*
 * Copyright (C) 2013 Frank James
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */


#include "procs.h"

/* (car cell) */
void *proc_car (type_cell *args) {
	void *c;

	c = cell_car(args);
	if (get_type(c) == TYPE_CELL) {
		c = cell_car(c);
	} else {
		error ("Not a cons cell", "CAR");
		c = NULL;
	}
	return c;
}

/* (car cell) */
void *proc_cdr (type_cell *args) {
	void *c;

	c = cell_car(args);
	if (get_type(c) == TYPE_CELL) {
		c = cell_cdr(c);
	} else {
		error ("Not a cons cell", "CDR");
		c = NULL;
	}
	return c;
}

/* (set-car! cell vak) */
void *proc_set_car (type_cell *args) {
	void *c, *val;

	c = cell_car(args);
	if (get_type(c) == TYPE_CELL) {
		val = cell_cadr(args);
		set_car(c, val);
	} 
	return NULL;
}

/* (car cell) */
void *proc_set_cdr (type_cell *args) {
	void *c, *val;

	c = cell_car(args);
	if (get_type(c) == TYPE_CELL) {
		val = cell_cadr(args);
		set_cdr(c, val);
	} 
	return NULL;
}

/* (cons car cdr) */
void *proc_cons (type_cell *args) {
	void *car, *cdr, *ret;
	car = cell_car(args);
	cdr = cell_cadr(args);
	return cons(car, cdr);
}

void *proc_add (type_cell *args) {
    int i, iret = 0;
    double d, dret = 0;
    gc_type t = TYPE_INT;
    type_cell *c;

    while (args != NULL) {
        c = args->car;
    
        switch (get_type(c)) {
        case TYPE_INT:
            i = CAST(type_int *, c)->i;
            if (t == TYPE_INT) {
                iret += i;
            } else {
                dret += (double)i;
            }
            break;
        case TYPE_DOUBLE:
            d = CAST(type_double *, c)->d;
            if (t == TYPE_INT) {
                t = TYPE_DOUBLE;
                dret = (double)iret;
                dret += d;
            } else {
                dret += d;
            }
            break;
        default:
            /* error? */
			error ("Not a number", "+");
			return NULL;
            i = 0;
        }
          
        args = args->cdr;
    }

    if (t == TYPE_INT) {
        return (void *)gc_new_int (iret);
    } else if (t == TYPE_DOUBLE) {
        return (void *)gc_new_double (dret);
    } else {
		return NULL;
    }
}

void *proc_mul (type_cell *args) {
    int i, iret = 1;
    double d, dret = 1;
    gc_type t = TYPE_INT;
    type_cell *c;

    t = TYPE_INT;    
    while (args != NULL) {
        c = args->car;
    
        switch (get_type(c)) {
        case TYPE_INT:
            i = CAST(type_int *, c)->i;
            if (t == TYPE_INT) {
                iret *= i;
            } else if (t == TYPE_DOUBLE) {
                dret *= (double)i;
            }
            break;
        case TYPE_DOUBLE:
            d = (double)CAST(type_double *, c)->d;
            if (t == TYPE_INT) {
                t = TYPE_DOUBLE;
                dret = (double)iret;
                dret *= d;
            } else if (t == TYPE_DOUBLE) {
                dret *= d;
            }
            break;
        default:
            /* error? */
			error ("Not a number", "*");
			return NULL;
            i = 1;
        }
          
        args = args->cdr;
    }

    if (t == TYPE_INT) {
        return (void *)gc_new_int (iret);
    } else {
        return (void *)gc_new_double (dret);
    }  
}

void *proc_sub (type_cell *args) {
	int i1, i2;
	double d1, d2;
	gc_type t1, t2;
	type_cell *c;
	void *subs;

	i1 = 0;
	d1 = 0.0;

	c = CAST(type_cell *, cell_car(args));
	t1 = get_type(c);
	if (t1 == TYPE_INT) {
		i1 = CAST(type_int *, c)->i;
	} else if (t1 == TYPE_DOUBLE) {
		d1 = CAST(type_double *, c)->d;
	}

	args = cell_cdr(args);
	if (args == NULL) {
		i2 = -i1;
		d2 = -d1;
	} else {
		subs = proc_add((type_cell *)args);
		t2 = get_type(subs);
		if (t2 == TYPE_INT) {
			if (t1 == TYPE_INT) {
				i2 = i1 - CAST(type_int *, subs)->i;
			} else if (t1 == TYPE_DOUBLE) {
				d2 = d1 - (double)CAST(type_int *, subs)->i;
			}      
		} else if (t2 == TYPE_DOUBLE) {
			if (t1 == TYPE_INT) {
				d2 = (double)i1 - CAST(type_double *, subs)->d;
				t1 = TYPE_DOUBLE;
			} else if (t1 == TYPE_DOUBLE) {
				d2 = d1 - CAST(type_double *, subs)->d;
			}
		} else {
			error ("Not a number", "-");
			return NULL;
		}
	}

	if (t1 == TYPE_INT) {
		return gc_new_int (i2);
	} else if (t1 == TYPE_DOUBLE){
		return gc_new_double (d2);
	} else {
		return NULL;
	}
}

void *proc_div (type_cell *args) {
	int i1, i2;
	double d1, d2;
	gc_type t1, t2;
	type_cell *c;
	void *subs;

	i1 = 0;
	d1 = 0.0;

	c = CAST(type_cell *, cell_car(args));
	t1 = get_type(c);
	if (t1 == TYPE_INT) {
		i1 = CAST(type_int *, c)->i;
	} else if (t1 == TYPE_DOUBLE) {
		d1 = CAST(type_double *, c)->d;
	}

	args = cell_cdr(args);
	if (args == NULL) {
		if (t1 == TYPE_INT) {
			if (i1 == 0) {
				error ("Division by zero", "PROC_DIV");
				i2 = 0;
			} else {
				i2 = 1 / i1;
			}
		} else {
			if (d1 == 0.0) {
				error ("Division by zero", "PROC_DIV");
				d2 = 0.0;
			} else {
				d2 = 1.0 / d1;
			}
		}
	} else {
		subs = proc_add((type_cell *)args);
		t2 = get_type(subs);
		if (t2 == TYPE_INT) {
			if (CAST(type_int *, subs)->i == 0) {
				error ("Division by zero", "PROC_DIV");
				i2 = 0;
				d2 = 0.0;
			} else {
				if (t1 == TYPE_INT) {
					i2 = i1 / CAST(type_int *, subs)->i;
				} else if (t1 == TYPE_DOUBLE) {
				d2 = d1 / (double)CAST(type_int *, subs)->i;
				}
			}
		} else if (t2 == TYPE_DOUBLE) {
			if (CAST(type_double *, subs)->d == 0.0) {
				error ("Division by zero", "PROC_DIV");
				i2 = 0;
				d2 = 0.0;
			} else {
				if (t1 == TYPE_INT) {
					d2 = (double)i1 / CAST(type_double *, subs)->d;
					t1 = TYPE_DOUBLE;
				} else if (t1 == TYPE_DOUBLE) {
					d2 = d1 / CAST(type_double *, subs)->d;
				}
			}
		} else {
			error ("Not a number", "/");
			return NULL;
		}
	}

	if (t1 == TYPE_INT) {
		return gc_new_int (i2);
	} else if (t1 == TYPE_DOUBLE){
		return gc_new_double (d2);
	} else {
		return NULL;
	}
}

/* (make-array size) */
void *proc_make_array (type_cell *args) {
	size_t size;
	type_array *a;
	gc_type t;
	
	args = cell_car(args);
	t = get_type(args);
	if (t == TYPE_INT) {
		size = CAST(type_int *, args)->i;
		a = gc_new_array (size);
	} else {
		error ("Size not an integer", "MAKE-ARRAY");
		a = NULL;
	}
	return a;
}

/* (aref array index) */
void *proc_aref (type_cell *args) {
	type_array *a;
	size_t i;
	void *x, *ret;
	
	x = cell_car(args);
	args = cell_cdr(args);
	if (get_type(x) == TYPE_ARRAY) {
		a = CAST(type_array *, x);
			
		x = cell_car(args);
		if (get_type (x) == TYPE_INT) {
			i = CAST(type_int *, x)->i;

			ret = aref(a, i);
		} else {
			error ("Index not an integer", "AREF");			
			ret = NULL;
		}
	} else {
		error ("Not an array", "AREF");
		ret = NULL;
	}
	return ret;
}

/* (set-aref! array index val) */
void *proc_set_aref (type_cell *args) {
	type_array *a;
	size_t i;
	void *x;
	
	x = cell_car(args);
	args = cell_cdr(args);
	if (get_type(x) == TYPE_ARRAY) {
		a = CAST(type_array *, x);
			
		x = cell_car(args);
		args = cell_cdr(args);
		if (get_type (x) == TYPE_INT) {
			i = CAST(type_int *, x)->i;

			x = cell_car(args);
			set_aref(a, i, x);
		} else {
			error ("Index not an integer", "SET-AREF!");
		}
	} else {
		error ("Not an array", "SET-AREF!");
	}
	return NULL;
}

void *proc_list (type_cell *args) {
	type_cell *ret, **c;

	ret = NULL;
	c = &ret;
	while (args != NULL) {
		*c = cons(args->car, NULL);
		c = (type_cell **)&((*c)->cdr);
		args = args->cdr;
	}

	return ret;
}

void *proc_reverse (type_cell *args) {
	type_cell *ret, **c;
	
	args = cell_car(args);
	ret = NULL;
	c = &ret;
	while (args != NULL) {
		*c = cons(args->car, *c);
		args = args->cdr;
	}
	return ret;
}

void *proc_length (type_cell *args) {
	void *ret, *list;

	list = cell_car (args);
	switch (get_type (list)) {
	case TYPE_CELL:
	case TYPE_NULL:
		ret = gc_new_int (list_length (CAST(type_cell *, list)));
		break;
	case TYPE_ARRAY:
		ret = gc_new_int (CAST(type_array *, list)->size);
		break;
	default:
		error ("Length expects a list or array", "LENGTH");
		ret = NULL;
	}
	return ret;
}


/* apply a proc to a list of arguments, e.g. (apply + 1 2 (list 3 4)) -> 10. the final arg MUST be a list or NULL */
void *proc_apply (type_cell *args) {
	void *proc;

	proc = cell_car(args);
	args = append (cell_cdr(args));

	return apply(proc, args);
}

void *proc_append (type_cell *args) {
  return append (args);
}

/* (load filename) */
void *proc_load (type_cell *args) {
	void *fname;

	fname = cell_car(args);
	if (get_type(fname) == TYPE_STRING) {
		load_file(CAST(type_string *, fname)->str);
	}
	return NULL;
}

void *proc_eq (type_cell *args) {
	void *ret, *c;

	ret = cell_car(args);
	args = cell_cdr(args);
	while (args != NULL) {
		c = cell_car(args);
		if (eq(ret, c)) {
			ret = c;
			args = args->cdr;
		} else {
			ret = NULL;
			break;
		}
	}

	return ret;
}
							 
void *proc_eql (type_cell *args) {
	void *ret, *c;

	ret = cell_car(args);
	args = cell_cdr(args);
	while (args != NULL) {
		c = cell_car(args);
		if (eql(ret, c)) {
			ret = c;
			args = args->cdr;
		} else {
			ret = NULL;
			break;
		}
	}

	return ret;
}

void *proc_equal (type_cell *args) {
	void *ret, *c;

	ret = cell_car(args);
	args = cell_cdr(args);
	while (args != NULL) {
		c = cell_car(args);
		if (equal(ret, c)) {
			ret = c;
			args = args->cdr;
		} else {
			ret = NULL;
			break;
		}
	}

	return ret;
}

void *proc_gt (type_cell *args) {
	void *ret, *c;

	ret = cell_car(args);
	args = cell_cdr(args);
	while (args != NULL) {
		c = cell_car(args);
		if (cast_to_double (ret) > cast_to_double (c)) {
			ret = c;
			args = args->cdr;
		} else {
			ret = NULL;
			break;
		}
	}

	return ret;
}

void *proc_lt (type_cell *args) {
	void *ret, *c;

	ret = cell_car(args);
	args = cell_cdr(args);
	while (args != NULL) {
		c = cell_car(args);
		if (cast_to_double (ret) < cast_to_double (c)) {
			ret = c;
			args = args->cdr;
		} else {
			ret = NULL;
			break;
		}
	}

	return ret;
}

void *proc_gte (type_cell *args) {
	void *ret, *c;

	ret = cell_car(args);
	args = cell_cdr(args);
	while (args != NULL) {
		c = cell_car(args);
		if (cast_to_double (ret) >= cast_to_double (c)) {
			ret = c;
			args = args->cdr;
		} else {
			ret = NULL;
			break;
		}
	}

	return ret;
}

void *proc_lte (type_cell *args) {
	void *ret, *c;

	ret = cell_car(args);
	args = cell_cdr(args);
	while (args != NULL) {
		c = cell_car(args);
		if (cast_to_double (ret) <= cast_to_double (c)) {
			ret = c;
			args = args->cdr;
		} else {
			ret = NULL;
			break;
		}
	}

	return ret;
}


void *proc_e (type_cell *args) {
	void *ret, *c;

	ret = cell_car(args);
	args = cell_cdr(args);
	while (args != NULL) {
		c = cell_car(args);
		if (cast_to_double (ret) == cast_to_double (c)) {
			ret = c;
			args = args->cdr;
		} else {
			ret = NULL;
			break;
		}
	}

	return ret;
}

void *proc_macroexpand (type_cell *args) {
	type_cell *expr;

	expr = cell_car(args);
	return macroexpand (expr, &toplevel);
}

void *proc_gensym (type_cell *args) {
	return gensym ();
}

void *proc_typeof (type_cell *args) {
	void *ret;
	gc_type t = get_type (cell_car(args));

    ret = intern (type_names[t]);
}

void *proc_print (type_cell *args) {
	print_object (cell_car (args));
	putch ('\n');
	return NULL;
}


void *proc_princ (type_cell *args) {
	print_object_nice (cell_car (args));
	return NULL;
}

void *proc_format (type_cell *args) {
	if (get_type (cell_car (args)) == TYPE_STRING) {
		format ((type_string *)cell_car (args), cell_cdr (args));
	} else {
		error ("Format string must be a string", "FORMAT");
	}
}

/* print information about the toplevel env */
void *proc_toplevel (type_cell *args) {
	/* print all the defined procedures */
	size_t i;
	type_ht *ht = toplevel.special;
	type_cell *c;
	
	for (i=0; i < ht->size; i++) {
		c = ht->buckets[i];
		while (c != NULL) {
			print_object (cell_car (cell_car (c))); print_string (" ");
			c = c->cdr;
		}
	}
	print_string ("\n");
	
	return ht;
}

void *proc_gethash (type_cell *args) {
	type_ht *ht;
	void *key, *ret;
	bool found;
	type_cell *c;
	
	ht = cell_car(args);
	key = cell_cadr (args);

	if (get_type(ht) == TYPE_HT) {
		found = gethash (ht, key, &ret);
		c = cons (ret, NULL);
		if (found) {
			c->cdr = cons (intern("T"), NULL);
		} else {
			c->cdr = cons (NULL, NULL);
		}
		ret = c;
	} else {
		error ("Gethash expects a hash table as the first argument", "GETHASH");
		ret = NULL;
	}
	return ret;
}


void *proc_sethash (type_cell *args) {
	type_ht *ht;
	void *key, *val;
	
	ht = cell_car(args);
	args = cell_cdr (args);
	key = cell_car (args);
	val = cell_cadr (args);
	
	if (get_type(ht) == TYPE_HT) {
		sethash (&ht, key, val);
	} else {
		error ("Sethash expects a hash table as the first argument", "SETHASH");
	}
	return NULL;
}

void *proc_remhash (type_cell *args) {
	type_ht *ht;
	void *key;
	
	ht = cell_car (args);
	key = cell_cadr (args);

	if (get_type (ht) == TYPE_HT) {
		remhash (ht, key);
	} else {
		error ("Remhash expects a hash table as the first argument", "REMHASH");
	}

	return NULL;
}

	
void *proc_heap (type_cell *args) {
	int used, tot;
	used = (int)(free_p - gc_working);
	tot = (int)gc_heap_size / 2;
	print_string ("Heap: ");
	print_int (used);
	print_string (" / ");
	print_int (tot);
	print_string (" (");
	print_int ((100 * used)/tot);
	print_string ("%)\n");

	used = (int)(string_table_p - string_table);
	tot = (int)symbol_table_size;
	print_string ("Symbol table: ");
	print_int (used);
	print_string (" / ");
	print_int (tot);
	print_string (" (");
	print_int ((100 * used)/tot);
	print_string ("%)\n");

    return NULL;
}


void *proc_copy_list (type_cell *args) {
	return copy_list (cell_car (args));
}

void *proc_error (type_cell *args) {
	type_string *message, *location;

	message = cell_car (args);
	location = cell_cadr (args);

	error (message->str, location->str);
	return NULL;
}

void *math_helper (type_cell *args, double (*fun)(double)) {
	double x;
	void *val;

	val = cell_car (args);

	switch (get_type (val)) {
	case TYPE_INT:
		x = (double)CAST(type_int *, val)->i;
		break;
	case TYPE_DOUBLE:
		x = CAST(type_double *, val)->d;
		break;
	default:
		error ("Not a number", "MATH");
		x = 0.0;
	}

	return gc_new_double ((*fun)(x));
}

void *proc_cos (type_cell *args) {
	return math_helper (args, &cos);
}

void *proc_sin (type_cell *args) {
	return math_helper (args, &sin);
}

void *proc_tan (type_cell *args) {
	return math_helper (args, &tan);
}

void *proc_acos (type_cell *args) {
	return math_helper (args, &acos);
}

void *proc_asin (type_cell *args) {
	return math_helper (args, &asin);
}

/* atan is different as we have atan2 */
void *proc_atan (type_cell *args) {
	double x, y;
	void *val;

	val = cell_car (args);

	switch (get_type (val)) {
	case TYPE_INT:
		x = (double)CAST(type_int *, val)->i;
		break;
	case TYPE_DOUBLE:
		x = CAST(type_double *, val)->d;
		break;
	default:
		error ("Not a number", "MATH");
		x = 0.0;
	}

	val = cell_cadr (args);
	if (val) {
		switch (get_type (val)) {
		case TYPE_INT:
			y = (double)CAST(type_int *, val)->i;
			break;
		case TYPE_DOUBLE:
			y = CAST(type_double *, val)->d;
			break;
		default:
			error ("Not a number", "MATH");
			y = 0.0;
		}
		return gc_new_double (atan2(y, x));
	} else {
		return gc_new_double (atan (x));
	}
}

void *proc_cosh (type_cell *args) {
	return math_helper (args, &cosh);
}

void *proc_sinh (type_cell *args) {
	return math_helper (args, &sinh);
}

void *proc_tanh (type_cell *args) {
	return math_helper (args, &tanh);
}

void *proc_acosh (type_cell *args) {
	return math_helper (args, &acosh);
}

void *proc_asinh (type_cell *args) {
	return math_helper (args, &asinh);
}

void *proc_atanh (type_cell *args) {
	return math_helper (args, &atanh);
}

void *proc_exp (type_cell *args) {
	return math_helper (args, &exp);
}

void *proc_log (type_cell *args) {
	return math_helper (args, &log);
}

void *proc_log10 (type_cell *args) {
	return math_helper (args, &log10);
}

void *proc_sqrt (type_cell *args) {
	return math_helper (args, &sqrt);
}

void *proc_ceil (type_cell *args) {
	return math_helper (args, &ceil);
}

void *proc_fabs (type_cell *args) {
	return math_helper(args, &fabs);
}

void *proc_floor (type_cell *args) {
	return math_helper(args, &floor);
}

void *proc_erf (type_cell *args) {
	return math_helper(args, &erf);
}

void *proc_erfc (type_cell *args) {
	return math_helper(args, &erfc);
}

void *proc_gamma (type_cell *args) {
	return math_helper(args, &gamma);
}

void *proc_j0 (type_cell *args) {
	return math_helper(args, &j0);
}

void *proc_j1 (type_cell *args) {
	return math_helper(args, &j1);
}

void *proc_jn (type_cell *args) {
	double x;
	int n;
	void *val;

	val = cell_car (args);
	switch (get_type (val)) {
	case TYPE_INT:
		x = (double)CAST(type_int *, val)->i;
		break;
	case TYPE_DOUBLE:
		x = CAST(type_double *, val)->d;
		break;
	default:
		error ("Argument 0 must be a number", "BESSEL");
		x = 0.0;
	}

	val = cell_cadr (args);
	switch (get_type (val)) {
	case TYPE_INT:
		n = CAST(type_int *, val)->i;
		break;
	default:
		error ("Argument 1 must be an integer", "BESSEL");
		n = 0;
	}

	if (n == 0) {
		return gc_new_double (j0(x));
	} else if (n == 1) {
		return gc_new_double (j1(x));
	} else {
		return gc_new_double (jn (n, x));
	}
}

	
void *proc_lgamma (type_cell *args) {
	return math_helper(args, &lgamma);
}

void *proc_y0 (type_cell *args) {
	return math_helper(args, &y0);
}

void *proc_y1 (type_cell *args) {
	return math_helper(args, &y1);
}

void *proc_yn (type_cell *args) {
	double x;
	int n;
	void *val;

	val = cell_car (args);
	switch (get_type (val)) {
	case TYPE_INT:
		x = (double)CAST(type_int *, val)->i;
		break;
	case TYPE_DOUBLE:
		x = CAST(type_double *, val)->d;
		break;
	default:
		error ("Argument 0 must be a number", "BESSEL");
		x = 0.0;
	}

	val = cell_cadr (args);
	switch (get_type (val)) {
	case TYPE_INT:
		n = CAST(type_int *, val)->i;
		break;
	default:
		error ("Argument 1 must be an integer", "BESSEL");
		n = 0;
	}

	if (n == 0) {
		return gc_new_double (y0(x));
	} else if (n == 1) {
		return gc_new_double (y1(x));
	} else {
		return gc_new_double (yn (n, x));
	}
}
