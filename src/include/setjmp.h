/*
 *	babyos/lib/libc/setjmp.h
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


#define	_JBLEN	11		/* size, in longs, of a jmp_buf */

typedef long sigjmp_buf[_JBLEN + 1];
typedef long jmp_buf[_JBLEN];

#define _setjmp(ctx) ({\
    int ret;\
    asm("lea     LJMPRET%=(%%rip), %%rcx    \n\t"\
        "xor     %%rax, %%rax               \n\t"\
        "mov     %%rbx, (%%rdx)             \n\t"\
        "mov     %%rbp, 8(%%rdx)            \n\t"\
        "mov     %%r12, 16(%%rdx)           \n\t"\
        "mov     %%r13, 24(%%rdx)           \n\t"\
        "mov     %%r14, 32(%%rdx)           \n\t"\
        "mov     %%r15, 40(%%rdx)           \n\t"\
        "mov     %%rsp, 48(%%rdx)           \n\t"\
        "mov     %%rcx, 56(%%rdx)           \n\t"\
        "LJMPRET%=:\n\t"\
        : "=a" (ret)\
        : "d" (ctx)\
        : "memory", "rcx", "rsi", "rdi", "r8", "r9", "r10", "r11", "cc");\
        ret;\
})

#define longjmp(ctx, x) \
    asm("movq   56(%%rdx), %%rcx            \n\t"\
        "movq   48(%%rdx), %%rsp            \n\t"\
        "movq   40(%%rdx), %%r15            \n\t"\
        "movq   32(%%rdx), %%r14            \n\t"\
        "movq   24(%%rdx), %%r13            \n\t"\
        "movq   16(%%rdx), %%r12            \n\t"\
        "movq   8(%%rdx),  %%rbp            \n\t"\
        "movq   (%%rdx),   %%rbx            \n\t"\
        ".cfi_def_cfa %%rdx, 0              \n\t"\
        ".cfi_offset %%rbx, 0               \n\t"\
        ".cfi_offset %%rbp, 8               \n\t"\
        ".cfi_offset %%r12, 16              \n\t"\
        ".cfi_offset %%r13, 24              \n\t"\
        ".cfi_offset %%r14, 32              \n\t"\
        ".cfi_offset %%r15, 40              \n\t"\
        ".cfi_offset %%rsp, 48              \n\t"\
        ".cfi_offset %%rip, 56              \n\t"\
        "jmp    *%%rcx\n\t"\
        : : "d" (ctx), "a" (1))

#define setjmp(env)	_setjmp (env)
