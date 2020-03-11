/*
 *	babyos/lib/stdio.cc
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


#include "stdio.h"
#include "string.h"


#define CHARACTER(ch)       (ch & 0xff)


int sprint_int(char* buffer, int n, int width, int base, bool sign)
{
    static char digits[] = "0123456789abcdef";
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
    if (s == NULL) {
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

int vsprintf(char *buffer, const char *fmt, va_list ap)
{
    buffer[0] = '\0';
    if (fmt == NULL) {
        return 0;
    }

    int total = 0;

    char c;
    int width = 0;
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

        switch (c) {
        case 'd':
            total += sprint_int(buffer + total, va_arg(ap, int32), width, 10, true);
            break;
        case 'u':
            total += sprint_int(buffer + total, va_arg(ap, int32), width, 10, false);
            break;
        case 'x':
        case 'p':
            total += sprint_int(buffer + total, va_arg(ap, int32), width, 16, false);
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

/* only support %d %u %x %p %c %s */
int printf(const char *fmt, ...)
{
    const int c_buffer_size = 1024;
    char buffer[c_buffer_size] = {0};

    va_list ap;
    va_start(ap, fmt);
    int total = vsprintf(buffer, fmt, ap);
    va_end(ap);

    write(fd_stdout, buffer, total);
    return total;
}


void gets(char* buf, uint32 max)
{
    memset(buf, 0, max);
    uint32 i = 0;
    while (i < max) {
        char c;
        int n = read(fd_stdin, &c, 1);
        if (n == 1) {
            if (c == '\n') {
                break;
            }
            *buf++ = c;
            i++;
        }
    }
}

void puts(char* buf)
{
    write(fd_stdout, buf, strlen(buf));
}
