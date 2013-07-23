

#ifndef TYPES_H
#define TYPES_H

#include "sys.h"

#define TYPE_INT 0
#define TYPE_STRING 1
#define TYPE_CELL 2
#define TYPE_SYMBOL 3
#define TYPE_DOUBLE 4

typedef unsigned int gc_type;


/* garbage collector tag that all types must contain */
typedef struct {
	gc_type type;
	bool bh;
	void *forw;
} gc_tag;


/* the different types follow */

typedef struct {
	gc_tag tag;
	int i;
} type_int;

typedef struct {
	gc_tag tag;
	char *str;
	size_t len;
} type_string;

typedef struct {
	gc_tag tag;
	void *car;
	void *cdr;
} type_cell;

typedef struct {
	gc_tag tag;
	char *sym;
} type_symbol;

typedef struct {
	gc_tag tag;
	double d;
} type_double;


#endif

