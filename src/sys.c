
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
	int ret = 0;
	while (!(ret = (*str1 - *str2)) && (*str2 != '\0')) {
		str1++;
		str2++;
	}
	
	if (ret < 0) {
		ret = -1;
	} else if (ret > 0) {
		ret = 1;
	}
	
	return ret;
}

int strncmp(char *str1, char *str2, size_t num) {
	int ret = 0;
	size_t i = 0;
	while (!(ret = (*str1 - *str2)) && (*str2 != '\0') && (i < num)) {
		str1++;
		str2++;
		i++;
	}
	
	if (ret < 0) {
		ret = -1;
	} else if (ret > 0) {
		ret = 1;
	}
	
	return ret;
}

char *string_upcase(char *string) {
	char *str = string;
	while (*str != '\0') {
		if (*str >= 'a' && *str <= 'z') {
			*str = *str - 'a' + 'A';
		}
		str++;
	}
	return string;
}
