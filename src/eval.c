
#include "eval.h"

void *eval(void *expr, environment *env) {
  gc_type type;
  type_cell *args;
  void *ret, *tmp;
  bool found;

  type = get_type(expr);
  switch (type) {
  case TYPE_NULL:
    ret = NULL;
    break;
  case TYPE_INT:
  case TYPE_DOUBLE:
  case TYPE_STRING:
  case TYPE_ARRAY:
    ret = expr;
    break;
  case TYPE_SYMBOL:
    found = lookup(&ret, expr, env);
    if (!found) {
        /* not found error */
        ret = NULL;
    }
    break;
  case TYPE_CELL:
    ret = eval_expr(CAST(type_cell *, expr), env);
    break;
  }
  return ret;    
}


void *eval_expr(type_cell *expr, environment *env) {
  void *proc, *ret;
  void *name, *val;

  proc = (type_cell *)cell_car((void *)expr);
  expr = (type_cell *)cell_cdr((void *)expr);

  /* go through the special operators */
  if (eq(proc, intern("QUOTE"))) {
    ret = cell_car(expr);
  } else if (eq(proc, intern("DEF"))) {
    /* (def name expr). proc = DEF, exprs = (name expr) */
    name = cell_car(expr);
    expr = cell_cadr(expr);
    val = eval(expr, env);
    sethash(&(env->special), name, val);
    ret = name;
  } else {
      ret = NULL;
  }
  return ret;
}

void *eval_exprs(type_cell *exprs, environment *env) {
  void *ret;

  ret = NULL;
  while (exprs != NULL) {
    ret = eval(exprs->car, env);
    if (exprs->cdr == NULL) {
      break;
    }
    exprs = exprs->cdr;
  }
  return ret;
}

void *apply (void *proc, type_cell *args) {
  return NULL;
}

#if 0
void *apply_proc (type_proc *proc, type_cell *args) {
  while (args != NULL) {
    /* push args->car onto stack */
  }
  call proc->proc;
  return top of stack
}
#endif

