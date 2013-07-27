
#include "eval.h"

void *eval(void *expr, environment *env) {
	gc_type type;
	type_cell *args;
	void *ret, *tmp;
	bool found;

	type = get_type(expr);
	switch (type) {
	case TYPE_NULL:
	case TYPE_INT:
	case TYPE_DOUBLE:
	case TYPE_STRING:
	case TYPE_ARRAY:
	case TYPE_HT:
		ret = expr;
		break;
	case TYPE_SYMBOL:
		found = lookup(&ret, expr, env);
		if (!found) {
			/* not found error */
			ret = NULL;
		}
		break;
	case TYPE_CELL:
		ret = eval_expr(CAST(type_cell *, expr), env);
		break;
	default:
		/* unknown type ? */
		ret = expr;
	}
	return ret;    
}


void *eval_expr(type_cell *expr, environment *env) {
	void *proc, *ret, *bindings, *body;
	void *name, *val;
	type_cell *args, **c;

	proc = cell_car((void *)expr);
	expr = (type_cell *)cell_cdr((void *)expr);

	/* go through the special operators */
	if (eq(proc, intern("QUOTE"))) {
		ret = cell_car(expr);
	} else if (eq(proc, intern("DEF"))) {
		/* (def name expr). proc = DEF, exprs = (name expr) */
		name = cell_car(expr);
		expr = cell_cadr(expr);
		val = eval(expr, env);
		sethash(&(env->special), name, val);
		ret = name;
	} else if (eq(proc, intern("SET!"))) {
		/* (set! var val) */
		name = cell_car(expr);
		expr = cell_cadr(expr);
		val = eval(expr, env);
		env_set(env, name, val);
		ret = name;
	} else if (eq(proc, intern("IF"))) {
		/* (if test then else) */
		name = eval(cell_car(expr), env);
		if (name != NULL) {
			expr = cell_cdr(expr);
			ret = eval(cell_car(expr), env);
		} else {
			expr = cell_cddr(expr);
			ret = eval(cell_car(expr), env);
		}
	} else if (eq(proc, intern("LET"))) {
		/* (let bindings body) */
		bindings = cell_car(expr);
		body = cell_cdr(expr);
		ret = eval_let (bindings, body, env);
	} else {
		/* procedure application. could be either a closure or primitive proc */
		args = NULL;
		c = &args;
		while (expr != NULL) {
			(*c) = cons(eval(expr->car, env), NULL);
			c = (type_cell **)&((*c)->cdr);

			expr = expr->cdr;
		}

		ret = apply (eval(proc, env), args); 
	}
	return ret;
}

void *eval_exprs(type_cell *exprs, environment *env) {
	void *ret;

	ret = NULL;
	while (exprs != NULL) {
		ret = eval(cell_car(exprs), env);
		exprs = exprs->cdr;
	}
	return ret;
}

void *eval_let (type_cell *bindings, type_cell *body, environment *env) {
	type_cell *syms, *vals;
	void *ret, *binding;
	
	syms = vals = NULL;
	while (bindings != NULL) {
		/* bindings->car = (name expr) or name */
		binding = cell_car(bindings);
		
		switch (get_type(binding)) {
		case TYPE_SYMBOL:
			syms = cons(binding, syms);
			vals = cons(NULL, vals);
			break;
		case TYPE_CELL:
			syms = cons(cell_car(binding), syms);
			vals = cons(eval(cell_cadr(binding), env), vals);
			break;
		default:
			/* error ? */
			return NULL;
		}
		
		bindings = bindings->cdr;
	}
	
	extend_env (env, syms, vals);
	ret = eval_exprs(body, env);
	remove_frame (env);
	return ret;
}



void *apply (void *proc, type_cell *args) {
	void *ret;
	gc_type t = get_type (proc);

	switch (t) {
	case TYPE_PROC:
		ret = apply_proc(CAST(type_proc *, proc)->proc, args);
		break;
	case TYPE_CELL:        
		/* closure */
		ret = NULL;
		break;
	default :
		/* error */
		ret = NULL;
	}

	return ret;
}

void *apply_proc (flisp_proc_t proc, type_cell *args) {
	return (proc)(args);
}


