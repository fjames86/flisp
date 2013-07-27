
#ifndef EVAL_H
#define EVAL_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "env.h"
#include "flisp.h"
#include "ht.h"

void *eval (void *expr, environment *env);
void *eval_expr (type_cell *expr, environment *env);
void *eval_exprs (type_cell *exprs, environment *env);
void *eval_let (type_cell *bindings, type_cell *body, environment *env);
void *apply (void *proc, type_cell *args);
void *apply_proc (flisp_proc_t proc, type_cell *args);



#endif 


