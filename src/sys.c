
#include "sys.h"

void *memcpy (void *dest, void *source, size_t num) {
	unsigned int i;

	for (i=0; i < num; i++) {
		((char *)dest)[i] = ((char *)source)[i];
	}
	return dest;
}

void *memset (void *dest, int val, size_t num) {
	unsigned int i;
	for(i=0; i < num; i++) {
		((int *)dest)[i] = val;
	}
	return dest;
}

char *strcpy (char *dest, char *source) {
	while (1) {
		if (*source == '\0') {
			*dest = '\0';
			break;
		}
		
		*dest = *source;
		dest++;
		source++;
	}
	return dest;
}

char *strncpy (char *dest, char *source, size_t num) {
	unsigned int i = 0;
	
	while (i < num) {
		if (*source == '\0') {
			*dest = '\0';
			break;
		}
		
		*dest = *source;
		dest++;
		source++;
		i++;
	}
	return dest;
}

int strcmp(char *str1, char *str2) {
	while (str1 != '\0' && str2 != '\0') {
		if (*str1 == *str2) {
			str1++;
			str2++;
		}
	}
	return (str2 - str1);
}
