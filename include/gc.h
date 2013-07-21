
#ifndef GC_H
#define GC_H

#include "sys.h"
#include "value.h"
#include "list.h"
#include "symbol.h"

void *gc_working;
void *gc_free;
unsigned int gc_heap_size;
void *free_p;

void gc_init (void *total_heap, size_t size);
void *gc_malloc(unsigned int num);
value *gc_new ();
value *gc_new_int (int val);
value *gc_new_string (char *str);
value *gc_new_cell ();
value *gc_new_array (size_t size);
value *gc_new_symbol(symbol s);


void gc_relocate (value *new, value *old);
void gc_collect (value **root);
void gc_collect_init ();

#endif 
