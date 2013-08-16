
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


#ifndef ENV_H
#define ENV_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "lists.h"
#include "ht.h"
#include "procs.h"

/* default initial size of special variable hash table */
#define ENV_SIZE           100


#define PI                 3.141592653589793

/* environments store the top level bindings in a hash table
   this makes sense because we are likely to have many bindings here
   local/lexical bindings are stored as a list of assoc lists. each assoc list
   represents a lexical frame
*/

typedef struct environment_ {
  type_ht *special;
  type_cell *lexical;
} environment;

environment toplevel;

void env_init (environment *env);
bool lookup(void **val, type_symbol *sym, environment *env);
environment *extend_env (environment *env, type_cell *syms, type_cell *vals);
environment *rebind_env (environment *env, type_cell *vals);
environment *remove_frame (environment *env);
environment *extend_env_special (environment *env, type_cell *syms, type_cell *vals);
void env_define (environment *env, type_symbol *sym, void *val);
void env_set (environment *env, type_symbol *sym, void *val);

#endif 


