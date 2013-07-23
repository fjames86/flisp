
#include "gc.h"

/*
 * gc_init
 * void *total_heap        pointer to start of the chunk of memory to use as the gc heap
 * size_t size             total size of the chunk of memory
 *
 * sets up the pointers to initialise the heap
 */
void gc_init (void *total_heap, size_t size) {
	gc_heap_size = size / 2;
	gc_working = total_heap;
	gc_free = total_heap + gc_heap_size;
	
	free_p = gc_working;
}

/*
 * gc_malloc
 * size_t size             number of bytes to return
 *
 * returns: pointer to start of block of memory
 *
 * returns the current top of the heap and advances the pointers or NULL if no heap space
 * remains
 */
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


/* ---------------- constructors ----------------- */

/* for each type define here a constructor to return a new object
   on the heap
*/

type_int *gc_new_int (int i) {
	type_int *ret = (type_int *)gc_malloc(sizeof(type_int));
	ret->tag.type = TYPE_INT;
	ret->tag.bh = FALSE;
	ret->tag.forw = NULL;

	ret->i = i;

	return ret;
}

type_string *gc_new_string (char *str) {
	size_t len = strlen(str) + 1;
	type_string *ret = (type_string *)gc_malloc(sizeof(type_string));
	ret->tag.type = TYPE_STRING;
	ret->tag.bh = FALSE;
	ret->tag.forw = NULL;

	ret->str = (char *)gc_malloc(sizeof(char)*len);
	strcpy(ret->str, str);
	ret->len = len;

	return ret;
}

type_cell *gc_new_cell () {
	type_cell *ret = gc_malloc(sizeof(type_cell));
	ret->tag.type = TYPE_CELL;
	ret->tag.bh = FALSE;
	ret->tag.forw = NULL;

	ret->car = NULL;
	ret->cdr = NULL;

	return ret;
}

/* symbols are essentiall the same as strings */
type_symbol *gc_new_symbol (char *str) {
	type_symbol *ret = gc_malloc(sizeof(type_symbol));
	size_t len = strlen(str) + 1;	
	ret->tag.type = TYPE_SYMBOL;
	ret->tag.bh = FALSE;
	ret->tag.forw = NULL;

	ret->sym = (char *)gc_malloc(sizeof(char)*len);
	strcpy(ret->sym, str);
	return ret;
}

type_double *gc_new_double (double d) {
	type_double *ret = gc_malloc(sizeof(type_double));
	ret->tag.type = TYPE_DOUBLE;
	ret->tag.bh = FALSE;
	ret->tag.forw = NULL;

	ret->d = d;
	return ret;
}

/* make a new object based on its type */
void *gc_new_copy (void *object) {
	gc_type type;
	void *ret = NULL;
	
	type = ((gc_tag *)object)->type;

	switch (type) {
	case TYPE_INT:
		ret = gc_new_int (((type_int *)object)->i);
		break;
	case TYPE_STRING:
		ret = gc_new_string (((type_string *)object)->str);
		break;
	case TYPE_CELL:
		ret = gc_new_cell ();
		break;
	case TYPE_SYMBOL:
		ret = gc_new_symbol(((type_symbol *)object)->sym);
		break;
	case TYPE_DOUBLE:
		ret = gc_new_double(((type_double *)object)->d);
		break;
	}
	return ret;
}


/* ------------- destructors for each type follow ------------------------------------- */

/* each type needs to know how to move its contents around */

/* ints don't store any references so don't need to do anything
 * this is the simplest base case
 */
void gc_relocate_int (type_int **new, type_int *old) {
	old->tag.bh = TRUE;
	old->tag.forw = new;
}

/* strings are also pretty simple. they only store a reference to a passive
 * block of data (the characters) and this gets copied automatically by the
 * constructor
 */
void gc_relocate_string (type_string **new, type_string *old) {
	old->tag.bh = TRUE;
	old->tag.forw = new;
}

/* list cells are more complicated. both the car and cdr may reference
 * other objects. if this has already been moved, as indicated by the broken
 * heart flag, then just use the forwarding address. Otherwise, it's not
 * been moved so call the relocate function on it
 */
void gc_relocate_cell (type_cell **new, type_cell *old) {
	gc_tag *tag;
	void **d;

	old->tag.bh = TRUE;
	old->tag.forw = (void *)new;

	if (old->car != NULL) {
		tag = ((gc_tag *)(old->car));
		if (tag->bh == TRUE) {
			(*new)->car = tag->forw;
		} else {
			gc_relocate(&((*new)->car), old->car);
		}
	}

	if (old->cdr != NULL) {
		tag = ((gc_tag *)(old->cdr));
		if (tag->bh == TRUE) {
			(*new)->car = tag->forw;
		} else {
			gc_relocate(&((*new)->cdr), old->cdr);
		}
	}
}

/* relocate a symbol. this is as easy as relocating an int because symbols aren't collected */
void gc_relocate_symbol (type_symbol **new, type_symbol *old) {
	old->tag.bh = TRUE;
	old->tag.forw = new;
}

/* relocating a double is also easy */
void gc_relocate_double (type_double **new, type_double *old) {
	old->tag.bh = TRUE;
	old->tag.forw = new;
}


/* --------- top level relocate function -------- */

void gc_relocate (void **new, void *old) {
	gc_type type;

	if (*new == NULL) {
		*new = gc_new_copy (old);
	}
	
	type = ((gc_tag *)old)->type;
	switch (type) {
	case TYPE_INT:
		gc_relocate_int ((type_int **)new, (type_int *)old);
		break;
	case TYPE_STRING:
		gc_relocate_string ((type_string **)new, (type_string *)old);
		break;
	case TYPE_CELL:
		gc_relocate_cell ((type_cell **)new, (type_cell *)old);
		break;
	case TYPE_SYMBOL:
		gc_relocate_symbol ((type_symbol **)new, (type_symbol *)old);
		break;
	case TYPE_DOUBLE:
		gc_relocate_double ((type_double **)new, (type_double *)old);
		break;
	}
}

void gc_collect (void **root) {
	void *new, *old;

	old = *root;
	
	/* make the new object */
	if (old != NULL) {
		*root = gc_new_copy(old);
		
		gc_relocate(root, old);
	}
}

void gc_collect_init () {
	void *tmp;
	
	free_p = gc_free;		
	tmp = gc_working;
	gc_working = gc_free;
	gc_free = tmp;
}
