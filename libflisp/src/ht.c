
/*
 * Copyright (C) 2013 Frank James
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

#include "ht.h"

bool gethash (type_ht *ht, void *key, void **val) {
	size_t i;
	type_cell *c;

	if (ht == NULL) {
	  error ("Hash table not initialised", "GETHASH");
	  *val = NULL;
	  return FALSE;
	}

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

	if (ht == NULL) {
	  error ("Hash table not initialised", "SETHASH");
	}

	
	i = sxhash(key) % ht->size;
    c = ht->buckets[i];
#if 0
    while (c != NULL) {
      /* c = ((key . val) ....) */
      e = cell_car(c);
      if (eql(cell_car(e), key)) {
        e->cdr = val;
        return;
      }
      c = c->cdr; 
    }
#endif 
    e = assoc (key, c);
    if (e != NULL) {
      e->cdr = val;
      return;
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
	c = &(ht->buckets[i]);

	while (*c != NULL) {
        if (eql(cell_car (*c), key)) {
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
