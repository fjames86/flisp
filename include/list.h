
#ifndef LIST_H
#define LIST_H

#include "sys.h"
#include "value.h"
#include "gc.h"
#include "symbol.h"

typedef struct {
	value *car;
	value *cdr;
} cell;

/* returns length of the list or -1 if it contains a cycle */
int list_length (cell *list);

value *cons (value *car, value *cdr);
value *car (cell *list);
value *cdr (cell *list);
value *cddr (cell *list);
value *cadr (cell *list);

value *acons (symbol key, value *val, value *a);
value *assoc (symbol key, value *a);

#endif
