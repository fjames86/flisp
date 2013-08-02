
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
		/*
		 * need to trigger a garbage collection here. can't because we don't know the root points
		 * however, returning a NULL signals we're out of memory.
		 * probably should be checking the current state of the heap somewhere higher up and
		 * trigger a collection before it gets to this stage, because the results of returning NULL
		 * here is almost universally bad 
		 */
		p = NULL;
	}
	return p;
}


/* ---------------- constructors ----------------- */

/*
 * for each type define here a constructor to return a new object
 *  on the heap
 */

type_int *gc_new_int (int i) {
	type_int *ret = (type_int *)gc_malloc(sizeof(type_int));
	ret->tag.type = TYPE_INT;
	ret->tag.forw = NULL;

	ret->i = i;

	return ret;
}

type_string *gc_new_string (char *str) {
	size_t len = strlen(str) + 1;
	type_string *ret = (type_string *)gc_malloc(sizeof(type_string));
	ret->tag.type = TYPE_STRING;
	ret->tag.forw = NULL;

	ret->str = (char *)gc_malloc(sizeof(char)*len);
	strcpy(ret->str, str);
	ret->len = len;

	return ret;
}

type_cell *gc_new_cell () {
	type_cell *ret = gc_malloc(sizeof(type_cell));
	ret->tag.type = TYPE_CELL;
	ret->tag.forw = NULL;

	ret->car = NULL;
	ret->cdr = NULL;

	return ret;
}

/* symbols are simpler than strings. they just store a static reference to the 
 * string stored in the symbol table, which never gets garbage collected so we can just copy the 
 * pointer. 
 */
type_symbol *gc_new_symbol (char *sym) {
	type_symbol *ret = gc_malloc(sizeof(type_symbol));

	ret->tag.type = TYPE_SYMBOL;
	ret->tag.forw = NULL;

	ret->sym = sym;
	return ret;
}

type_double *gc_new_double (double d) {
	type_double *ret = gc_malloc(sizeof(type_double));
	ret->tag.type = TYPE_DOUBLE;
	ret->tag.forw = NULL;

	ret->d = d;
	return ret;
}

type_ht *gc_new_ht(size_t size) {
	type_ht *ret = gc_malloc(sizeof(type_ht));
	size_t i;
	ret->tag.type = TYPE_HT;
	ret->tag.forw = NULL;

	ret->buckets = gc_malloc(sizeof(type_cell *)*size);
	for(i=0; i < size; i++) {
		ret->buckets[i] = NULL;
	}

	ret->size = size;
	ret->fill = 0;
	return ret;
}

type_array *gc_new_array (size_t size) {
	type_array *ret = gc_malloc(sizeof(type_array));
	size_t i;
	ret->tag.type = TYPE_ARRAY;
	ret->tag.forw = NULL;

	ret->data = gc_malloc(sizeof(void *)*size);
	for(i=0; i < size; i++) {
		ret->data[i] = NULL;
	}

	ret->size = size;
	return ret;	
}

type_proc *gc_new_proc (flisp_proc_t proc) {
	type_proc *ret = gc_malloc(sizeof(type_proc));
	ret->tag.type = TYPE_PROC;
	ret->tag.forw = NULL;
	ret->proc = proc;
	return ret;
}

type_closure *gc_new_closure (type_cell *params, type_cell *body, environment *env) {
	type_closure *ret = gc_malloc(sizeof(type_closure));
	ret->tag.type = TYPE_CLOSURE;
	ret->tag.forw = NULL;
	
	ret->params = params;
	ret->body = body;
	ret->env = env;
	return ret;
}

/* make a new object based on its type */
void *gc_new_copy (void *object) {
	gc_type type;
	void *ret = NULL;
	type_closure *c;
	
	type = get_type(object);
	switch (type) {
    case TYPE_NULL:
        ret = NULL;
        break;
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
    case TYPE_HT:
		ret = gc_new_ht(((type_ht *)object)->size);
		break;
	case TYPE_ARRAY:
		ret = gc_new_array(((type_array *)object)->size);
		break;
    case TYPE_PROC:
        ret = gc_new_proc(((type_proc *)object)->proc);
        break;
	case TYPE_CLOSURE:
		c = CAST(type_closure *, object);		
		ret = gc_new_closure (c->params, c->body, c->env);
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
	old->tag.forw = new;
}

/* strings are also pretty simple. they only store a reference to a passive
 * block of data (the characters) and this gets copied automatically by the
 * constructor
 */
void gc_relocate_string (type_string **new, type_string *old) {
	old->tag.forw = new;
}

/* list cells are more complicated. both the car and cdr may reference
 * other objects. if this has already been moved, as indicated by the broken
 * heart flag, then just use the forwarding address. Otherwise, it's not
 * been moved so call the relocate function on it
 */
void gc_relocate_cell (type_cell **new, type_cell *old) {	
	void **d;
    void *forw;

	old->tag.forw = new;

	if (old->car != NULL) {
        forw = CAST(gc_tag *, cell_car(old))->forw;
		if (forw != NULL) {
			(*new)->car = forw;
		} else {
            (*new)->car = NULL;
			gc_relocate(&((*new)->car), old->car);
		}
	}

	if (old->cdr != NULL) {
        forw = CAST(gc_tag *, cell_cdr(old))->forw;
		if (forw != NULL) {
			(*new)->cdr = forw;
		} else {
            (*new)->cdr = NULL;
			gc_relocate(&((*new)->cdr), old->cdr);
		}
	}
}

/* relocatign a string is the smae as relocating a stirng */
void gc_relocate_symbol (type_symbol **new, type_symbol *old) {
	old->tag.forw = new;
}

/* relocating a double is also easy */
void gc_relocate_double (type_double **new, type_double *old) {
	old->tag.forw = new;
}

/* hash tables are harder to deal with than lists, but thankfully are composed of lists */
void gc_relocate_ht (type_ht **new, type_ht *old) {
	size_t i;
	type_cell *c;

	old->tag.forw = new;
	for(i=0; i < old->size; i++) {
		c = old->buckets[i];
        if (c != NULL) {
            if (c->tag.forw == NULL) {
                (*new)->buckets[i] = NULL;
                gc_relocate((void **)&((*new)->buckets[i]), c);
            } else {
                (*new)->buckets[i] = c->tag.forw;
            }
        }
	}
	(*new)->fill = old->fill;
}

/* arrays are similar to hash tables */
void gc_relocate_array (type_array **new, type_array *old) {
	size_t i;
	void *c;

	old->tag.forw = new;
	for(i=0; i < old->size; i++) {
		c = old->data[i];
        (*new)->data[i] = NULL;
		gc_relocate((void **)&((*new)->data[i]), c);
	}
}

void gc_relocate_proc (type_proc **new, type_proc *old) {
    old->tag.forw = new;
}

void gc_relocate_closure (type_closure **new, type_closure *old) {
    void *forw;

	old->tag.forw = new;

	(*new)->params = NULL;
	(*new)->body = NULL;

    if (old->params != NULL) {
        forw = old->params->tag.forw;
        if (forw) {
            (*new)->params = forw;
        } else {
            gc_relocate ((void **)(&((*new)->params)), old->params);
        }
    }

    if (old->body != NULL) {
        forw = old->body->tag.forw;
        if (forw) {
            (*new)->body = forw;
        } else {
            gc_relocate ((void **)(&((*new)->body)), old->body);
        }
    }
}


/* --------- top level relocate function -------- */

/* relocate an object from the old heap to the new one */
void gc_relocate (void **new, void *old) {
	gc_type type;

	if (*new == NULL) {
		*new = gc_new_copy (old);
	}

	type = get_type(old);
	switch (type) {
    case TYPE_NULL:
        break;
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
    case TYPE_HT:
		gc_relocate_ht ((type_ht **)new, (type_ht *)old);
		break;
	case TYPE_ARRAY:
		gc_relocate_array ((type_array **)new, (type_array *)old);
		break;
    case TYPE_PROC:
		gc_relocate_proc ((type_proc **)new, (type_proc *)old);
		break;
	case TYPE_CLOSURE:
		gc_relocate_closure ((type_closure **)new, (type_closure *)old);
		break;
	}
}

/* copy the root variable and all of its references to the new heap */
void gc_collect (void **root) {
	void *new, *old;

	old = *root;
	
	/* make the new object */
	if (old != NULL) {
		*root = gc_new_copy(old);
		
		gc_relocate(root, old);
	}
}

/*
 * call this to signal the start of a gc cycle. then call gc_collect() on all
 * root variables
 */
void gc_collect_init () {
	void *tmp;
	
	free_p = gc_free;		
	tmp = gc_working;
	gc_working = gc_free;
	gc_free = tmp;
}
