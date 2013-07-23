
#ifndef SYMBOL_H
#define SYMBOL_H

#include "sys.h"
#include "types.h"
#include "gc.h"

type_cell *symbol_list;

void symbol_init ();
type_symbol *intern (char *str);
type_symbol *make_symbol (char *str);

#endif

