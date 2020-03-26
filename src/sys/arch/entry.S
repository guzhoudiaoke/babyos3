/*
 *	babyos/kernel/isr.S
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
 *  aquad with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


/*
 *  2020-02-16		created
 */


#include "kernel.h"


# ----------------------------------------------------------------------------------------------------------------------
# 向量号  助记符 说明                        类型        错误号    产生源
# --------------------------------------------------------------------------------------------------------------------
# 0       #DE    除出错                      故障        无        DIV或IDIV指令
# 1       #DB    调试                        故障/陷阱   无        任何代码或数据引用，或是INT 1指令
# 2        --    NMI中断                     中断        无        非屏蔽外部中断
# 3       #BP    断点                        陷阱        无        INT 3指令
# 4       #OF    溢出                        陷阱        无        INTO指令
# 5       #BR    边界范围超出                故障        无        BOUND指令
# 6       #UD    无效操作码（未定义操作码）  故障        无        UD2指令或保留的操作码。（Pentium Pro中加入的新指令）
# 7       #NM    设备不存在（无数学协处理器）故障        无        浮点或WAIT/FWAIT指令
# 8       #DF    双重错误                    异常终止    有（0）   任何可产生异常、NMI或INTR的指令
# 9        --    协处理器段超越（保留）      故障        无        浮点指令（386以后的CPU不产生该异常）
# 10      #TS    无效的任务状态段TSS         故障        有        任务交换或访问TSS
# 11      #NP    段不存在                    故障        有        加载段寄存器或访问系统段
# 12      #SS    堆栈段错误                  故障        有        堆栈操作和SS寄存器加载
# 13      #GP    一般保护错误                故障        有        任何内存引用和其他保护检查
# 14      #PF    页面错误                    故障        有        任何内存引用
# 15       --    （Intel保留，请勿使用）                 无
# 16      #MF    x87 FPU浮点错误（数学错误） 故障        无        x87 FPU浮点或WAIT/FWAIT指令
# 17      #AC    对齐检查                    故障        有（0）   对内存中任何数据的引用
# 18      #MC    机器检查                    异常终止    无        错误码（若有）和产生源与CPU类型有关（奔腾处理器引进）
# 19      #XF    SIMD浮点异常                故障        无        SSE和SSE2浮点指令（PIII处理器引进）
# 20-31    --    （Intel保留，请勿使用）
# 32-255   --    用户定义（非保留）中断      中断                  外部中断或者INT n指令
# ---------------------------------------------------------------------------------------------------------------------


.global isr_vector
.global ret_from_isr
.global ret_from_fork
.global switch_to_asm



need_resched = 0
sig_pending  = 4

frame_cs = (18*8)


#define SAVE_ALL                  \
    cld                       ;   \
    push %r15                 ;   \
	push %r14                 ;   \
	push %r13                 ;   \
	push %r12                 ;   \
	push %r11                 ;   \
	push %r10                 ;   \
	push %r9                  ;   \
	push %r8                  ;   \
	push %rdi                 ;   \
	push %rsi                 ;   \
	push %rbp                 ;   \
	push %rdx                 ;   \
	push %rcx                 ;   \
	push %rbx                 ;   \
	push %rax                 ;   \


#define RESTORE_ALL               \
	pop %rax                  ;   \
	pop %rbx                  ;   \
	pop %rcx                  ;   \
	pop %rdx                  ;   \
	pop %rbp                  ;   \
	pop %rsi                  ;   \
	pop %rdi                  ;   \
	pop %r8                   ;   \
	pop %r9                   ;   \
	pop %r10                  ;   \
	pop %r11                  ;   \
	pop %r12                  ;   \
	pop %r13                  ;   \
	pop %r14                  ;   \
	pop %r15                  ;   \


#define GET_CURRENT(reg)         \
	movq $-8192,reg          ;   \
	andq %rsp,  reg          ;   \


/*
 * %rdi: prev task
 * %rsi: next task
 * %rdx: &prev->context->rsp
 * %rcx: &next->context->rsp
 */
switch_to_asm:
	/*
	 * Save callee-saved registers
	 * This must match the order in inactive_task_frame
	 */
	pushq	%rbp
	pushq	%rbx
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	/* switch stack */
	movq	%rsp, (%rdx)
	movq	(%rcx), %rsp

	/* restore callee-saved registers */
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%rbx
	popq	%rbp

	jmp	__switch_to


reschedule:
	call    schedule
	jmp	    restore_all

dosignal:
    mov     %rsp,           %rdi
	call	do_signal
	jmp		restore_all

ret_from_fork:
    movq    %rax,           %rdi
	call	schedule_tail
    movq    %rsp,           %rdi
	jmp		restore_all

common_interrupt:
    SAVE_ALL
    mov     %rsp,           %rdi
	call    do_common_isr
ret_from_intr:
    movb    frame_cs(%rsp), %al
    testb   $0x3,           %al
    jz      restore_all

    GET_CURRENT(%rbx)
    cli
    cmpl    $0,             need_resched(%rbx)
    jne     reschedule
	cmpl    $0,				sig_pending(%rbx)
	jne     dosignal

restore_all:
	RESTORE_ALL
	add     $16,            %rsp
	iretq


#define MAKE_ISR(index) \
	isr_##index:			\
	push	$0x00;			\
	push	$index;			\
	jmp		common_interrupt


#define MAKE_ISR_WITH_ERROR(index) \
isr_##index:				\
	push	$index;			\
	jmp		common_interrupt


MAKE_ISR(0x00)
MAKE_ISR(0x01)
MAKE_ISR(0x02)
MAKE_ISR(0x03)
MAKE_ISR(0x04)
MAKE_ISR(0x05)
MAKE_ISR(0x06)
MAKE_ISR(0x07)
MAKE_ISR_WITH_ERROR(0x08)
MAKE_ISR(0x09)
MAKE_ISR_WITH_ERROR(0x0a)
MAKE_ISR_WITH_ERROR(0x0b)
MAKE_ISR_WITH_ERROR(0x0c)
MAKE_ISR_WITH_ERROR(0x0d)
MAKE_ISR_WITH_ERROR(0x0e)
MAKE_ISR(0x0f)
MAKE_ISR_WITH_ERROR(0x10)
MAKE_ISR(0x11)
MAKE_ISR(0x12)
MAKE_ISR(0x13)
MAKE_ISR(0x14)
MAKE_ISR(0x15)
MAKE_ISR(0x16)
MAKE_ISR(0x17)
MAKE_ISR(0x18)
MAKE_ISR(0x19)
MAKE_ISR(0x1a)
MAKE_ISR(0x1b)
MAKE_ISR(0x1c)
MAKE_ISR(0x1d)
MAKE_ISR(0x1e)
MAKE_ISR(0x1f)
MAKE_ISR(0x20)
MAKE_ISR(0x21)
MAKE_ISR(0x22)
MAKE_ISR(0x23)
MAKE_ISR(0x24)
MAKE_ISR(0x25)
MAKE_ISR(0x26)
MAKE_ISR(0x27)
MAKE_ISR(0x28)
MAKE_ISR(0x29)
MAKE_ISR(0x2a)
MAKE_ISR(0x2b)
MAKE_ISR(0x2c)
MAKE_ISR(0x2d)
MAKE_ISR(0x2e)
MAKE_ISR(0x2f)
MAKE_ISR(0x30)
MAKE_ISR(0x31)
MAKE_ISR(0x32)
MAKE_ISR(0x33)
MAKE_ISR(0x34)
MAKE_ISR(0x35)
MAKE_ISR(0x36)
MAKE_ISR(0x37)
MAKE_ISR(0x38)
MAKE_ISR(0x39)
MAKE_ISR(0x3a)
MAKE_ISR(0x3b)
MAKE_ISR(0x3c)
MAKE_ISR(0x3d)
MAKE_ISR(0x3e)
MAKE_ISR(0x3f)
MAKE_ISR(0x40)
MAKE_ISR(0x41)
MAKE_ISR(0x42)
MAKE_ISR(0x43)
MAKE_ISR(0x44)
MAKE_ISR(0x45)
MAKE_ISR(0x46)
MAKE_ISR(0x47)
MAKE_ISR(0x48)
MAKE_ISR(0x49)
MAKE_ISR(0x4a)
MAKE_ISR(0x4b)
MAKE_ISR(0x4c)
MAKE_ISR(0x4d)
MAKE_ISR(0x4e)
MAKE_ISR(0x4f)
MAKE_ISR(0x50)
MAKE_ISR(0x51)
MAKE_ISR(0x52)
MAKE_ISR(0x53)
MAKE_ISR(0x54)
MAKE_ISR(0x55)
MAKE_ISR(0x56)
MAKE_ISR(0x57)
MAKE_ISR(0x58)
MAKE_ISR(0x59)
MAKE_ISR(0x5a)
MAKE_ISR(0x5b)
MAKE_ISR(0x5c)
MAKE_ISR(0x5d)
MAKE_ISR(0x5e)
MAKE_ISR(0x5f)
MAKE_ISR(0x60)
MAKE_ISR(0x61)
MAKE_ISR(0x62)
MAKE_ISR(0x63)
MAKE_ISR(0x64)
MAKE_ISR(0x65)
MAKE_ISR(0x66)
MAKE_ISR(0x67)
MAKE_ISR(0x68)
MAKE_ISR(0x69)
MAKE_ISR(0x6a)
MAKE_ISR(0x6b)
MAKE_ISR(0x6c)
MAKE_ISR(0x6d)
MAKE_ISR(0x6e)
MAKE_ISR(0x6f)
MAKE_ISR(0x70)
MAKE_ISR(0x71)
MAKE_ISR(0x72)
MAKE_ISR(0x73)
MAKE_ISR(0x74)
MAKE_ISR(0x75)
MAKE_ISR(0x76)
MAKE_ISR(0x77)
MAKE_ISR(0x78)
MAKE_ISR(0x79)
MAKE_ISR(0x7a)
MAKE_ISR(0x7b)
MAKE_ISR(0x7c)
MAKE_ISR(0x7d)
MAKE_ISR(0x7e)
MAKE_ISR(0x7f)
MAKE_ISR(0x80)
MAKE_ISR(0x81)
MAKE_ISR(0x82)
MAKE_ISR(0x83)
MAKE_ISR(0x84)
MAKE_ISR(0x85)
MAKE_ISR(0x86)
MAKE_ISR(0x87)
MAKE_ISR(0x88)
MAKE_ISR(0x89)
MAKE_ISR(0x8a)
MAKE_ISR(0x8b)
MAKE_ISR(0x8c)
MAKE_ISR(0x8d)
MAKE_ISR(0x8e)
MAKE_ISR(0x8f)
MAKE_ISR(0x90)
MAKE_ISR(0x91)
MAKE_ISR(0x92)
MAKE_ISR(0x93)
MAKE_ISR(0x94)
MAKE_ISR(0x95)
MAKE_ISR(0x96)
MAKE_ISR(0x97)
MAKE_ISR(0x98)
MAKE_ISR(0x99)
MAKE_ISR(0x9a)
MAKE_ISR(0x9b)
MAKE_ISR(0x9c)
MAKE_ISR(0x9d)
MAKE_ISR(0x9e)
MAKE_ISR(0x9f)
MAKE_ISR(0xa0)
MAKE_ISR(0xa1)
MAKE_ISR(0xa2)
MAKE_ISR(0xa3)
MAKE_ISR(0xa4)
MAKE_ISR(0xa5)
MAKE_ISR(0xa6)
MAKE_ISR(0xa7)
MAKE_ISR(0xa8)
MAKE_ISR(0xa9)
MAKE_ISR(0xaa)
MAKE_ISR(0xab)
MAKE_ISR(0xac)
MAKE_ISR(0xad)
MAKE_ISR(0xae)
MAKE_ISR(0xaf)
MAKE_ISR(0xb0)
MAKE_ISR(0xb1)
MAKE_ISR(0xb2)
MAKE_ISR(0xb3)
MAKE_ISR(0xb4)
MAKE_ISR(0xb5)
MAKE_ISR(0xb6)
MAKE_ISR(0xb7)
MAKE_ISR(0xb8)
MAKE_ISR(0xb9)
MAKE_ISR(0xba)
MAKE_ISR(0xbb)
MAKE_ISR(0xbc)
MAKE_ISR(0xbd)
MAKE_ISR(0xbe)
MAKE_ISR(0xbf)
MAKE_ISR(0xc0)
MAKE_ISR(0xc1)
MAKE_ISR(0xc2)
MAKE_ISR(0xc3)
MAKE_ISR(0xc4)
MAKE_ISR(0xc5)
MAKE_ISR(0xc6)
MAKE_ISR(0xc7)
MAKE_ISR(0xc8)
MAKE_ISR(0xc9)
MAKE_ISR(0xca)
MAKE_ISR(0xcb)
MAKE_ISR(0xcc)
MAKE_ISR(0xcd)
MAKE_ISR(0xce)
MAKE_ISR(0xcf)
MAKE_ISR(0xd0)
MAKE_ISR(0xd1)
MAKE_ISR(0xd2)
MAKE_ISR(0xd3)
MAKE_ISR(0xd4)
MAKE_ISR(0xd5)
MAKE_ISR(0xd6)
MAKE_ISR(0xd7)
MAKE_ISR(0xd8)
MAKE_ISR(0xd9)
MAKE_ISR(0xda)
MAKE_ISR(0xdb)
MAKE_ISR(0xdc)
MAKE_ISR(0xdd)
MAKE_ISR(0xde)
MAKE_ISR(0xdf)
MAKE_ISR(0xe0)
MAKE_ISR(0xe1)
MAKE_ISR(0xe2)
MAKE_ISR(0xe3)
MAKE_ISR(0xe4)
MAKE_ISR(0xe5)
MAKE_ISR(0xe6)
MAKE_ISR(0xe7)
MAKE_ISR(0xe8)
MAKE_ISR(0xe9)
MAKE_ISR(0xea)
MAKE_ISR(0xeb)
MAKE_ISR(0xec)
MAKE_ISR(0xed)
MAKE_ISR(0xee)
MAKE_ISR(0xef)
MAKE_ISR(0xf0)
MAKE_ISR(0xf1)
MAKE_ISR(0xf2)
MAKE_ISR(0xf3)
MAKE_ISR(0xf4)
MAKE_ISR(0xf5)
MAKE_ISR(0xf6)
MAKE_ISR(0xf7)
MAKE_ISR(0xf8)
MAKE_ISR(0xf9)
MAKE_ISR(0xfa)
MAKE_ISR(0xfb)
MAKE_ISR(0xfc)
MAKE_ISR(0xfd)
MAKE_ISR(0xfe)
MAKE_ISR(0xff)

isr_vector:
	.quad	isr_0x00, isr_0x01, isr_0x02, isr_0x03
	.quad	isr_0x04, isr_0x05, isr_0x06, isr_0x07
	.quad	isr_0x08, isr_0x09, isr_0x0a, isr_0x0b
	.quad	isr_0x0c, isr_0x0d, isr_0x0e, isr_0x0f

	.quad	isr_0x10, isr_0x11, isr_0x12, isr_0x13
	.quad	isr_0x14, isr_0x15, isr_0x16, isr_0x17
	.quad	isr_0x18, isr_0x19, isr_0x1a, isr_0x1b
	.quad	isr_0x1c, isr_0x1d, isr_0x1e, isr_0x1f

	.quad	isr_0x20, isr_0x21, isr_0x22, isr_0x23
	.quad	isr_0x24, isr_0x25, isr_0x26, isr_0x27
	.quad	isr_0x28, isr_0x29, isr_0x2a, isr_0x2b
	.quad	isr_0x2c, isr_0x2d, isr_0x2e, isr_0x2f

	.quad	isr_0x30, isr_0x31, isr_0x32, isr_0x33
	.quad	isr_0x34, isr_0x35, isr_0x36, isr_0x37
	.quad	isr_0x38, isr_0x39, isr_0x3a, isr_0x3b
	.quad	isr_0x3c, isr_0x3d, isr_0x3e, isr_0x3f

	.quad	isr_0x40, isr_0x41, isr_0x42, isr_0x43
	.quad	isr_0x44, isr_0x45, isr_0x46, isr_0x47
	.quad	isr_0x48, isr_0x49, isr_0x4a, isr_0x4b
	.quad	isr_0x4c, isr_0x4d, isr_0x4e, isr_0x4f

	.quad	isr_0x50, isr_0x51, isr_0x52, isr_0x53
	.quad	isr_0x54, isr_0x55, isr_0x56, isr_0x57
	.quad	isr_0x58, isr_0x59, isr_0x5a, isr_0x5b
	.quad	isr_0x5c, isr_0x5d, isr_0x5e, isr_0x5f

	.quad	isr_0x60, isr_0x61, isr_0x62, isr_0x63
	.quad	isr_0x64, isr_0x65, isr_0x66, isr_0x67
	.quad	isr_0x68, isr_0x69, isr_0x6a, isr_0x6b
	.quad	isr_0x6c, isr_0x6d, isr_0x6e, isr_0x6f

	.quad	isr_0x70, isr_0x71, isr_0x72, isr_0x73
	.quad	isr_0x74, isr_0x75, isr_0x76, isr_0x77
	.quad	isr_0x78, isr_0x79, isr_0x7a, isr_0x7b
	.quad	isr_0x7c, isr_0x7d, isr_0x7e, isr_0x7f

	.quad	isr_0x80, isr_0x81, isr_0x82, isr_0x83
	.quad	isr_0x84, isr_0x85, isr_0x86, isr_0x87
	.quad	isr_0x88, isr_0x89, isr_0x8a, isr_0x8b
	.quad	isr_0x8c, isr_0x8d, isr_0x8e, isr_0x8f

	.quad	isr_0x90, isr_0x91, isr_0x92, isr_0x93
	.quad	isr_0x94, isr_0x95, isr_0x96, isr_0x97
	.quad	isr_0x98, isr_0x99, isr_0x9a, isr_0x9b
	.quad	isr_0x9c, isr_0x9d, isr_0x9e, isr_0x9f

	.quad	isr_0xa0, isr_0xa1, isr_0xa2, isr_0xa3
	.quad	isr_0xa4, isr_0xa5, isr_0xa6, isr_0xa7
	.quad	isr_0xa8, isr_0xa9, isr_0xaa, isr_0xab
	.quad	isr_0xac, isr_0xad, isr_0xae, isr_0xaf

	.quad	isr_0xb0, isr_0xb1, isr_0xb2, isr_0xb3
	.quad	isr_0xb4, isr_0xb5, isr_0xb6, isr_0xb7
	.quad	isr_0xb8, isr_0xb9, isr_0xba, isr_0xbb
	.quad	isr_0xbc, isr_0xbd, isr_0xbe, isr_0xbf

	.quad	isr_0xc0, isr_0xc1, isr_0xc2, isr_0xc3
	.quad	isr_0xc4, isr_0xc5, isr_0xc6, isr_0xc7
	.quad	isr_0xc8, isr_0xc9, isr_0xca, isr_0xcb
	.quad	isr_0xcc, isr_0xcd, isr_0xce, isr_0xcf

	.quad	isr_0xd0, isr_0xd1, isr_0xd2, isr_0xd3
	.quad	isr_0xd4, isr_0xd5, isr_0xd6, isr_0xd7
	.quad	isr_0xd8, isr_0xd9, isr_0xda, isr_0xdb
	.quad	isr_0xdc, isr_0xdd, isr_0xde, isr_0xdf

	.quad	isr_0xe0, isr_0xe1, isr_0xe2, isr_0xe3
	.quad	isr_0xe4, isr_0xe5, isr_0xe6, isr_0xe7
	.quad	isr_0xe8, isr_0xe9, isr_0xea, isr_0xeb
	.quad	isr_0xec, isr_0xed, isr_0xee, isr_0xef

	.quad	isr_0xf0, isr_0xf1, isr_0xf2, isr_0xf3
	.quad	isr_0xf4, isr_0xf5, isr_0xf6, isr_0xf7
	.quad	isr_0xf8, isr_0xf9, isr_0xfa, isr_0xfb
	.quad	isr_0xfc, isr_0xfd, isr_0xfe, isr_0xff

