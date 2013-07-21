
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

typedef struct {
	void *data;
	unsigned char type;

	/* data marks the start of a block of size bytes of data */
	unsigned int size;
	
	/* gc tags. broken heart indicates that this has already been moved
	 forwarding_p points back to the old memory location */
	bool bh;
	void *forw;

} value;

#endif
