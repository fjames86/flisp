
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

#ifndef SYS_H
#define SYS_H

#ifndef NULL
#define NULL ((void *)0x00000000)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define abs(x)    (((x) < 0) ? (-(x)) : (x))

typedef unsigned int bool;
typedef unsigned int size_t;

extern void putch (unsigned char c);
extern char getch ();


void *memcpy(void *dest, void *src, int count);
void *memset(void *dest, unsigned char val, int count);
char *strcpy (char *dest, char *source);
char *strncpy (char *dest, char *source, size_t num);
int strcmp(char *str1, char *str2);
int strncmp(char *str1, char *str2, size_t num);
int strlen(char *str);
char *string_upcase(char *string);

void print_int (int i);
void print_hex (size_t i);
void print_hexl (unsigned long l);
void print_double (double x, int p);
void print_string (char *str);

#endif

