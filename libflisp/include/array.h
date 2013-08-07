
#ifndef ARRAY_H
#define ARRAY_H

#include "sys.h"
#include "types.h"
#include "gc.h"

void *aref(type_array *arr, size_t i);
void set_aref(type_array *arr, size_t i, void *val);

#endif

