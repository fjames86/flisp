
#ifndef FLISP_H
#define FLISP_H

#include "sys.h"
#include "types.h"
#include "gc.h"

bool eq(void *val1, void *val2);
bool eql(void *val1, void *val2);
bool equal(void *val1, void *val2);
bool number_equal(void *val1, void *val2);
bool cell_equal(void *val1, void *val2);
bool array_equal(void *val1, void *val2);

#endif

