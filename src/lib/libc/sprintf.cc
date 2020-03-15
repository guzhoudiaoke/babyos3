/*
 *	babyos/lib/libc/sprintf.cc
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
 *  2020-03-15		created
 */


#include <cdefs.h>
#include "stddef.h"
#include "stdarg.h"
#include "string.h"


__BEGIN_DECLS


#define CHARACTER(ch)       (ch & 0xff)


int sprint_int(char* buffer, int n, int width, int base, bool sign)
{
    static char digits[] = "0123456789abcdef";
    char buf[16] = {0};

    unsigned num = (unsigned)n;
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

    int len = i;
    while (--i >= 0) {
        *buffer++ = buf[i];
    }

    for (int i = 0; i < width - len; i++) {
        *buffer++ = ' ';
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

    unsigned long num = (unsigned long)n;
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
            if (c == 'l') {
                prefix_l = true;
                c = CHARACTER(fmt[++i]);
            }
        }

        if (c == '\0') {
            break;
        }

        switch (c) {
        case 'd':
        case 'u':
            sign = c == 'd';
            if (prefix_l) {
                total += sprint_long(buffer + total, va_arg(ap, unsigned long), width, 10, sign);
            }
            else {
                total += sprint_int(buffer + total, va_arg(ap, int), width, 10, sign);
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
                total += sprint_long(buffer + total, va_arg(ap, unsigned long), width, 16, false);
            }
            else {
                total += sprint_int(buffer + total, va_arg(ap, int), width, 16, false);
            }
            break;
        case 'c':
            buffer[total++] = (char) CHARACTER(va_arg(ap, int));
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

int snprintf(char * buf, size_t size, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int total = vsprintf(buf, fmt, ap);
    va_end(ap);

    return total;
}


__END_DECLS