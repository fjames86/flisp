
#ifndef SYMBOL_H
#define SYMBOL_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "lists.h"

/* symbols must not be garbage collected, so must live outside the heap */

/* store pointers to the strings from the top of the stack */
char **symbol_table;
char **symbol_p;

/* store the strings from the bottom */
char *string_table_p;
size_t symbol_table_size;

void symbol_init (char *table, size_t size);
char *intern_new_symbol (char *str);
char *lookup_symbol (char *str);
type_symbol *intern (char *str);

#endif

