
#include "flisp.h"


bool eq (void *val1, void *val2) {
	gc_type t1, t2;
	bool ret = FALSE;

	t1 = get_type(val1);
	t2 = get_type(val2);

	if (t1 == t2) {
		switch (t1) {
		case TYPE_INT:
			ret = (((type_int *)val1)->i == ((type_int *)val2)->i);
			break;
        case TYPE_SYMBOL:
            ret = (CAST(type_symbol *, val1)->sym == CAST(type_symbol *, val2)->sym);
            break;
		default:
			ret = (val1 == val2);
		}
	}
	return ret;
}

bool eql(void *val1, void *val2) {
	gc_type t1, t2;
	bool ret;
	t1 = get_type(val1);
	t2 = get_type(val2);

	if (t1 == t2) {
		switch (t1) {
		case TYPE_INT:
			ret = (((type_int *)val1)->i == ((type_int *)val2)->i);
			break;
		case TYPE_DOUBLE:
			ret = (((type_double *)val1)->d == ((type_double *)val2)->d);
			break;
        case TYPE_SYMBOL:
            ret = (CAST(type_symbol *, val1)->sym == CAST(type_symbol *, val2)->sym);
            break;
		default:
			ret = eq(val1, val2);
		}
	}

	return ret;
}


bool equal(void *val1, void *val2) {
	gc_type t1, t2;
	bool ret;

	t1 = get_type(val1);
	t2 = get_type(val2);

	if ((t1 == TYPE_INT || t1 == TYPE_DOUBLE) && (t2 == TYPE_INT || t2 == TYPE_DOUBLE)) {
		ret = number_equal(val1, val2);
    } else if (t1 == TYPE_SYMBOL && t2 == TYPE_SYMBOL) {
        ret = (CAST(type_symbol *, val1)->sym == CAST(type_symbol *, val2)->sym);
	} else if (t1 == TYPE_STRING && t2 == TYPE_STRING) {
		ret = strcmp(CAST(type_string *, val1)->str, CAST(type_string *, val2)->str);
	} else if (t1 == TYPE_ARRAY && t2 == TYPE_ARRAY) {
		ret = array_equal(val1, val2);
	} else if (t1 == TYPE_CELL && t2 == TYPE_CELL) {
		ret = cell_equal(val1, val2);
	} else {
		ret = eq(val1, val2);
	}
	return ret;
}



bool number_equal (void *val1, void *val2) {
	gc_type t1, t2;
	bool ret;

	t1 = get_type(val1);
	t2 = get_type(val2);

	if (t1 == TYPE_INT && t2 == TYPE_INT) {
		ret = (CAST(type_int *, val1)->i == CAST(type_int *, val2)->i);
	} else if (t1 == TYPE_DOUBLE && t2 == TYPE_DOUBLE) {
		ret = (CAST(type_double *, val1)->d == CAST(type_double *, val2)->d);
	} else if (t1 == TYPE_INT && t2 == TYPE_DOUBLE) {
		ret = ((double)CAST(type_int *, val1)->i == CAST(type_double *, val2)->d);
	} else if (t2 == TYPE_INT && t1 == TYPE_DOUBLE) {
		ret = ((double)CAST(type_int *, val2)->i == CAST(type_double *, val1)->d);
	} else {
		ret = FALSE;
	}
	return ret;
}

bool cell_equal(void *val1, void *val2) {
	gc_type t1, t2;
	bool ret;

	t1 = get_type(val1);
	t2 = get_type(val2);

	if (t1 == TYPE_NULL && t2 == TYPE_NULL) {
		ret = TRUE;
	} else if (t1 != TYPE_CELL || t2 != TYPE_CELL) {
		ret = FALSE;
	} else {
		ret = equal(CAST(type_cell *, val1)->car, CAST(type_cell *, val2)->car) &&
			equal(CAST(type_cell *, val1)->cdr, CAST(type_cell *, val2)->cdr);

	}
	return ret;
}

bool array_equal(void *val1, void *val2) {
	gc_type t1, t2;
	bool ret;
	type_array *a1, *a2;
	size_t i;

	t1 = get_type(val1);
	t2 = get_type(val2);

	if (t1 != TYPE_ARRAY || t2 != TYPE_ARRAY) {
		ret = FALSE;
	} else {
		a1 = CAST(type_array *, val1);
		a2 = CAST(type_array *, val2);

		ret = (a1->size == a2->size);

		for(i=0; i < a1->size; i++) {
			ret = ret && equal(a1->data[i], a2->data[i]);
			if (!ret) {
				break;
			}
		}
	}
	return ret;
}

