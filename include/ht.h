
#ifndef HT_H
#define HT_H

#include "sys.h"
#include "gc.h"
#include "symbol.h"

/* hash tables are arrays of values. each value is an assoc list */

value *gethash (hash_table *ht, symbol key);
void sethash (hash_table *ht, symbol key, void *val);
void remhash (hash_table *ht, symbol key);

unsigned int hash_string (char *str);

#endif

