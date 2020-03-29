/*
 *	babyos/include/assert.h
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


#ifndef _ASSERT_H_
#define _ASSERT_H_

#include <sys/cdefs.h>


__BEGIN_DECLS

#ifdef DEBUG
extern void __assert(const char* file, int line, const char* func, const char* expr);
#define assert(expr) ((expr) ? (void) 0 : __assert(__FILE__, __LINE__, __FUNCTION__, #expr))
#else
#define assert(expr) ((ovid) 0)
#endif

__END_DECLS

#endif