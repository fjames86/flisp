
/*
 * Copyright (C) 2013 Frank James
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

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

