
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

#ifndef PROCS_H
#define PROCS_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "array.h"
#include "lists.h"
#include "env.h"
#include "fdlibm.h"

extern void *apply (void *proc, type_cell *args);
extern void load_file (char *fname);
extern void *macroexpand (void *expr, struct environment_ *env);
extern struct environment_ toplevel;

void *proc_car (type_cell *args);
void *proc_cdr (type_cell *args);
void *proc_set_car (type_cell *args);
void *proc_set_cdr (type_cell *args);
void *proc_cons (type_cell *args);
void *proc_list (type_cell *args);
void *proc_reverse (type_cell *args);
void *proc_length (type_cell *args);

void *proc_add (type_cell *args);
void *proc_sub (type_cell *args);
void *proc_mul (type_cell *args);
void *proc_div (type_cell *args);

void *proc_make_array (type_cell *args);
void *proc_aref (type_cell *args);
void *proc_set_aref (type_cell *args);

void *proc_apply (type_cell *args);
void *proc_append (type_cell *args);
void *proc_load (type_cell *args);
void *proc_eq (type_cell *args);
void *proc_eql (type_cell *args);
void *proc_equal (type_cell *args);
void *proc_gt (type_cell *args);
void *proc_gte (type_cell *args);
void *proc_lt (type_cell *args);
void *proc_lte (type_cell *args);
void *proc_e (type_cell *args);

void *proc_macroexpand (type_cell *args);
void *proc_gensym (type_cell *args);
void *proc_typeof (type_cell *args);

void *proc_print (type_cell *args);
void *proc_princ (type_cell *args);
void *proc_format (type_cell *args);
void *proc_toplevel (type_cell *args);
void *proc_gethash (type_cell *args);
void *proc_sethash (type_cell *args);
void *proc_remhash (type_cell *args);
void *proc_heap (type_cell *args);

void *proc_copy_list (type_cell *args);
void *proc_error (type_cell *args);

void *proc_cos (type_cell *args);
void *proc_sin (type_cell *args);
void *proc_tan (type_cell *args);
void *proc_acos (type_cell *args);
void *proc_asin (type_cell *args);
void *proc_atan (type_cell *args);
void *proc_cosh (type_cell *args);
void *proc_sinh (type_cell *args);
void *proc_tanh (type_cell *args);
void *proc_acosh (type_cell *args);
void *proc_asinh (type_cell *args);
void *proc_atanh (type_cell *args);

void *proc_exp (type_cell *args);
void *proc_log (type_cell *args);
void *proc_log10 (type_cell *args);
void *proc_sqrt (type_cell *args);
void *proc_ceil (type_cell *args);
void *proc_fabs (type_cell *args);
void *proc_floor (type_cell *args);
void *proc_erf (type_cell *args);
void *proc_erfc (type_cell *args);
void *proc_gamma (type_cell *args);
void *proc_j0 (type_cell *args);
void *proc_j1 (type_cell *args);
void *proc_jn (type_cell *args);
void *proc_lgamma (type_cell *args);
void *proc_y0 (type_cell *args);
void *proc_y1 (type_cell *args);
void *proc_yn (type_cell *args);

void *proc_assoc (type_cell *args);

#endif 

