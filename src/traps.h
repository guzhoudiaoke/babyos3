/*
 *	babyos/kernel/traps.h
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
 *  2020-02-16		created
 */


#ifndef _TRAPS_H_
#define _TRAPS_H_


#include "types.h"


#define IRQ_0               (0x20)
#define IRQ_NUM             (0x10)

#define IRQ_TIMER           (0x00)
#define IRQ_KEYBOARD        (0x01)
#define IRQ_HARDDISK        (0x0e)

#define VEC_LOCAL_TIMER     (0xfc)
#define VEC_THERMAL         (0xfd)
#define VEC_ERROR           (0xfe)
#define VEC_SPURIOUS        (0xff)

#define IRQ_SYSCALL         (0x80)
#define INT_PF				(14)


typedef struct trap_frame_s {
    uint64 rax;
    uint64 rbx;
    uint64 rcx;
    uint64 rdx;
    uint64 rbp;
    uint64 rsi;
    uint64 rdi;
    uint64 r8;
    uint64 r9;
    uint64 r10;
    uint64 r11;
    uint64 r12;
    uint64 r13;
    uint64 r14;
    uint64 r15;

    uint64 trapno;
    uint64 err;

    uint64 rip;
    uint64 cs;
    uint64 rflags;
    uint64 rsp;
    uint64 ss;
} trap_frame_t;


#endif
