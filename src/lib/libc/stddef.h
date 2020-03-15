/*
 *	babyos/lib/stddef.h
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


#ifndef _STDDEF_H_
#define _STDDEF_H_


typedef unsigned long size_t;
typedef long ssize_t;
typedef size_t off_t;

typedef int pid_t;
typedef int uid_t;

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
typedef unsigned long   uint64_t;
typedef char            int8_t;
typedef short           int16_t;
typedef int             int32_t;
typedef long            int64_t;


#define NULL nullptr

typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__ size_t;

#define offsetof(type, member) __builtin_offsetof(type, member)


#define assert(expr) 
#define ASSERT assert
#define ASSERT_NOT_REACHED assert


extern "C" {



}


//#define isalnum(c) (_ctype_[(int)(c)] & (_U | _L | _N))
//#define isalpha(c) (_ctype_[(int)(c)] & (_U | _L))
//#define iscntrl(c) (_ctype_[(int)(c)] & (_C))
//#define isdigit(c) (_ctype_[(int)(c)] & (_N))
//#define isxdigit(c) (_ctype_[(int)(c)] & (_N | _X))
//#define isspace(c) (_ctype_[(int)(c)] & (_S))
//#define ispunct(c) (_ctype_[(int)(c)] & (_P))
//#define isprint(c) (_ctype_[(int)(c)] & (_P | _U | _L | _N | _B))
//#define isgraph(c) (_ctype_[(int)(c)] & (_P | _U | _L | _N))
//#define islower(c) ((_ctype_[(int)(c)] & (_U | _L)) == _L)
//#define isupper(c) ((_ctype_[(int)(c)] & (_U | _L)) == _U)
//
//#define isascii(c) ((unsigned)c <= 127)
//#define toascii(c) ((c)&127)




#endif
