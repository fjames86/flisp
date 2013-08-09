
#ifndef READER_H
#define READER_H


#include "gc.h"


#define MAX_LINE 100
#if 0
FILE *readfile;
#endif
char reader_buffer[MAX_LINE];
char *reader_bufferp;

#define refresh_buffer() ((*refresh_buffer_f)())
void (*refresh_buffer_f)();


bool whitespace(char c);
bool specialcharp(char c);
bool digitp (char c);
bool integerp (char *str);
bool doublep (char *str);
int parse_integer (char *str);
double parse_double (char *str);

void next_word (char *dest);
type_cell *read_list ();
void *next_expr();
type_string *read_string();


#endif

