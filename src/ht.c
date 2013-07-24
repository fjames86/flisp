
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

void sethash(type_ht *ht, void *key, void *val) {
  size_t i;
  type_cell **c;

  i = sxhash(key) % ht->size;
  c = &(ht->buckets[i]);
  while (*c != NULL) {
    if (eq(cell_car(*c), key)) {
      set_cdr(c, val);
      break;
    } 
    c = (type_cell **)&((*c)->cdr);
  }

  /* not found so need to add a new entry */
  ht->buckets[i] = acons(key, val, &(ht->buckets[i]));
  (ht->fill)++;
}

void remhash(type_ht *ht, void *key) {
  size_t i;
  type_cell **c;

  i = sxhash(key) % ht->size;
  c = &(ht->buckets[i]);

  while (*c != NULL) {
    if (eq((*c)->car, key)) {
      *c = (*c)->cdr;
      break;
    }
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
      sethash(*ht, cell_caar(c), cell_cdar(c));
      c = c->cdr;
    }
  }
}

size_t sxhash(void *val) {
  return 42;
}
