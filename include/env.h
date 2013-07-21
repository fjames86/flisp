
#ifndef ENV_H
#define ENV_H

/* environment contains a pair of the toplevel (special) bindings and an assoc list of locals */
typedef struct {
	value *specials;
	value *lexical;
} env;

#endif
