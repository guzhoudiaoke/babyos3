/*
 *	babyos/lib/libc/stdlib.h
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
 *  2020-03-12		created
 */


#ifndef _STDLIB_H_
#define _STDLIB_H_


#include "stddef.h"
#include "malloc.h"


#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1



#define RAND_MAX 0x7fffffff


int system(const char* command);
char* getenv(const char* name);

double strtod(const char*, char** endptr);
char* strpbrk(const char*, const char* accept);


int rand();
void srand(unsigned seed);

long int random();
void srandom(unsigned seed);

void abort();

int abs(int val);

double atof (const char *__nptr);

#endif
