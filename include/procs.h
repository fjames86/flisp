
#ifndef PROCS_H
#define PROCS_H

#include "sys.h"
#include "types.h"
#include "gc.h"


void *proc_quit (type_cell *args);
void *proc_add (type_cell *args);
void *proc_sub (type_cell *args);
void *proc_mul (type_cell *args);
void *proc_div (type_cell *args);

#endif 

