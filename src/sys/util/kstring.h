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



#ifndef _STRING_H_
#define _STRING_H_

#include <stdarg.h>
#include "babyos/types.h"

extern "C" {

//void* memmove(void* dst, const void* src, uint32 n);
void* memcpy(void* dst, const void* src, size_t n);
void* memset(void* dst, int c, size_t n);
int   memcmp(const void* b1, const void* b2, size_t n);

char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, int n);
int   strcmp(const char* s1, const char *s2);
int   strlen(const char* s);
int   strncmp(const char* s1, const char *s2, int n);

bool  is_digit(char c);
int   sprintf(char* buffer, const char *fmt, ...);
int   vsprintf(char *buffer, const char *fmt, va_list ap);

void* memmove(void *dest, const void *src, size_t count);

}

#endif
