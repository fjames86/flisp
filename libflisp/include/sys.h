
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
#if 0
void *memcpy (void *dest, void *start, size_t num);
void *memset (void *dest, int val, size_t num);
#endif



char *strcpy (char *dest, char *source);
char *strncpy (char *dest, char *source, size_t num);
int strcmp(char *str1, char *str2);
int strncmp(char *str1, char *str2, size_t num);
char *string_upcase(char *string);

void print_int (int i);
void print_hex (size_t i);
void print_hexl (unsigned long l);
void print_double (double x, int p);

#endif

