
#include "symbol.h"

void symbol_init (char *table, size_t size) {
	symbol_table = table + size;
	symbol_p = (char **)(table + size);
	string_table_p = table;
	symbol_table_size = size;

 	/* intern some symbols to get things going */
    intern("NIL");
    intern("T");
    intern("QUIT");
    intern("HEAP");
}

char *intern_new_symbol (char *str) {
	char *ret;
	size_t len = strlen(str) + 1;

	if ((void *)symbol_p > (void *)(string_table_p + len)) {
		ret = string_table_p;
		*symbol_p = string_table_p;
		symbol_p--;
		strcpy(string_table_p, str);
		string_table_p += len;
	} else {
		/* out of memory */
		ret = NULL;
	}
	
	return ret;
}

char *intern (char *str) {
	type_cell *c;
	char *s;
	char **p;
	
	/* first look up in the symbol table */
	for(p = (char **)symbol_p + 1; (void *)p <= (void *)symbol_table; p++) {
		if (strcmp(*p, str) == 0) {
			return *p;
		}
	}

	/* not found in the table of interned symbols, so add it */
	return intern_new_symbol (str);
}

