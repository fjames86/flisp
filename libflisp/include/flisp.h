
#ifndef FLISP_H
#define FLISP_H


#include "array.h"
#include "error.h"
#include "ht.h"
#include "procs.h"
#include "symbol.h"
#include "types.h"
#include "env.h"
#include "eval.h"
#include "gc.h"
#include "lists.h"
#include "reader.h"
#include "sys.h"
#include "flisp-core.h"

static char *rebuffer_corep;

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
void format_ (char *fstr, type_cell *args);
void flisp_repl (bool quit);
void load_core ();
void rebuffer_core ();
#endif

