
#ifndef READER_H
#define READER_H


#include "gc.h"


#define MAX_LINE 100
#if 0
FILE *readfile;
#endif
char buffer[MAX_LINE];
char *bufferp;


bool whitespace(char c);
bool digitp (char c);
bool integerp (char *str);
bool doublep (char *str);
int parse_integer (char *str);
double parse_double (char *str);
extern void refresh_buffer();
void next_word (char *dest);
type_cell *read_list ();
void *next_expr();
type_string *read_string();


#endif

