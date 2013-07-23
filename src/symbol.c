
#include "symbol.h"

void symbol_init () {
	type_cell *c, **d;

	d = &symbol_list;
	
	c = gc_new_cell();
	c->car = make_symbol("NIL");
	*d = c;

	c = gc_new_cell();
	c->car = make_symbol("T");
	d = (type_cell **)&((*d)->cdr);
	*d = c;

	c = gc_new_cell();
	c->car = make_symbol("QUIT");
	d = (type_cell **)&((*d)->cdr);	
	*d = c;
}

type_symbol *make_symbol (char *str) {
	return gc_new_symbol (str);
}

type_symbol *intern (char *str) {
	char *start;
	size_t i;
	type_cell **c;
	type_symbol *s;
	
	/* first look up in the symbol table */
	c = &symbol_list;
	while (*c != NULL) {
		start = (char *)((type_symbol *)((*c)->car))->sym;
		if (strcmp(start, str) == 0) {
			return (*c)->car;
		}
		c = (type_cell **)&((*c)->cdr);
	}

	/* not found in the table of interned symbols, so add it */
	*c = gc_new_cell();
	s = make_symbol (str);
	(*c)->car = s;
	return s;
}

