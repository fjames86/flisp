


#ifndef ENV_H
#define ENV_H

#include "sys.h"
#include "types.h"
#include "gc.h"
#include "lists.h"
#include "ht.h"
#include "procs.h"

#define ENV_SIZE           20

typedef struct environment_ {
  type_ht *special;
  type_cell *lexical;
} environment;

void env_init (environment *env);
bool lookup(void **val, type_symbol *sym, environment *env);
environment *extend_env (environment *env, type_cell *syms, type_cell *vals);
environment *extend_env_special (environment *env, type_cell *syms, type_cell *vals);
void env_define (environment *env, type_symbol *sym, void *val);

#endif 


