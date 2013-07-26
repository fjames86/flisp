
#ifndef LIST_H
#define LIST_H

#include "sys.h"
#include "types.h"
#include "gc.h"

type_cell *cons (void *car, void *cdr);
void *cell_car (type_cell *cell);
void *cell_cdr (type_cell *cell);
void *cell_caar (type_cell *cell);
void *cell_cdar (type_cell *cell);
void *cell_cddr (type_cell *cell);
void *cell_cadr (type_cell *cell);
void cell_push (type_cell **place, void *val);
void *cell_pop (type_cell **place);

void set_car(type_cell *cell, void *val);
void set_cdr(type_cell *cell, void *val);

type_cell *acons (void *key, void *val, type_cell *alist);
void *assoc (void *key, type_cell *alist);


#endif

