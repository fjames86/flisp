
/*
 * Copyright (C) 2013 Frank James
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

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

