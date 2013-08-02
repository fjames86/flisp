
#include "ht.h"

bool gethash (type_ht *ht, void *key, void **val) {
	size_t i;
	type_cell *c;
  
	i = sxhash(key) % ht->size;
	c = assoc(key, ht->buckets[i]);
	if (c != NULL) {
		*val = cell_cdr(c);
		return TRUE;
	} else {
		*val = NULL;
		return FALSE;
	}
}

void sethash(type_ht **htb, void *key, void *val) {
	size_t i;
	type_cell *c, *e;
	type_ht *ht;

	ht = *htb;
	
	i = sxhash(key) % ht->size;
    c = ht->buckets[i];
    while (c != NULL) {
      /* c = ((key . val) ....) */
      e = cell_car(c);
      if (eql(cell_car(e), key)) {
        e->cdr = val;
        return;
      }
      c = c->cdr; 
    }

	/* if fill level larger than the threshold then resize */
	if (((double)ht->fill / (double)ht->size) > HT_THRESHOLD) {
		ht_resize(htb);
		ht = *htb;
	}
	
	/* not found so need to add a new entry */
	ht->buckets[i] = acons(key, val, ht->buckets[i]);
	ht->fill++;
}

void remhash(type_ht *ht, void *key) {
	size_t i;
	type_cell **c;

	i = sxhash(key) % ht->size;
	c = ht->buckets + i;

	while (*c != NULL) {
        if (eql((*c)->car, key)) {
			*c = (*c)->cdr;
            ht->fill--;
			break;
		}
        c = (type_cell **)&((*c)->cdr);
	}  
}

void clearhash(type_ht *ht) {
	size_t i;

	for(i=0; i < ht->size; i++) {
		ht->buckets[i] = NULL;
	}
}

void ht_resize(type_ht **ht) {
	size_t i, oldsize, newsize;
	type_cell *c, **oldbuckets;

	oldsize = (*ht)->size;
	newsize = (size_t)(HT_RESIZE*(double)((*ht)->size));

	oldbuckets = (*ht)->buckets;

	*ht = gc_new_ht(newsize);

	for(i=0; i < oldsize; i++) {
		c = oldbuckets[i];
		while (c != NULL) {      
            /* c = ((key . val) ...) so we want key = (caar c), val = (cdar c) */
			sethash(ht, cell_caar(c), cell_cdar(c));
			c = c->cdr;
		}
	}
}

size_t sxhash(void *val) {
  gc_type type = get_type(val);
	size_t ret = 42;
	
	switch (type) {
	case TYPE_INT:
		ret = ((type_int *)val)->i;
		break;
	case TYPE_DOUBLE:
		ret = (size_t)((type_double *)val)->d;
		break;
	case TYPE_STRING:
		ret = sxhash_string(((type_string *)val)->str);
		break;
	case TYPE_SYMBOL:
		ret = sxhash_string(((type_symbol *)val)->sym);
		break;
	case TYPE_CELL:
		ret = 42;
		break;
	}

	return ret;
}

size_t sxhash_string (char *str) {
	size_t len = strlen(str); 
	size_t intLength = len / 4;
	unsigned int sum = 0;
	unsigned int mul = 0;
	size_t j, k;
	char *c;
	
	for (j = 0; j < intLength; j++) {
		c = &str[j*4]; 
		mul = 1;
		for (k = 0; k < 4; k++) {
			sum += c[k] * mul;
			mul *= 256;
		}
	}

	c = &str[intLength * 4];
	mul = 1;
	for (k = 0; k < len - (intLength * 4); k++) {
		sum += c[k] * mul;
		mul *= 256;
	}

	return abs(sum);
}
