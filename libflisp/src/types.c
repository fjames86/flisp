

#include "types.h"

gc_type get_type (void *val) {
  if (val == NULL) {
    return TYPE_NULL;
  } else {
    return CAST(gc_tag *, val)->type;
  }
}


