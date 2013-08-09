
#include "system.h"


void rebuffer () {
    memset(reader_buffer, '\0', MAX_LINE);
    gets(reader_buffer, MAX_LINE);
    reader_bufferp = reader_buffer;
}

void load_file (char *fname) {
    /* can't load from files */
    error ("Loading files from disk not implemented yet", "LOAD-FILE");
}

