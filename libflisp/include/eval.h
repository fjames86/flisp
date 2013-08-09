
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

#endif 


