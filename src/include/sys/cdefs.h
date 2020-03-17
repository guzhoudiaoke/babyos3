/*
 *	babyos/lib/libc/sys/cdefs.h
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
 *  2020-03-16		created
 */


#ifndef _CDEFS_H_
#define _CDEFS_H_


#if defined(__cplusplus)
    #define	__BEGIN_DECLS		extern "C" {
    #define	__END_DECLS		}
    #define	__static_cast(x,y)	static_cast<x>(y)
#else
    #define	__BEGIN_DECLS
    #define	__END_DECLS
    #define	__static_cast(x,y)	(x)y
#endif

#define __always_inline __attribute__((__always_inline__))

#endif
