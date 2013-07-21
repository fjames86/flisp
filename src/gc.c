
#include "gc.h"


void gc_init (void *total_heap, unsigned int size) {
	gc_heap_size = size / 2;
	gc_working = total_heap;
	gc_free = total_heap + gc_heap_size;
	
	free_p = gc_working;
}

void *gc_malloc (size_t size) {
	void *p;

	if ((free_p - gc_working + size) < gc_heap_size) {
		p = free_p;
		free_p += size;
	} else {
		p = NULL;
	}
	return p;
}

value *gc_new () {
	value *v = gc_malloc (sizeof(value));

	if (v == NULL) {
		return NULL;
	} else {
		v->data = NULL;
		v->type = TYPE_INT;
		v->size = 0;
		v->bh = FALSE;
		v->forw = NULL;
	}
	return v;
}

value *gc_new_int (int val) {
	value *v = gc_new();
	v->data = (void *)val;
	return v;
}

value *gc_new_string (char *str) {
	unsigned int strl = strlen(str) + 1;
	value *v = gc_new();

	if (v == NULL) {
		return NULL;
	} else {
		v->data = gc_malloc(sizeof(char)*strl);
		v->size = strl;
		v->type = TYPE_STRING;
		if (v->data) {
			strcpy(v->data, str);
		}
	}
	
	return v;
}

value *gc_new_symbol (symbol s) {
	value *v = gc_new();
	if (v != NULL) {
		v->type = TYPE_SYMBOL;
		v->data = s;
	}
	return v;
}
	   
value *gc_new_cell () {
	value *v = gc_new();
	if (v == NULL) {
	} else {
		v->type = TYPE_CELL;
	}
	return v;
}

/* return an array of values */
value *gc_new_array (size_t size) {
	value *v = gc_new();
	size_t i;
	if (v != NULL) {
		v->data = gc_malloc(sizeof(value *)*size);
		v->type = TYPE_ARRAY;
		v->size = sizeof(value)*size;

		for (i = 0; i < size; i++) {
			((value **)(v->data))[i] = gc_new();
		}
	}
	return v;
}

/*
value *gc_new_ht (size_t ht_size) {
	value *v = gc_new();
	hash_table *ht;
	
	v->data = gc_malloc(sizeof(hash_table));
	ht = (hash_table *)v->data;
	
	ht->data = gc_malloc(sizeof(void *)*ht_size);
	ht->size = ht_size;
	return v;
}
*/

/*
 * relocate an object from old memory to new memory
 * and recursively call on any child objects
 */
void gc_relocate (value *old) {
	value *new, *v;
	cell *c;
	size_t i;
	
	/* make the new object */
	new = gc_new();
	
	/* set the old objects broken heart and forwarding tags */
	old->bh = TRUE;
	old->forw = new;

	new->type = old->type;
	new->data = old->data;
	new->size = old->size;
	new->bh = FALSE;
	new->forw = NULL;
	
	/* if this is not an atom then it may reference child objects. need to ensure these are updated too */
	switch (new->type) {
	case TYPE_STRING:
		/* copy the string/block of data across */
		new->data = gc_malloc(sizeof(char)*new->size);
		memcpy(new->data, old->data, new->size);
		break;
	case TYPE_CELL:
		/* cons cell, need to check the car and cdr */
		c = (cell *)new->data;
		
		/* only need to do something if not the empty list */
		if (c != NULL) {
			/* if the bh flag is set then this object has already been copied over,
			 *  so simply replace with the forwarding address, otherwise relocate it
			 */
			if (c->car->bh == TRUE) {
				c->car = c->car->forw;
			} else {
				gc_relocate(c->car);
			}

			if (c->cdr->bh == TRUE) {
				c->cdr = c->cdr->forw;
			} else {
				gc_relocate(c->cdr);
			}
		}
		break;
	case TYPE_ARRAY:
		/* a flat array of values. need to check each of them */

		for (i=0; i < new->size; i++) {
			v = ((value **)(new->data))[i];			
			if (v->bh == TRUE) {
				/* this has already been relocated, just update the reference */
				((value **)(new->data))[i] = v->forw;
			} else {
				/* relocate it */
				gc_relocate(v);
			}
		}
		break;
	}
}

void gc_collect_init () {
	void *tmp;
	
	free_p = gc_free;		
	tmp = gc_working;
	gc_working = gc_free;
	gc_free = tmp;
}
