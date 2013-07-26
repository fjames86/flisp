

#include "procs.h"


void *proc_quit (type_cell *args) {
  printf("quitting\n");
  exit(0);
  return NULL;
}

void *proc_add (type_cell *args) {
    int i, iret = 0;
    double d, dret = 0;
    gc_type t = TYPE_INT;
    type_cell *c;

    while (args != NULL) {
        c = args->car;
    
        switch (get_type(c)) {
        case TYPE_INT:
            i = CAST(type_int *, c)->i;
            if (t == TYPE_INT) {
                iret += i;
            } else {
                dret += (double)i;
            }
            break;
        case TYPE_DOUBLE:
            d = CAST(type_double *, c)->d;
            if (t == TYPE_INT) {
                t = TYPE_DOUBLE;
                dret = (double)iret;
                dret += d;
            } else {
                dret += d;
            }
            break;
        default:
            /* error? */
            i = 0;
        }
          
        args = args->cdr;
    }

    if (t == TYPE_INT) {
        return (void *)gc_new_int (iret);
    } else if (t == TYPE_DOUBLE) {
        return (void *)gc_new_double (dret);
    } else {
      return NULL;
    }
}

void *proc_mul (type_cell *args) {
    int i, iret = 1;
    double d, dret = 1;
    gc_type t = TYPE_INT;
    type_cell *c;

    t = TYPE_INT;    
    while (args != NULL) {
        c = args->car;
    
        switch (get_type(c)) {
        case TYPE_INT:
            i = CAST(type_int *, c)->i;
            if (t == TYPE_INT) {
                iret *= i;
            } else if (t == TYPE_DOUBLE) {
                dret *= (double)i;
            }
            break;
        case TYPE_DOUBLE:
            d = (double)CAST(type_double *, c)->d;
            if (t == TYPE_INT) {
                t = TYPE_DOUBLE;
                dret = (double)iret;
                dret *= d;
            } else if (t == TYPE_DOUBLE) {
                dret *= d;
            }
            break;
        default:
            /* error? */
            i = 1;
        }
          
        args = args->cdr;
    }

    if (t == TYPE_INT) {
        return (void *)gc_new_int (iret);
    } else {
        return (void *)gc_new_double (dret);
    }  
}

void *proc_sub (type_cell *args) {
  int i1, i2;
  int d1, d2;
  gc_type t1, t2;
  type_cell *c;
  void *subs;

  i1 = 0;
  d1 = 0.0;

  c = CAST(type_cell *, cell_car(args));
  t1 = get_type(c);
  if (t1 == TYPE_INT) {
    i1 = CAST(type_int *, c)->i;
  } else if (t1 == TYPE_DOUBLE) {
    d1 = CAST(type_double *, c)->d;
  }

  args = cell_cdr(args);
  if (args == NULL) {
    i2 = -i1;
    d2 = -d1;
  } else {
    subs = proc_add((type_cell *)args);
    t2 = get_type(subs);
    if (t2 == TYPE_INT) {
      if (t1 == TYPE_INT) {
        i2 = i1 - CAST(type_int *, subs)->i;
      } else if (t1 == TYPE_DOUBLE) {
        d2 = d1 - (double)CAST(type_int *, subs)->i;
      }      
    } else if (t2 == TYPE_DOUBLE) {
      if (t1 == TYPE_INT) {
        d2 = (double)i1 - CAST(type_double *, subs)->d;
        t1 = TYPE_DOUBLE;
      } else if (t1 == TYPE_DOUBLE) {
        d2 = d1 - CAST(type_double *, subs)->d;
      }
    }
  }

  if (t1 == TYPE_INT) {
    return gc_new_int (i2);
  } else if (t1 == TYPE_DOUBLE){
    return gc_new_double (d2);
  } else {
    return NULL;
  }
}


void *proc_div (type_cell *args) {
  int i1, i2;
  int d1, d2;
  gc_type t1, t2;
  type_cell *c;
  void *subs;

  c = cell_car(args);
  t1 = get_type(c);
  if (t1 == TYPE_INT) {
    i1 = CAST(type_int *, c)->i;
  } else if (t1 == TYPE_DOUBLE) {
    d1 = CAST(type_double *, c)->d;
  }

  subs = proc_mul(cell_cdr(args));
  t2 = get_type(subs);
  if (t2 == TYPE_INT) {
    if (t1 == TYPE_INT) {
      i2 = i1 / CAST(type_int *, subs)->i;
    } else if (t1 == TYPE_DOUBLE) {
      d2 = d1 / (double)CAST(type_int *, subs)->i;
    }      
  } else if (t2 == TYPE_DOUBLE) {
    if (t1 == TYPE_INT) {
      d2 = (double)i1 / CAST(type_double *, subs)->d;
      t1 = TYPE_DOUBLE;
    } else if (t1 == TYPE_DOUBLE) {
      d2 = d1 / CAST(type_double *, subs)->d;
    }
  }

  if (t1 == TYPE_INT) {
    return gc_new_int (i2);
  } else {
    return gc_new_double (d2);
  }
}

