
#ifndef PROCS_H
#define PROCS_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "array.h"
#include "lists.h"

extern void *apply (void *proc, type_cell *args);
extern void load_file (char *fname);

void *proc_car (type_cell *args);
void *proc_cdr (type_cell *args);
void *proc_set_car (type_cell *args);
void *proc_set_cdr (type_cell *args);
void *proc_cons (type_cell *args);
void *proc_list (type_cell *args);
void *proc_reverse (type_cell *args);

void *proc_quit (type_cell *args);
void *proc_add (type_cell *args);
void *proc_sub (type_cell *args);
void *proc_mul (type_cell *args);
void *proc_div (type_cell *args);

void *proc_make_array (type_cell *args);
void *proc_aref (type_cell *args);
void *proc_set_aref (type_cell *args);

void *proc_apply (type_cell *args);
void *proc_load (type_cell *args);
void *proc_eq (type_cell *args);
void *proc_eql (type_cell *args);
void *proc_equal (type_cell *args);
void *proc_gt (type_cell *args);
void *proc_gte (type_cell *args);
void *proc_lt (type_cell *args);
void *proc_lte (type_cell *args);
void *proc_e (type_cell *args);

#endif 

