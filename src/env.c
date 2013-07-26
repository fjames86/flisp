
#include "env.h"

void env_init(environment *env) {
  env->special = gc_new_ht(ENV_SIZE);
  env->lexical = NULL;

  env_define (env, intern("NIL"), NULL);
  env_define (env, intern("T"), intern("T"));
  
  env_define (env, intern("QUIT"), gc_new_proc (&proc_quit));
  env_define (env, intern("+"), gc_new_proc (&proc_add));
  env_define (env, intern("-"), gc_new_proc (&proc_sub));
  env_define (env, intern("*"), gc_new_proc (&proc_mul));
  env_define (env, intern("/"), gc_new_proc (&proc_div));
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

environment *extend_env_special (environment *env, type_cell *syms, type_cell *vals) {
  while (syms != NULL && vals != NULL) {
    env_define(env, syms->car, vals->car);
    syms = syms->cdr;
    vals = vals->cdr;
  }

  return env;
}

void env_define (environment *env, type_symbol *sym, void *val) {
  sethash(&(env->special), sym, val);
}



