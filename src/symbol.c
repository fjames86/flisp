
#include "symbol.h"

void symbol_init (char *table, size_t size) {
    /* symbol_table is the start of the symbol table, where the strings are actually stored */
    symbol_table =  (char **)(table + size - 1);

    /* symbol_p is the current top of the symbol table, where pointers to all interned symbols are stored */
	symbol_p = symbol_table; 

    /* string_table_p is where we can store the strings */
	string_table = table;
	string_table_p = table;

    /* total size of table */
	symbol_table_size = size;

 	/* intern some symbols to get things going */
    intern_new_symbol("NIL");
    intern_new_symbol("T");
    intern_new_symbol("QUIT");
    intern_new_symbol("HEAP");
}

/* copy the string onto the string_table_p and increment, add a reference at symbol_p */
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

char *lookup_symbol (char *str) {
	char **p;
	
	/* first look up in the symbol table */
	for(p = symbol_table; p > symbol_p; p--) {
		if (strcmp(*p, str) == 0) {
			return *p;
		}
	}
    return NULL;
}


type_symbol *intern (char *str) {
    char *sym;

    sym = lookup_symbol(str);
    if (sym == NULL) {
        sym = intern_new_symbol (str);
    }
    
    return gc_new_symbol(sym);
}

type_symbol *gensym () {
	char str[10];
	size_t len;
	type_symbol *ret;
	
	strcpy (str, "#:GENSYM");	
	len = strlen(str) + 1;
	if ((void *)symbol_p > (void *)(string_table_p + len)) {
		/* add the name to the string table, but don't add it to the symbol table */
		strcpy(string_table_p, str);
		ret = gc_new_symbol (string_table_p);
		string_table_p += len;		
	} else {
		/* out of memory */
		ret = NULL;
	}
	
	return ret;
}
