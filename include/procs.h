
#ifndef PROCS_H
#define PROCS_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "array.h"
#include "lists.h"

void *proc_car (type_cell *args);
void *proc_cdr (type_cell *args);
void *proc_set_car (type_cell *args);
void *proc_set_cdr (type_cell *args);

void *proc_quit (type_cell *args);
void *proc_add (type_cell *args);
void *proc_sub (type_cell *args);
void *proc_mul (type_cell *args);
void *proc_div (type_cell *args);

void *proc_make_array (type_cell *args);
void *proc_aref (type_cell *args);
void *proc_set_aref (type_cell *args);

#endif 

