
#include "ht.h"

value *gethash (hash_table *ht, symbol key) {
	size_t i;

	
	i = hash_string(key) % ht->size;
	entry = ht->entries[i];
	while (entry != NULL) {
		if (entry->key == key) {
			return entry->val;
		}
		entry = entry->next;
	}
	
	return NULL;
}

value *make_hash_table_entry (symbol key, void *val) {
	value *v;
	hash_table_entry *entry;
	
	v = gc_new_block(sizeof(hash_table_entry));
	entry = (hash_table_entry *)v->data;
	entry->key = key;
	entry->val = val;
	entry->next = NULL;

	return v;
}
	
void sethash (hash_table *ht, symbol key, void *val) {
	unsigned int i;
	hash_table_entry *entry;
	
	i = hash_string (key) % ht->size;
	entry = ht->entries[i];
	while (entry->next != NULL) {
		if (entry->key == key) {
			entry->val = val;
			return;
		} 
		entry = entry->next;
	}

	entry->next = gc_malloc(sizeof(hash_table_entry));
	entry->key = key;
	entry->val = val;

	ht->fill += 1;
}

void remhash (hash_table *ht, symbol key) {
	unsigned int i;
	hash_table_entry *entry, *e;
	
	i = hash_string (key) % ht->size;
	entry = ht->entries[i];
	e = NULL;
	while (entry != NULL) {
		if (entry->key != key) {
			/* keep it */
			e = entry;
			e->next = NULL;
		}
		entry = entry->next;
	}
	ht->entries[i] = e;
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
