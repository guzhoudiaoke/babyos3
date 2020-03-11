/*
 *	babyos/kernel/string.h
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
 *  2020-02-13		created
 */


#include "types.h"
#include "kstring.h"


#define CHARACTER(ch)       (ch & 0xff)


void* memmov(void* dst, const void* src, uint32 n)
{
    const char* s = (const char *) src;
    char* d = (char *) dst;

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

void* memcpy(void* dst, const void* src, uint32 n)
{
    return memmov(dst, src, n);
}

void* memset(void* dst, uint32 c, uint32 n)
{
    char* d = (char *) dst;
    for (uint32 i = 0; i < n; i++) {
        *d++ = (c & 0xff);
    }

    return dst;
}

int memcmp(const void* b1, const void* b2, uint32 n)
{
    if (n == 0) {
        return 0;
    }

    while (--n && *((char *)b1) == *((char *)b2)) {
        b1 = (char *) b1 + 1;
        b2 = (char *) b2 + 1;
    }

    return (*((char *)b1) - *((char *)b2));
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

int strlen(const char* s)
{
    int len = 0;
    while (*s++) {
        len++;
    }
    return len;
}

int strncmp(const char* s1, const char *s2, int n)
{
    while (*s1 && *s2 && *s1 == *s2 && --n >= 0) {
        s1++;
        s2++;
    }

    return n == 0 ? 0 : *s1 - *s2;
}

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


bool is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

int sprint_long(char* buffer, long n, int width, int base, bool sign)
{
    if (base <= 0) {
        return -1;
    }

    const static char digits[] = "0123456789abcdef";
    char buf[16] = {0};

    uint64 num = (uint64)n;
    if (sign && (sign = (n < 0))) {
        num = -n;
    }

    int i = 0;
    do {
        buf[i++] = digits[num % base];
        num /= base;
    } while (num != 0);

    if (sign) {
        buf[i++] = '-';
    }

    if (width < i) {
        width = i;
    }

    for (int j = 0; j < width - i; j++) {
        *buffer++ = base == 16 ? '0' : ' ';
    }
    while (--i >= 0) {
        *buffer++ = buf[i];
    }

    return width;
}

int sprint_int(char* buffer, int n, int width, int base, bool sign)
{
    if (base <= 0) {
        return -1;
    }

    const static char digits[] = "0123456789abcdef";
    char buf[16] = {0};

    uint32 num = (uint32)n;
    if (sign && (sign = (n < 0))) {
        num = -n;
    }

    int i = 0;
    do {
        buf[i++] = digits[num % base];
        num /= base;
    } while (num != 0);

    if (sign) {
        buf[i++] = '-';
    }

    if (width < i) {
        width = i;
    }

    for (int j = 0; j < width - i; j++) {
        *buffer++ = base == 16 ? '0' : ' ';
    }
    while (--i >= 0) {
        *buffer++ = buf[i];
    }

    return width;
}

int sprint_str(char* buffer, char* s, int width)
{
    if (s == nullptr) {
        return 0;
    }

    int len = strlen(s);
    if (width < len) {
        width = len;
    }

    strcat(buffer, s);
    buffer += len;
    for (int i = 0; i < width - len; i++) {
        *buffer++ = ' ';
    }
    return width;
}

int vsprintf(char *buffer, const char* fmt, va_list ap)
{
    buffer[0] = '\0';
    if (fmt == nullptr) {
        return 0;
    }

    char c;
    int total = 0, width = 0;
    bool prefix_l = false, sign = false;

    for (int i = 0; (c = CHARACTER(fmt[i])) != 0; i++) {
        if (c != '%') {
            buffer[total++] = c;
            continue;
        }

        c = CHARACTER(fmt[++i]);
        if (c == '\0') {
            break;
        }

        width = 0;
        while (c != '\0' && is_digit(c)) {
            width = width * 10 + c - '0';
            c = CHARACTER(fmt[++i]);
        }

        if (c == '\0') {
            break;
        }

        if (c == 'l') {
            prefix_l = true;
            c = CHARACTER(fmt[++i]);
        }

        if (c == '\0') {
            break;
        }

        switch (c) {
        case 'd':
        case 'u':
            sign = c == 'd';
            if (prefix_l) {
                total += sprint_long(buffer + total, va_arg(ap, int64), width, 10, sign);
            }
            else {
                total += sprint_int(buffer + total, va_arg(ap, int32), width, 10, sign);
            }
            break;
        case 'x':
        case 'p':
            if (c == 'p') {
                prefix_l = true;
                buffer[total++] = '0';
                buffer[total++] = 'x';
            }
            if (prefix_l) {
                total += sprint_long(buffer + total, va_arg(ap, int64), width, 16, false);
            }
            else {
                total += sprint_int(buffer + total, va_arg(ap, int32), width, 16, false);
            }
            break;
        case 'c':
            buffer[total++] = (char) CHARACTER(va_arg(ap, int32));
            break;
        case 's':
            total += sprint_str(buffer + total, va_arg(ap, char *), width);
            break;
        case '%':
            buffer[total++] = '%';
            break;
        default:
            buffer[total++] = '%';
            buffer[total++] = c;
            break;
        }

        if (prefix_l) {
            prefix_l = false;
        }
    }

    return total;
}

int sprintf(char* buffer, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int total = vsprintf(buffer, fmt, ap);
    va_end(ap);

    return total;
}

