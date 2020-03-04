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

static inline void lgdt(void* gdt, uint32 size)
{
    volatile uint16 pd[5];

    pd[0] = size-1;
    pd[1] = (uint64)gdt;
    pd[2] = (uint64)gdt >> 16;
    pd[3] = (uint64)gdt >> 32;
    pd[4] = (uint64)gdt >> 48;

    __asm__ volatile("lgdt (%0)" : : "r" (pd));
}

static inline void lidt(void* idt, uint32 size)
{
    volatile uint16 pd[5];

    pd[0] = size-1;
    pd[1] = (uint64)idt;
    pd[2] = (uint64)idt >> 16;
    pd[3] = (uint64)idt >> 32;
    pd[4] = (uint64)idt >> 48;

    __asm__ volatile("lidt (%0)" : : "r" (pd));
}

static inline void ltr(uint16 sel)
{
    __asm__ volatile("ltr %0" : : "r" (sel));
}

static inline void sti(void)
{
    __asm__ volatile("sti");
}

static inline void cli(void)
{
    __asm__ volatile("cli");
}

static inline void halt(void)
{
        __asm__ volatile("hlt");
}

static inline uint64 rdtsc()
{
    uint32 low, high;
    __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high));
    return ((uint64) high << 32) | low;
}

static inline void nop(void)
{
    __asm__ __volatile__("rep; nop");
}

static inline uint64 get_cr0(void)
{
    uint32 val;
    __asm__ volatile("movl %%cr0, %0" : "=r" (val));
    return val;
}

static inline void set_cr0(uint64 val)
{
    __asm__ volatile("movl %0, %%cr0" : : "r" (val));
}

static inline void set_cr3(uint64 val)
{
    __asm__ volatile("mov %0, %%cr3" : : "r" (val));
}

static inline uint32 change_bit(uint64 nr, void* addr)
{
    uint32 oldbit;

    __asm__ __volatile__("btcq %2,%1\n\tsbbl %0,%0"
                         :"=r" (oldbit),"=m" (*((unsigned *)(addr)))
                         :"r" (nr));
    return oldbit;
}

#define CMOS_ADDR_PORT		0x70
#define CMOS_DATA_PORT		0x71

static inline uint32 cmos_read(uint32 reg)
{
    outb(CMOS_ADDR_PORT, reg);
    return inb(CMOS_DATA_PORT);
}

static inline void cmos_write(uint32 reg, uint32 value)
{
    outb(CMOS_ADDR_PORT, reg);
    outb(CMOS_DATA_PORT, value);
}

#define rdmsr(msr, val1, val2)                      \
	__asm__ __volatile__("rdmsr"                    \
                         : "=a" (val1), "=d" (val2) \
                         : "c" (msr))

#define wrmsr(msr, val1, val2)                                  \
	__asm__ __volatile__("wrmsr"                                \
                         : /* no outputs */                     \
                         : "c" (msr), "a" (val1), "d" (val2))


static inline uint32 xchg(volatile uint32 *addr, uint32 newval)
{
    uint32 result;

    // "+m": read-modify-write operand
    __asm__ volatile("lock; xchgl %0, %1" :
                     "+m" (*addr), "=a" (result) :
                     "1" (newval) :
                     "cc");
    return result;
}


#define mb() 	__asm__ __volatile__ ("lock; addq $0,0(%%rsp)": : :"memory")
#define rmb()	mb()
#define wmb()	__asm__ __volatile__ ("": : :"memory")

#define barrier() __asm__ __volatile__("": : :"memory")


#endif
