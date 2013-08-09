
#ifndef READER_H
#define READER_H


#include "gc.h"


#define MAX_LINE 100
#if 0
FILE *readfile;
#endif
char reader_buffer[MAX_LINE];
char *reader_bufferp;


bool whitespace(char c);
bool specialcharp(char c);
bool digitp (char c);
bool integerp (char *str);
bool doublep (char *str);
int parse_integer (char *str);
double parse_double (char *str);

/* this must be implemented outside the library, so that implementors may
 * e.g. provide for reading from files from a disk rather than stdin
 * in the kernel we can't read from files so can't use fgets */
extern void refresh_buffer();

void next_word (char *dest);
type_cell *read_list ();
void *next_expr();
type_string *read_string();


#endif

