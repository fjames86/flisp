
#include "env.h"

void env_init(environment *env) {
  env->special = gc_new_ht(ENV_SIZE);
  env->lexical = NULL;
}

bool lookup(void **val, type_symbol *sym, environment *env) {
  bool found;
  void *tmp;

  found = gethash(env->special, sym, val);
  if (found) {
    return TRUE;
  } else {
    tmp = assoc(CAST(void *, sym), env->lexical);
    if (tmp != NULL) {
      *val = CAST(type_cell *, tmp)->cdr;
      return TRUE;
    }
  } 
  
  *val = NULL;
  return FALSE;
}

environment *extend_env (environment *env, type_cell *syms, type_cell *vals) {
  while (syms != NULL && vals != NULL) {
    env->lexical = acons(syms->car, vals->car, env->lexical);
    syms = syms->cdr;
    vals = vals->cdr;
  }

  return env;
}

