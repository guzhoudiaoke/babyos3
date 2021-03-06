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


#ifndef _STRING_H_
#define _STRING_H_


#include "stddef.h"


char* strcat(char* dst, const char* src);
char* strrev(char* str, int len);
size_t strlen(const char* s);
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, int n);
int   strcmp(const char* s1, const char *s2);
int   strncmp(const char* s1, const char *s2, int n);
char* strchr(const char* s, char c);

void* memmov(void *dst, const void *src, unsigned n);
void* memcpy(void *dst, const void *src, unsigned n);
void* memset(void *dst, unsigned c, unsigned n);
int   memcmp(const void*, const void*, size_t);

extern size_t strspn(const char * s, const char * c);
extern char * strstr(const char * h, const char * n);
int strcoll(const char * s1, const char * s2);
void* memchr(const void*, int c, size_t);

char* strerror(int errnum);

#endif
