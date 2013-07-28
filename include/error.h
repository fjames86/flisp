
#ifndef ERROR_H
#define ERROR_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "lists.h"

/* errors never persist so relocation routines don't need to be installed into the gc */
typedef struct error_ {
	type_string *message;
	type_string *location;
	struct error_ *next;
} type_error;

static type_error *error_p;

void error (char *message, char *location);
type_error *errors ();
void error_clear ();

#endif

