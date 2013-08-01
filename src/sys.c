
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

void print_int (int i) {
	if (i < 0) {
		putch('-');
		i = -i;
	}

	if (i / 10 != 0) {
		print_int (i / 10);
	}
	putch('0' + (i % 10));
}

void print_hex (size_t i) {
	size_t x;
	if (i / 16 != 0) {
		print_int (i / 16);
	}
	x = i % 16;
	if (x < 10) {
		putch('0' + (i % 16));
	} else {
		putch ('A' + x - 10);
	}
}

void print_double (double x, int p) {
	long d;
	
	if (x < 0) {
		putch('-');
		x = -x;
	}
	
	d = x;
	
	print_int ((int)d);
	putch('.');
	while (p--) {
		x = (x - d) * 10;
		d = x;
		putch('0' + d);
	}
}

void print_string (char *str) {
	while (*str != '\0') {
		putch (*str);
		str = str++;
	}
}

