
#ifndef HT_H
#define HT_H

#include "sys.h"
#include "gc.h"
#include "symbol.h"
#include "list.h"

/* hash tables are arrays of values. each value is an assoc list */

value *gethash (value *ht, symbol key);
void sethash (value *ht, symbol key, void *val);
void remhash (value *ht, symbol key);
void clearhash (value *ht);

unsigned int hash_string (char *str);

#endif

