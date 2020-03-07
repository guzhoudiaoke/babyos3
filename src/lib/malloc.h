/*
 *	babyos/lib/malloc.h
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



#ifndef _MALLOC_H_
#define _MALLOC_H_


#include "stdio.h"


extern bool  malloc_init(void);
extern void* malloc(uint64 size);
extern void  free(void *ptr);
extern void* realloc(void *ptr, uint64 size);
extern void* calloc(uint64 nmemb, uint64 size);


#endif