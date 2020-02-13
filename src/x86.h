/*
 *	babyos/kernel/x86.h
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
 *  2020-01-21		created
 */


#ifndef _X86_H_
#define _X86_H_

#include "types.h"

static inline uint8 inb(uint16 port)
{
    uint8 data;
    __asm__ volatile("inb %1,%0" : "=a" (data) : "d" (port));
    return data;
}

static inline void outb(uint16 port, uint8 data)
{
    __asm__ volatile("outb %0,%1" : : "a" (data), "d" (port));
}

static inline uint16 inw(uint16 port)
{
    uint16 data;
    __asm__ volatile("inw %1,%0" : "=a" (data) : "d" (port));
    return data;
}

static inline void insl(int port, void *addr, int cnt)
{
    __asm__ volatile("cld; rep insl" :
            "=D" (addr), "=c" (cnt) :
            "d" (port), "0" (addr), "1" (cnt) :
            "memory", "cc");
}

static inline void outw(uint16 port, uint16 data)
{
    __asm__ volatile("outw %0,%1" : : "a" (data), "d" (port));
}

static inline void outl(uint16 port, uint32 val)
{
	asm volatile("outl %0,%1" : : "a" (val), "dN" (port));
}

static inline uint32 inl(uint16 port)
{
	uint32 v;
	asm volatile("inl %1,%0" : "=a" (v) : "dN" (port));
	return v;
}

static inline void outsl(int port, const void *addr, int cnt)
{
    __asm__ volatile("cld; rep outsl" :
            "=S" (addr), "=c" (cnt) :
            "d" (port), "0" (addr), "1" (cnt) :
            "cc");
}

static inline void stosb(void *addr, int32 data, int32 cnt)
{
    __asm__ volatile("cld; rep stosb" :
            "=D" (addr), "=c" (cnt) :
            "0" (addr), "1" (cnt), "a" (data) :    /* di=addr, cx=cnt, ax=data */
            "memory", "cc");
}

static inline void movsb(void *dst, void *src, int32 cnt)
{
    __asm__ volatile("cld; rep movsb" :
            :
            "D" (dst), "S" (src), "c" (cnt) :      /* di=dst, si=src, cx=cnt */
            "memory", "cc");
}

static inline void halt(void)
{
        __asm__ volatile("hlt");
}

#define rdtsc(low,high) \
    __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))

#define rdtsc64(val) \
    __asm__ __volatile__("rdtsc" : "=A" (val))

static inline void nop(void)
{
    __asm__ __volatile__("rep; nop");
}


#endif
