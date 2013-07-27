
#include "env.h"

void env_init(environment *env) {
	env->special = gc_new_ht(ENV_SIZE);
	env->lexical = NULL;

	env_define (env, intern("NIL"), NULL);
	env_define (env, intern("T"), intern("T"));

	env_define (env, intern("CAR"), gc_new_proc (&proc_car));
	env_define (env, intern("CDR"), gc_new_proc (&proc_cdr));
	env_define (env, intern("SET-CAR!"), gc_new_proc (&proc_set_car));
	env_define (env, intern("SET-CDR!"), gc_new_proc (&proc_set_cdr));
	env_define (env, intern("CONS"), gc_new_proc (&proc_cons));
	
	env_define (env, intern("QUIT"), gc_new_proc (&proc_quit));
	env_define (env, intern("+"), gc_new_proc (&proc_add));
	env_define (env, intern("-"), gc_new_proc (&proc_sub));
	env_define (env, intern("*"), gc_new_proc (&proc_mul));
	env_define (env, intern("/"), gc_new_proc (&proc_div));
	env_define (env, intern("MAKE-ARRAY"), gc_new_proc (&proc_make_array));
	env_define (env, intern("AREF"), gc_new_proc (&proc_aref));
	env_define (env, intern("SET-AREF!"), gc_new_proc (&proc_set_aref));
}

/* lookup a binding in the environment. search through the lexical frames before trying the toplevel */
bool lookup(void **val, type_symbol *sym, environment *env) {
    bool found;
	void *tmp;
	type_cell *frame;

	
	frame = env->lexical;
	while (frame != NULL) {
		tmp = assoc(CAST(void *, sym), frame->car);
		if (tmp != NULL) {
			*val = CAST(type_cell *, tmp)->cdr;
			return TRUE;
		}
		frame = frame->cdr;
	}
	
	found = gethash(env->special, sym, val);
	return found;
}

environment *extend_env (environment *env, type_cell *syms, type_cell *vals) {
	type_cell *frame = NULL;
	while (syms != NULL && vals != NULL) {
		frame = acons(syms->car, vals->car, frame);
		syms = syms->cdr;
		vals = vals->cdr;
	}
	/* push the frame onto the lexical bindings */
	cell_push (&(env->lexical), frame);
	
	/*	env->lexical = cons(frame, env->lexical); */
	return env;
}

environment *remove_frame (environment *env) {
	cell_pop (&(env->lexical));
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

void env_set (environment *env, type_symbol *sym, void *val) {
	/* lookup in lexical vars first, then try special */
	type_cell *frame, *c, *binding;
	
	frame = env->lexical;
	while (frame != NULL) {
		c = cell_car(frame);
		while (c != NULL) {
			/* c = ((sym . val) ...) c->car = (sym . val) */
			binding = cell_car(c);
			if (eq(cell_car(binding), sym)) {
				binding->cdr = val;
				return;
			}
			c = c->cdr;
		}
		frame = frame->cdr;
	}

	/* not found in lexical frames, set at top level */
	sethash(&(env->special), sym, val);
}


