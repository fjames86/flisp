
#ifndef GC_H
#define GC_H

#include "sys.h"
#include "types.h"

/* globals used to keep track of the heap */
void *gc_working;
void *gc_free;
size_t gc_heap_size;
void *free_p;

/* initialisation function */
void gc_init (void *total_heap, size_t size);

/* memory allocation functions */
void *gc_malloc (size_t size);
type_int *gc_new_int (int i);
type_string *gc_new_string (char *str);
type_cell *gc_new_cell ();
void *gc_new_copy (void *object);

/* functions to move objects around in the heap */
void gc_relocate_int (type_int *new, type_int *old);
void gc_relocate_string (type_string *new, type_string *old);
void gc_relocate_cell (type_cell *new, type_cell *old);

void gc_relocate (void *new, void *old);

/* call gc_collect_init() and then gc_collect() on each of the root variables */
void gc_collect_init ();
void gc_collect (void **root);

#endif 
