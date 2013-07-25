
#ifndef EVAL_H
#define EVAL_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "env.h"

void *eval(void *expr, environment *env);
void *eval_expr(type_cell *expr, environment *env);
void *eval_exprs(type_cell *exprs, environment *env);
void *apply(void *proc, type_cell *args);

#endif 


