

#include "reader.h"


bool whitespace(char c) {
	switch (c) {
	case ' ':
	case '\t':
	case '\f':
	case '\n':
	case '\v':
		return TRUE;
	default:
		return FALSE;
	}
}

bool digitp (char c) {
	if (c >= '0' && c <= '9') {
		return TRUE;
	} else {
		return FALSE;
	}
}
	
bool integerp (char *str) {
	char *c = str;
	int d = 0;

	/* first char may be a +, - or of course a digit */
	if(!(*c == '+' || *c == '-' || digitp(*c))) return 0;

	if (digitp(*c)) d = 1;
	
	c++;
	while(*c != '\0') {
		if(!digitp(*c)) return 0;
		else d = 1;
		c++;
	}

	return d;
}

int parse_integer (char *str) {
	char *c;
	int ret, factor;

	c = str;
	while (*c != '\0') c++;
	c--;

	factor = 1;
	ret = 0;
	while (c >= str) {
		if (*c == '-') {
			ret = -ret;
			break;
		} else if (*c == '+') {
			break;
		}
		
		ret += (*c - '0')*factor;
		factor *= 10;
		c--;
	}

	return ret;
}

double parse_double (char* str){
	double res = 0.0, factor = 1.0;
	int point_seen, d, exp;
	
 	if (*str == '-') {
		str++;
		factor = -1.0;
	}
	
	for (point_seen = 0; *str; str++) {
		if (*str == '.') {
			point_seen = 1;
			continue;
		} else if (*str == 'e' || *str == 'E') {
			str++;
			exp = parse_integer(str);
			if (exp > 0) {
				for(d = 0; d < exp; d++) {
					factor *= 10.0;
				}				
			} else {
				for (d=0; d < -exp; d++) {
					factor /= 10.0;
				}
			}
			break;
		}
		
		d = *str - '0';
		if (d >= 0 && d <= 9){
			if (point_seen) {
				factor /= 10.0;
			}
			res = res * 10.0 + (double)d;
		}
	}
	
	return res * factor;
}

bool doublep (char *str) {
	char *c = str;
	bool point = FALSE;
	bool exp = FALSE;
	
	while (*c == ' ') c++;

	if(!(*c == '+' || *c == '-' || *c == '.' || digitp(*c))) return FALSE;

	if(*c == '.') point = TRUE;
	
	c++;
	while(*c != '\0') {
		/* point must occur before exp */
		if(*c == '.') {
			if(exp == 0) point = TRUE;
			else return FALSE;
		} else if(*c == 'e' || *c == 'E') {
			if(exp == FALSE) {
				c++;
				return integerp (c);
			} else {
				return FALSE;
			}
		} else if (!digitp(*c)) {
			return FALSE;
		} 
		c++;
	}
	if(point == TRUE)
		return TRUE;
	else
		return FALSE;
}

#if 0
void refresh_buffer() {
	/*gets(buffer);*/

   	if (feof(readfile) != 0) {
		/* this is such a hack */
		strcpy(buffer, "NIL");
	} else {
		fgets(buffer, MAX_LINE, readfile);
	}			

	bufferp = buffer;
}
#endif

void next_word (char *dest) {
	bool done;
	
	/* refresh the buffer if needed */
	while (*bufferp == '\0') {
		refresh_buffer();
	}

	while (whitespace(*bufferp)) {
		bufferp++;
	}

	if (*bufferp == '\0' || *bufferp == '\n') {
		return next_word(dest);
	}
	
	done = FALSE;
	while (TRUE) {
		if (whitespace(*bufferp) || *bufferp == '\0') {
			break;
		} else if (*bufferp == '(' || *bufferp == ')' || *bufferp == '"' || *bufferp == '\'' || *bufferp == ';' || *bufferp == ',' || *bufferp == '@' || *bufferp == '`') {
			if (!done) {
				*dest = *bufferp;
				dest++;
				bufferp++;
			}
			break;
		}

		*dest = *bufferp;
		done = TRUE;
		dest++;
		bufferp++;
	}
	*dest = '\0';
}

type_cell *read_list () {
	char word[MAX_LINE];
	type_cell **builder;
	bool first = TRUE;
	type_cell *top, *cell;
	
	top = NULL;
	builder = &top;
	while (TRUE) {
		next_word(word);
        
		if (strcmp (word, "(") == 0) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
			
			(*builder)->car = read_list();
			builder = (type_cell **)(&((*builder)->cdr));
		} else if (strcmp (word, ")") == 0) {
			*builder = NULL;
			break;
		} else if (strcmp (word, ".") == 0) {
			/* dotted list */
			if (first == TRUE) {
				/* error, dotted list not at end of list */
				error ("Dotted list not at end of list", "READ-LIST");
				top = NULL;
				break;
			} else {
				*builder = next_expr();
				next_word(word);
				if (strcmp(word, ")") != 0) {
					error ("Dotted list not at end of list", "READ-LIST");
					top = NULL;
				}
				break;
			}
        } else if (strcmp (word, "'") == 0) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
            cell = cons(next_expr(), NULL);
            (*builder)->car = cons(intern("QUOTE"), cell);
			builder = (type_cell **)(&((*builder)->cdr));
		} else if (strcmp (word, "`") == 0) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
            cell = cons(next_expr(), NULL);
            (*builder)->car = cons(intern("QUASIQUOTE"), cell);
			builder = (type_cell **)(&((*builder)->cdr));
		} else if (strcmp (word, ",") == 0) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
            cell = cons(next_expr(), NULL);
            (*builder)->car = cons(intern("UNQUOTE"), cell);
			builder = (type_cell **)(&((*builder)->cdr));
		} else if (strcmp (word, "@") == 0) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
            cell = cons(next_expr(), NULL);
            (*builder)->car = cons(intern("UNQUOTE-SPLICING"), cell);
			builder = (type_cell **)(&((*builder)->cdr));
		} else if (strcmp (word, "\"") == 0) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
			(*builder)->car = read_string();
			builder = (type_cell **)(&((*builder)->cdr));
		} else if (strcmp (word, ";") == 0) {
			/* comment */
			refresh_buffer();
		} else if (integerp (word) == TRUE) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
			(*builder)->car = gc_new_int (parse_integer (word));
			builder = (type_cell **)(&((*builder)->cdr));
		} else if (doublep (word) == TRUE) {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
			(*builder)->car = gc_new_double (parse_double (word));
			builder = (type_cell **)(&((*builder)->cdr));
		} else {
			if (first == TRUE) {
				top = gc_new_cell();
				builder = &top;
				first = FALSE;
			} else {
				*builder = gc_new_cell();
			}
			string_upcase(word);
			(*builder)->car = intern (word);
			builder = (type_cell **)(&((*builder)->cdr));
		}
	}
	
	return top;
}

/* read until an unescaped " char is encountered. */
type_string *read_string () {
	char *strbuff1, *strbuff2;
	size_t i;
	size_t buffsize = MAX_LINE;
	type_string *ret;
	bool escape;
	
	strbuff1 = gc_malloc(buffsize);
	i = 0;
	escape = FALSE;
	while (TRUE) {
		if (escape == TRUE) {
			switch (*bufferp) {
			case '\0':
				strbuff1[i] = '\n';
				/* this is the end of line, so refresh */
				refresh_buffer();
			    break;
			case 'n':
				strbuff1[i] = '\n';
				bufferp++;
				break;
			case 't':
				strbuff1[i] = '\t';
				bufferp++;
				break;
			case 'f':
				strbuff1[i] = '\f';
				bufferp++;
				break;
			case 'r':
				strbuff1[i] = '\r';
				bufferp++;
				break;
			case 'v':
				strbuff1[i] = '\v';
				bufferp++;
				break;
			case 'b':
				strbuff1[i] = '\b';
				bufferp++;
				break;
			case '\\':
				strbuff1[i] = '\\';
				bufferp++;
				break;
			case '"':
				strbuff1[i] = '"';
				bufferp++;
				break;
			default:
				/* not an official escape char, so just ignore it */
				strbuff1[i] = *bufferp;
				bufferp++;
			}
			i++;
			escape = FALSE;
		} else if (*bufferp == '\0') {
			/* end of line without an escape, error */
			ret = NULL;
			error ("Unterminated string detected", "READ-STRING");
			bufferp++;
			break;
		} else if (*bufferp == '"') {
			strbuff1[i] = '\0';
			bufferp++;			
			break;
		} else if (*bufferp == '\\') {
			escape = TRUE;
			bufferp++;
		} else {
			/* put the char into the buffer */
			strbuff1[i] = *bufferp;
			bufferp++;
			i++;

			escape = FALSE;
		}
						
		/* need to increase the buffer and copy across */
		if (i >= buffsize) {
			buffsize += MAX_LINE;
			strbuff2 = gc_malloc(buffsize);
			strcpy(strbuff2, strbuff1);
			strbuff1 = strbuff2;
		}
	}

	ret = gc_new_string(strbuff1);
	return ret;
}

void *next_expr() {
	char word[MAX_LINE];
	void *ret;
	type_cell *builder;
	void *next;
	
	next_word(word);
	
	if (strcmp(word, "(") == 0) {
		/* start of a list */
		ret = read_list();
	} else if (strcmp (word, ")") == 0) {
		/* end of a list. shoulnd't happen here */
		error ("Unmatched closing paren", "NEXT-EXPR");
		ret = NULL;
	} else if (strcmp (word, ".") == 0) {
		/* dotted end of list. shouldn't happen here */
		error ("Dotted list not in list", "NEXT-EXPR");
		return NULL;
    } else if (strcmp (word, "'") == 0) {
        /* quote read macro */
        builder = cons(next_expr(), NULL);
        builder = cons(intern("QUOTE"), builder);
        ret = builder;
	} else if (strcmp (word, "`") == 0) {
        /* quote read macro */
        builder = cons(next_expr(), NULL);
        builder = cons(intern("QUASIQUOTE"), builder);
        ret = builder;
	} else if (strcmp (word, ",") == 0) {
        /* quote read macro */
        builder = cons(next_expr(), NULL);
        builder = cons(intern("UNQUOTE"), builder);
        ret = builder;
	} else if (strcmp (word, "@") == 0) {
        /* quote read macro */
        builder = cons(next_expr(), NULL);
        builder = cons(intern("UNQUOTE-SPLICING"), builder);
        ret = builder;
	} else if (strcmp (word, "\"") == 0) {
		/* quote, read string */
		ret = read_string();
	} else if (strcmp (word, ";") == 0) {
		/* comment */
		refresh_buffer();
		ret = next_expr();
	} else if (integerp (word) == TRUE) {
		/* integer */
		ret = (void *)gc_new_int (parse_integer (word));
	} else if (doublep (word) == TRUE) {
		/* double */
		ret = (void *)gc_new_double (parse_double (word));
	} else {
		/* if all else fails, intern a ssyobl */
		string_upcase(word);		
		ret = (void *)intern(word);
	}

	return ret;
}


