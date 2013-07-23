
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

typedef unsigned int bool;
typedef unsigned int size_t;

void *memcpy (void *dest, void *start, size_t num);
void *memset (void *dest, int val, size_t num);
char *strcpy (char *dest, char *source);
char *strncpy (char *dest, char *source, size_t num);
int strcmp(char *str1, char *str2);
int strncmp(char *str1, char *str2, size_t num);
char *string_upcase(char *string);

#endif

