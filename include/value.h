
#ifndef VALUE_H
#define VALUE_H

#include "sys.h"

#define TYPE_INT             0
#define TYPE_STRING          1
#define TYPE_SYMBOL          2
#define TYPE_CELL            3
#define TYPE_PROC            4
#define TYPE_CLOSURE         5
#define TYPE_ARRAY           6
#define TYPE_HASH_TABLE      7

typedef struct {
	void *data;
	unsigned int type;

	/* data marks the start of a block of size bytes of data */
	size_t size;
	
	/* gc tags. broken heart indicates that this has already been moved
	 * forw is the forwarding address into the new memory to use to resolve further references
	 */
	bool bh;
	void *forw;

} value;

#endif
