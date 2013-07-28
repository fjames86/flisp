
#include "error.h"

type_error *errors () {
	return error_p;
}

void error (char *message, char *location) {
	type_error *e = gc_malloc (sizeof(type_error));
	if (message != NULL) {
		e->message = gc_new_string(message);
	} else {
		e->message = NULL;
	}

	if (location != NULL) {
		e->location = gc_new_string(location);
	} else {
		e->location = NULL;
	}
	
	e->next = error_p;
	error_p = e;
}

void error_clear () {
	error_p = NULL;
}


