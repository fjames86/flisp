
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
	case TYPE_PROC:
	case TYPE_CLOSURE:
		ret = expr;
		break;
	case TYPE_SYMBOL:
		found = lookup(&ret, expr, env);
		if (!found) {
			/* not found error */
			error("Variable not found", "EVAL");
			ret = NULL;
		}
		break;
	case TYPE_CELL:
		ret = eval_expr(CAST(type_cell *, expr), env);
		break;
	default:
		/* unknown type ? */
		error ("Unknown type", "EVAl");
		ret = expr;
	}
	return ret;    
}


void *eval_expr(type_cell *expr, environment *env) {
	void *proc, *ret, *bindings, *params, *body;
	void *name, *val;
	type_cell *args, **c;

	if (expr == NULL) {
		return NULL;
	}
	
	proc = cell_car((void *)expr);
	expr = (type_cell *)cell_cdr((void *)expr);

	/* go through the special operators */
	if (eq(proc, intern("QUOTE"))) {
		ret = cell_car(expr);
	} else if (eq(proc, intern("BEGIN"))) {
		/* (begin name body) */
		ret = eval_exprs (expr, env);
	} else if (eq(proc, intern("DEFINE"))) {
		/* (define name expr) or (define (name args) body), */
		name = cell_car(expr);
		switch (get_type(name)) {
		case TYPE_SYMBOL:
			expr = cell_cadr(expr);
			val = eval(expr, env);
			sethash(&(env->special), name, val);
			break;
		case TYPE_CELL:
			params = cell_cdr(name);
			name = cell_car(name);
			body = macroexpand (cell_cdr(expr), env);
			sethash(&(env->special), name, gc_new_closure (params, body, env));
			break;
		} 
		ret = name;
	} else if (eq(proc, intern("DEFMACRO"))) {
		/* (defmacro (name . args) . body) */
		name = cell_car(expr);
		params = cell_cdr(name);
		name = cell_car(name);
		body = cell_cdr(expr);
		val = cons (intern ("MACRO"), cons (gc_new_closure (params, body, env), NULL));
		
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
	} else if (eq(proc, intern("LAMBDA"))) {
		/* (lambda params body) */
		params = CAST(type_cell *, cell_car(expr));
		body = CAST(type_cell *, cell_cdr(expr));
		ret = gc_new_closure (params, body, env);
	} else {
		/* procedure application. could be either a closure or primitive proc */
		proc = eval(proc, env);

		if (get_type(proc) == TYPE_CELL && eq(cell_car(proc), intern("MACRO"))) {
			/* runtime macro expansion */
			ret = eval(apply(cell_cadr(proc), expr), env);
		} else {
			args = NULL;
			c = &args;
			while (expr != NULL) {
				if (get_type(expr) != TYPE_CELL) {
					/* dotted list ? not allowed */
					error("Must be a proper list", "EVAL-EXPR");
					return NULL;
				}
				
				(*c) = cons(eval(cell_car(expr), env), NULL);
				c = (type_cell **)&((*c)->cdr);
				
				expr = expr->cdr;
			}
			
			ret = apply (proc, args);
		}
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
			error ("Incorrectly formatted let binding", "EVAL-LET");
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
	type_closure *c;
	
	switch (t) {
	case TYPE_PROC:
		ret = apply_proc(CAST(type_proc *, proc)->proc, args);
		break;
	case TYPE_CLOSURE:
		/* evaluate the body in the environment packed with the cosure */
		c = CAST(type_closure *, proc);
		extend_env (c->env, c->params, args);		
		ret = eval_exprs (c->body, c->env);
		remove_frame (c->env);
		break;
	default :
		/* error */
		error ("Can only apply a procedure or closure", "APPLY");
		ret = NULL;
	}
	
	return ret;
}

void *apply_proc (flisp_proc_t proc, type_cell *args) {
	return (proc)(args);
}

void *macroexpand_1 (void *expr, environment *env) {
	gc_type t;
	void *ret, *tmp;
	type_cell *c;
	
	t = get_type (expr);

	if (t == TYPE_CELL) {
		c = CAST(type_cell *, expr);
		tmp = eval (cell_car(c), env);
		if (get_type(tmp) == TYPE_CELL && eq(cell_car(tmp), intern("MACRO"))) {
			ret = macroexpand (apply (cell_cadr(tmp), cell_cdr(expr)), env);
		} else {
			ret = expr;
		}
	} else {
		ret = expr;
	}
	return ret;
}

void *macroexpand (void *expr, environment *env) {
	gc_type t;
	type_cell *cell, **c;
	void *ret;
	
	expr = macroexpand_1 (expr, env);
	
	t = get_type(expr);
	if (t == TYPE_CELL) {
		cell = CAST(type_cell *, expr);
		ret = NULL;
		c = (type_cell **)&ret;
		while (cell != NULL) {
			*c = macroexpand_1 (cell, env);
			c = (type_cell **)&((*c)->cdr);
			cell = cell_cdr(cell);
		}		
	} else {
		ret = expr;
	}
	return ret;
}


