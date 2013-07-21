
#ifndef HT_H
#define HT_H

#include "sys.h"
#include "gc.h"
#include "symbol.h"

typedef struct hash_table_entry_ {
	symbol key;
	value *val;
	struct hash_table_entry_ * next;
} hash_table_entry;

typedef struct {
	value **entries;
	unsigned int size;
	unsigned int fill;
} hash_table;

value *gethash (hash_table *ht, symbol key);
void sethash (hash_table *ht, symbol key, void *val);
void remhash (hash_table *ht, symbol key);

unsigned int hash_string (char *str);

#endif

