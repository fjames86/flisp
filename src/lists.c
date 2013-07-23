
#include "lists.h"


type_cell *cons (void *car, void *cdr) {
	type_cell *c = gc_new_cell();

	c->car = car;
	c->cdr = cdr;
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


type_cell *acons (void *key, void *val, void **alist) {
	type_cell *c = gc_new_cell();
	type_cell *d = gc_new_cell();
	
	c->car = key;
	c->cdr = val;

	d->car = c;
	d->cdr = *alist;
	*alist = d;
	*alist;
}

void *assoc (void *key, type_cell *alist) {
	type_cell *c;
	
	while (alist != NULL) {
		c = (type_cell *)(alist->car);
		if (eq(key, c->car)) {
			return c;
		}
		
		alist = alist->cdr;
	}

	return NULL;
}
