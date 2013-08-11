
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

#ifndef SYMBOL_H
#define SYMBOL_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "lists.h"

/* symbols must not be garbage collected, so must live outside the heap */

/* store pointers to the strings from the top of the stack */
char **symbol_table;
char **symbol_p;

/* store the strings from the bottom */
char *string_table;
char *string_table_p;
size_t symbol_table_size;

void symbol_init (char *table, size_t size);
char *intern_new_symbol (char *str);
char *lookup_symbol (char *str);
type_symbol *intern (char *str);
type_symbol *gensym ();

#endif

