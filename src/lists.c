
#include "lists.h"


type_cell *cons (void *car, void *cdr) {
	type_cell *c = gc_new_cell();

	c->car = car;
	c->cdr = cdr;

    return c;
}

void *cell_car(type_cell *cell) {
	if (cell == NULL) {
		return NULL;
	} else {
		return cell->car;
	}
}

void *cell_cdr(type_cell *cell) {
	if (cell == NULL) {
		return NULL;
	} else {
		return cell->cdr;
	}
}

void *cell_caar(type_cell *cell) {
  return cell_car(cell_car(cell));
}

void *cell_cddr(type_cell *cell) {
  return cell_cdr(cell_cdr(cell));
}

void *cell_cdar(type_cell *cell) {
  return cell_cdr(cell_car(cell));
}

void *cell_cadr(type_cell *cell) {
  return cell_car(cell_cdr(cell));
}

void set_car(type_cell *cell, void *val) {
  cell->car = val;
}

void set_cdr(type_cell *cell, void *val) {
  cell->cdr = val;
}

void cell_push(type_cell **place, void *val) {
  (*place) = cons(val, *place);
}

void *cell_pop(type_cell **place) {
    void *ret;
    ret = (*place)->car;
    *place = cell_cdr(*place);
    return ret;
}


/* this does essentially cons(cons(key, val), alist) */
type_cell *acons (void *key, void *val, type_cell *alist) {
	type_cell *c = gc_new_cell();
	type_cell *d = gc_new_cell();
	
	c->car = key;
	c->cdr = val;
    
	d->car = c;
	d->cdr = alist;

	return d;
}

void *assoc (void *key, type_cell *alist) {
	type_cell *c;
	
	while (alist != NULL) {
        c = cell_car(alist);
        if (c == NULL) {
            break;
        }
		if (eql(key, c->car)) {
            return c;
		}
		
		alist = alist->cdr;
	}
    
	return NULL;
}
