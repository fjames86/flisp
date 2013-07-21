
#include "ht.h"

value *gethash (value *ht, symbol key) {
	size_t i;
	value *alist, *entry;
	
	i = hash_string(key) % ht->size;
	alist = &((value *)(ht->data))[i];
	return assoc(key, alist);
}

void sethash (value *ht, symbol key, void *val) {
	size_t i; 
	value *entry, *alist;
	
	i = hash_string (key) % ht->size;
	alist = &((value *)(ht->data))[i];
	entry = assoc(key, alist);
	if (entry != NULL) {
		((cell *)(entry->data))->cdr = val;
	} else {
		((value **)(ht->data))[i] = acons(key, val, ((value **)(ht->data))[i]);
	}
}

void remhash (value *ht, symbol key) {
	size_t i;
	value *entry, *e;
	cell *alist;
	
	i = hash_string (key) % ht->size;
	alist = (cell *)(((value **)(ht->data))[i]->data);

	e = NULL;
	while (alist != NULL) {
		entry = car(alist);
		if (car(entry->data)->data != key) {
			/* keep it */
			e = cons(entry, e);
		}
		alist = cdr(alist)->data;
	}
	((value **)(ht->data))[i] = e;
}

void clearhash(value *ht) {
	size_t i;

	for(i=0; i < ht->size; i++) {
		((value **)(ht->data))[i] = gc_new_cell();
	}
}

// Use folding on a string, summed 4 bytes at a time
unsigned int hash_string (char *s) {
	size_t len = strlen(s); 
	size_t intLength = len / 4;
	unsigned int sum = 0;
	unsigned int mul = 0;
	size_t j, k;
	char *c;
	
	for (j = 0; j < intLength; j++) {
		c = &s[j*4]; 
		mul = 1;
		for (k = 0; k < 4; k++) {
			sum += c[k] * mul;
			mul *= 256;
		}
	}

	c = &s[intLength * 4];
	mul = 1;
	for (k = 0; k < len - (intLength * 4); k++) {
		sum += c[k] * mul;
		mul *= 256;
	}

	return abs(sum);
}
