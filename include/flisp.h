
#ifndef FLISP_H
#define FLISP_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "error.h"

bool eq(void *val1, void *val2);
bool eql(void *val1, void *val2);
bool equal(void *val1, void *val2);
bool number_equal(void *val1, void *val2);
bool cell_equal(void *val1, void *val2);
bool array_equal(void *val1, void *val2);
double cast_to_double (void *val);
void print_list (type_cell *l);
void print_object (void *obj);
void print_object_nice (void *obj);
void format (type_string *string, type_cell *args);

#endif

