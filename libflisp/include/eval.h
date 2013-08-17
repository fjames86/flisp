
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

#ifndef EVAL_H
#define EVAL_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "env.h"
#include "flisp.h"
#include "ht.h"

struct environment_;

void *eval (void *expr, struct environment_ *env);
void *eval_expr (type_cell *expr, struct environment_ *env);
void *eval_exprs (type_cell *exprs, struct environment_ *env);
void *eval_let (type_cell *bindings, type_cell *body, struct environment_ *env);
void *apply (void *proc, type_cell *args);
void *apply_proc (flisp_proc_t proc, type_cell *args);
void *macroexpand (void *expr, struct environment_ *env);
void *eval_quasiquote (void *expr, struct environment_ *env);
void *eval_do (type_cell *update_exprs, void *test_expr, void *ret_expr, type_cell *body, struct environment_ *env);

#endif 


