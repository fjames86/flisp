
#ifndef LIST_H
#define LIST_H

#include "sys.h"
#include "types.h"
#include "gc.h"

type_cell *cons (void *car, void *cdr);
void *cell_car (type_cell *cell);
void *cell_cdr (type_cell *cell);

type_cell *acons (void *key, void *val, void **alist);
void *assoc (void *key, type_cell *alist);


#endif

