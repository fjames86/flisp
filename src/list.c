
#include "list.h"

/*
 * returns the length of the list, or -1 if it contains a cycle
 */
int list_length (cell *list ) {
	cell *slow, *fast;
	value *v;
	int n = 0;
	
	slow = list;
	fast = list;
	while (1) {
		if (fast == NULL) {
			return n;
		} else if (fast->cdr == NULL) {
			return n + 1;
		} else if ((fast == slow) && (n > 0)) {
			/* if the fast and slow pointers are equal then we are stuck in a circular list */
			return -1;
		}

		v = cdr(slow);
		if (v->type == TYPE_CELL) {
			slow = v->data;
		} else {
			slow = NULL;
		}

		v = cddr(fast);
		if (v->type == TYPE_CELL) {
			fast = v->data;
		} else {
			fast = NULL;
		}
		
		n += 2;
	}
}
	
value *cons (value *car, value *cdr) {
	value *c = gc_new_cell();
	c->data = gc_malloc(sizeof(cell));
	((cell *)(c->data))->car = car;
	((cell *)(c->data))->cdr = cdr;
	return c;
}

value *car(cell *list) {
	if (list == NULL) {
		return NULL;
	} else {
		return list->car;
	}
}

value *cdr (cell *list) {
	if (list == NULL) {
		return NULL;
	} else {
		return list->cdr;
	}
}

value *cddr (cell *list) {
	value *v = cdr(list);
	return cdr((cell *)v->data);
}

value *cadr (cell *list) {
	value *v = cdr(list);
	return car((cell *)v->data);
}


value *acons (symbol key, value *val, value *a) {
	value *v;
	
	v = cons (gc_new_symbol(key), val);
	v = cons (v, a->data);
	return v;
}

value *assoc (symbol key, value *alist) {
	cell *c;
	value *a, *b;
	while (alist->type == TYPE_CELL && alist->data != NULL) {
		c = ((cell *)alist->data);
		a = car(c);
		if (a->type != TYPE_CELL) {
			return NULL;
		}
		b = car((cell *)(a->data));
		if (b->type == TYPE_SYMBOL && b->data == key) {
			return a; /*cdr((cell *)(a->data));*/
		} else {
			alist = cdr(c);
		}
	}
	return NULL;
}

