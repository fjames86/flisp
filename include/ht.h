
#ifndef HT_H
#define HT_H

#include "sys.h"
#include "gc.h"
#include "lists.h"


#define HT_RESIZE    1.5
#define HT_THRESHOLD 0.75

bool gethash(type_ht *ht, void *key, void **val);
void sethash(type_ht *ht, void *key, void *val);
void remhash(type_ht *ht, void *key);
void clearhash(type_ht *ht);
void ht_resize(type_ht **ht);

size_t sxhash(void *val);

#endif 

