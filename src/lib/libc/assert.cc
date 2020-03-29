/*
 *	babyos/lib/libc/assert.cc
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
 *  2020-03-29		created
 */


#include "assert.h"
#include "stdio.h"
#include "stdlib.h"


void __assert(const char* file, int line, const char* func, const char* expr)
{
    printf("Assertion failed: %s: line%d function: %s: expr: %s\n", file, line, func, expr);
    abort();
}