
/*
 * Copyright (C) 2013 Frank James
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

#include "eval.h"

/*
 * evaluation proceeds as follows:
 * 1. pass the input form to macroexpand. This promises to macro expand the toplevel of the form 
 *    until the car position is not a macro any longer.
 * 2. The result of macroexpand is passed to eval_expr which actually does the evaluation.
 *
 * Note that this means macro expansion happens every time a form is evaluated. This is bad news in
 * the body of lambda expressions, that might be evaluated multiple times. This means we must first macro
 * expand the body before passing it to gc_new_closure().
 */


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
		found = lookup (&ret, expr, env);
		if (!found) {
			/* not found error */
			error("Variable not found", "EVAL");
			ret = NULL;
		}
		break;
	case TYPE_CELL:
		ret = eval_expr (CAST (type_cell *, expr), env);
		break;
	default:
		/* unknown type ? */
		error ("Unknown type", "EVAL");
		ret = expr;
	}
	return ret;    
}


void *eval_expr(type_cell *expr, environment *env) {
	void *proc, *ret, *bindings, *params, *body;
	void *name, *val;
	type_cell *args, **c, *orig;

	if (expr == NULL) {
		return NULL;
	}

	orig = expr;
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
			/*			body = macroexpand (cell_cdr(expr), env);*/
			body = cell_cdr (expr);
			sethash(&(env->special), name, gc_new_closure (params, body, env));
			break;
        default:
            error ("Must define either a symbol or a closure", "EVAL-EXPR");
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
	} else if (eq(proc, intern ("QUASIQUOTE"))) {
		ret = eval_quasiquote (cell_car (expr), env);
	} else if (eq(proc, intern ("UNQUOTE"))) {
		error ("Unquote outside a quasiquote", "EVAL-EXPR");
		ret = NULL;
	} else if (eq(proc, intern ("UNQUOTE-SPLICING"))) {
		error ("Unquote-splicing outside a quasiquote", "EVAL-EXPR");
		ret = NULL;
	} else {
		/* procedure application. could be either a closure or primitive proc */
		proc = eval(proc, env);

		if (get_type(proc) == TYPE_CELL && eq(cell_car(proc), intern("MACRO"))) {
			/* runtime macro expansion */
			orig = macroexpand (orig, env);
			ret = eval (orig, env);
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

/* allow recursion looping by rebinding and jumping back to the start */
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

/* macroexpansion works as follows:
 * atoms. left unevaluated
 * lists. evaluate the object in the car position. if it returns (macro <proc>) then apply the <proc> to the cdr of the list
 *        otherwise build up a new list by expanding each element in the list
 */


/*
 * macroexpansion works by expanding the top. but it must not expand inside a quote expression
 * quasiquote expressions should be handled like the evaluation rules of eval-quasiquote
 */
void *macroexpand (void *expr, environment *env) {
	gc_type t;
	void *ret;
	type_cell *c, **builder;
	void *mac;
	bool found;
		
	t = get_type (expr);
	if (t == TYPE_CELL) {
		if (eq(cell_car (expr), intern("QUOTE"))) {
			/* quote expression, just return the expr itself */
			ret = expr;
		} else if (eq(cell_car (expr), intern("QUASIQUOTE"))) {
			/* if the expression to be spliced is a form (list) then macro expand that */
			ret = NULL;
			builder = (type_cell **)&ret;
			c = cell_cadr (expr);
			if (get_type (c) == TYPE_CELL) {
				while (c != NULL) {
					*builder = cons (macroexpand (cell_car (c), env), NULL);
					builder = (type_cell **)&((*builder)->cdr);
					c = c->cdr;
				} 
			} else {
				*builder = cell_cadr (expr);
			}
			ret = cons (ret, NULL);
			ret = cons (intern ("QUASIQUOTE"), ret);
		} else {
			/* macro? */
			c = cell_car (expr);
			mac = NULL;
			found = FALSE;
			if (get_type (c) == TYPE_SYMBOL) {
				found = lookup (&mac, cell_car (expr), env);
			}

			if (found && mac != NULL && get_type (mac) == TYPE_CELL && eq(cell_car (mac), intern("MACRO"))) {
				/* a macro, apply the cadr to the rest */
				ret = apply (cell_cadr(mac), cell_cdr (expr));
			} else {
				/* build up the list by macroexpanding each term in it */
				c = CAST(type_cell *, expr);
				ret = NULL;
				builder = (type_cell **)&ret;
				while (c != NULL) {
					if (get_type (c) == TYPE_CELL) {
						*builder = cons (macroexpand (cell_car (c), env), NULL);
						builder = (type_cell **)&((*builder)->cdr);
						c = c->cdr;
					} else {
						*builder = macroexpand (c, env);
						break;
					}
				}
			}
		}
	} else {
		ret = expr;
	}

	return ret;
}


/*
 * (quasiquote <atom>) -> <atom>
 * (quasiquote (a b c (unquote d))) -> (a b c (eval d))
 * (quasiquote (unquote a)) -> (eval a)
 * (quasiquote (((... (unquote a))))) -> (((... (eval a))))
 * (quasiquote (quote (unquote a))) -> (quote (eval a))
 */
void *eval_quasiquote (void *expr, environment *env) {
	gc_type t;
	type_cell *c, **builder;
	void *ret, *car;
	
	t = get_type(expr);
	if (t == TYPE_CELL) {
		c = CAST (type_cell *, expr);
		if (eq (cell_car(c), intern("UNQUOTE"))) {
			ret = eval (cell_cadr (c), env);
		} else if (eq (cell_car(c), intern ("UNQUOTE-SPLICING"))) {
			error ("Can't splice when not in a list", "EVAL-QUASIQUOTE");
			ret = NULL;
		} else {
			ret = NULL;
			builder = (type_cell **)&ret;
			while (c != NULL) {
				if (get_type (c) == TYPE_CELL) {
					car = cell_car(c);				
					t = get_type(car);
					if (t == TYPE_CELL) {
						if (eq(cell_car(car), intern("UNQUOTE"))) {
							*builder = cons (eval (cell_cadr(car), env), NULL);
							builder = (type_cell **)&((*builder)->cdr);
						} else if (eq(cell_car(car), intern("UNQUOTE-SPLICING"))) {
							/* unquote-splicing needs to be non-destructive */
							*builder = copy_list (eval (cell_cadr (car), env));
							while (*builder != NULL) {
								builder = (type_cell **)&((*builder)->cdr);
							}							
						} else {
							/* a new sub list. need to recursively call quasiquote on this */
							*builder = cons (eval_quasiquote (car, env), NULL);
							builder = (type_cell **)&((*builder)->cdr);
						}					
					} else {
						*builder = cons (car, NULL);
						builder = (type_cell **)&((*builder)->cdr);
					}
					c = c->cdr;
				} else {
					/* dotted list? */
					*builder = c;
					break;
				}
			}
		}
	} else {
		ret = expr;
	}
	return ret;
}
