
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

#ifndef LIST_H
#define LIST_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "env.h"

type_cell *cons (void *car, void *cdr);
void *cell_car (type_cell *cell);
void *cell_cdr (type_cell *cell);
void *cell_caar (type_cell *cell);
void *cell_cdar (type_cell *cell);
void *cell_cddr (type_cell *cell);
void *cell_cadr (type_cell *cell);
void cell_push (type_cell **place, void *val);
void *cell_pop (type_cell **place);

void set_car(type_cell *cell, void *val);
void set_cdr(type_cell *cell, void *val);

type_cell *acons (void *key, void *val, type_cell *alist);
void *assoc (void *key, type_cell *alist);
type_cell *mapcar (void *(*proc)(void *), type_cell *args);
type_cell *copy_list (type_cell *l);
type_cell *append (type_cell *lists);
size_t list_length (type_cell *list);

#endif

