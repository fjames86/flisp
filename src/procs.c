

#include "procs.h"

/* (car cell) */
void *proc_car (type_cell *args) {
	void *c;

	c = cell_car(args);
	if (get_type(c) == TYPE_CELL) {
		c = cell_car(c);
	} else {
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

void *proc_quit (type_cell *args) {
	printf("Bye.\n");
	exit(0);
	return NULL;
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
			i2 = 1 / i1;
		} else {
			d2 = 1.0 / d1;
		}
	} else {
		subs = proc_add((type_cell *)args);
		t2 = get_type(subs);
		if (t2 == TYPE_INT) {
			if (t1 == TYPE_INT) {
				i2 = i1 / CAST(type_int *, subs)->i;
			} else if (t1 == TYPE_DOUBLE) {
				d2 = d1 / (double)CAST(type_int *, subs)->i;
			}      
		} else if (t2 == TYPE_DOUBLE) {
			if (t1 == TYPE_INT) {
				d2 = (double)i1 / CAST(type_double *, subs)->d;
				t1 = TYPE_DOUBLE;
			} else if (t1 == TYPE_DOUBLE) {
				d2 = d1 / CAST(type_double *, subs)->d;
			}
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
			ret = NULL;
		}
	} else {
		ret = NULL;
	}
	return ret;
}

/* (set-aref! array index val) */
void *proc_set_aref (type_cell *args) {
	type_array *a;
	size_t i;
	void *val, *x, *ret;
	
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
			ret = NULL;
		}
	} else {
		ret = NULL;
	}
	return ret;
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

void *proc_apply (type_cell *args) {
	void *proc;

	proc = cell_car(args);
	args = cell_cdr(args);
	return apply(proc, args);
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

