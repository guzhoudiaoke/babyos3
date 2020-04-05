/*
 *	babyos/lib/unistd.cc
 *
 *  Copyright (C) <2020>  <Ruyi Liu>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


/*
 *  2020-03-07		created
 */


#include "string.h"
#include "stdint.h"


char* strcat(char* dst, const char* src)
{
    char* ret = dst;
    while (*dst) {
        dst++;
    }
    while (*src) {
        *dst++ = *src++;
    }
    return ret;
}

char* strrev(char* str, int len)
{
    char* s = str;
    char* e = str + len -1;
    while (s < e) {
        char c = *s;
        *s++ = *e;
        *e-- = c;
    }

    return str;
}

size_t strlen(const char* s)
{
    int len = 0;
    while (*s++) {
        len++;
    }
    return len;
}

char* strcpy(char* dst, const char* src)
{
	char* d = dst;
	while (*src) {
		*d++ = *src++;
	}
    *d++ = '\0';

	return dst;
}

char* strncpy(char* dst, const char* src, int n)
{
	char* d = dst;
	while (--n >= 0 && *src) {
		*d++ = *src++;
	}
    *d++ = '\0';

	return dst;
}

int strcmp(const char* s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }

    return *s1 - *s2;
}

int strncmp(const char* s1, const char *s2, int n)
{
    while (*s1 && *s2 && *s1 == *s2 && --n >= 0) {
        s1++;
        s2++;
    }

    return n == 0 ? 0 : *s1 - *s2;
}

char* strchr(const char* s, char c)
{
    char* p = (char *) s;
    while (*p != '\0') {
        if (c == *p) {
            return p;
        }
        p++;
    }
    return nullptr;
}

void* memmov(void *dst, const void *src, unsigned n)
{
    const char *s = (const char *) src;
    char *d = (char *) dst;

    if (s < d && s + n > d) {
        s += n, d += n;
        while (n--) {
            *--d = *--s;
        }
    }
    else {
        while (n--) {
            *d++ = *s++;
        }
    }

    return dst;
}

void* memcpy(void *dst, const void *src, unsigned n)
{
    return memmov(dst, src, n);
}

void* memset(void *dst, unsigned c, unsigned n)
{
    unsigned* d = (unsigned *) dst;
    for (unsigned i = 0; i < n / 4; i++) {
        *d++ = c;
    }

    char* d2 = (char *) dst + n / 4;
    for (unsigned i = 0; i < n % 4; i++) {
        *d2++ = (c & 0xff);
    }

    return dst;
}

int memcmp(const void* v1, const void* v2, size_t n)
{
    auto* s1 = (const uint8_t*)v1;
    auto* s2 = (const uint8_t*)v2;
    while (n-- > 0) {
        if (*s1++ != *s2++) {
            return s1[-1] < s2[-1] ? -1 : 1;
        }
    }
    return 0;
}

void* memchr(const void* ptr, int c, size_t size)
{
    char ch = c;
    auto* cptr = (const char*)ptr;
    for (size_t i = 0; i < size; ++i) {
        if (cptr[i] == ch) {
            return const_cast<char*>(cptr + i);
        }
    }
    return nullptr;
}


size_t strspn(const char* s, const char* accept)
{
    const char* p = s;
cont:
    char ch = *p++;
    char ac;
    for (const char* ap = accept; (ac = *ap++) != '\0';) {
        if (ac == ch)
            goto cont;
    }
    return p - 1 - s;
}

char* strstr(const char *s, const char *find)
{
	char c, sc;
	size_t len;

	if ((c = *find++) != '\0') {
		len = strlen(find);
		do {
			do {
				if ((sc = *s++) == '\0')
					return (NULL);
			} while (sc != c);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}

int strcoll(const char* s1, const char* s2)
{
    return strcmp(s1, s2);
}

char * strerror(int errnum)
{
    // TODO
    return nullptr;
}