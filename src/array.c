
#include "array.h"

void *aref(type_array *arr, size_t i) {
	if (i < arr->size) {
		return arr->data[i];
	} else {
		return NULL;
	}
}

void set_aref(type_array *arr, size_t i, void *val) {
	if (i < arr->size) {		
		arr->data[i] = val;
	} 		
}

