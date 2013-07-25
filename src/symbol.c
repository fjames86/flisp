
#include "symbol.h"

void symbol_init () {
	type_cell *c, **d;

	d = &symbol_list;
	
    intern("NIL");
    intern("T");
    intern("QUIT");
    intern("HEAP");
}


type_symbol *make_symbol (char *str) {
	return gc_new_symbol (str);
}

type_symbol *intern (char *str) {
	size_t i;
	type_cell *c;
	type_symbol *s;
	
	/* first look up in the symbol table */
	c = symbol_list;
	while (c != NULL) {
      s = cell_car(c);
      if (get_type(s) == TYPE_SYMBOL) {
        if (strcmp(s->sym, str) == 0) {
          return c->car;
        }
      } else {
        printf("non symbol %d\n", *s);
      }
        c = cell_cdr(c);      
	}

	/* not found in the table of interned symbols, so add it */
    s = make_symbol(str);
    symbol_list = cons((void *)s, (void *)symbol_list);
	return s;
}

