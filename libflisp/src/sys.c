
#include "sys.h"

void *memcpy(void *dest, void *src, int count)
{
	unsigned char *p = (unsigned char *)dest;
	unsigned char *s = (unsigned char *)src;
	while (count > 0) {
		*p = *s;
		p++;
		s++;
		count--;
	}
	return dest;
}

void *memset(void *dest, unsigned char val, int count)
{
	unsigned char *p = (unsigned char *)dest;	
	while (count > 0) {
		*p = val;
		p++;
		count--;
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

int strlen(char *str)
{
	char *p = str;
	unsigned int counter = 0;
	while (*p != '\0') {
		p++;
		counter++;
	}
	return counter;
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


void print_hex (unsigned int i) {
  unsigned int x;
  int nibble;

  for (nibble = 0; nibble < 8; nibble++) {
    x = (i >> (4 * (7 - nibble))) & 0x0000000F;

    if (x < 10) {
      putch ('0' + x);
    } else if (x >= 10 && x < 16) {
      putch ('A' + x - 10);
    } else {
      putch ('?');
    }
  }
}
  

void print_hexl (unsigned long l) {
  unsigned int i;
  i = (unsigned int)(l);
  print_hex (i);
  i = (unsigned int)(l >> 32);
  print_hex (i);
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

