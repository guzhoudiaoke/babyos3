/*
 *	babyos/boot/head.S
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
 *  2020-01-20		created
 */


#include "kernel.h"

	.code32
	.section .text
	.global _start

	.org	0
_start:
	jmp		startup32


/* function to draw a line cross the screen at row edi */
draw_line:
	pushl   %esi
	pushl   %ecx
	pushl   %eax
    movl    (VIDEO_INFO_ADDR+8),    %esi
    addl    %edi,					%esi
    movl    $1024,                  %ecx
1:
    movb    $0xff,                  %al
    movb    %al,                    (%esi)
    addl    $3,                     %esi
    decl    %ecx
    cmpl    $0x0,                   %ecx
    jne		1b
	popl	%eax
	popl	%ecx
	popl	%esi
	ret

/* do dead loop */
dead_loop:
1:
	hlt
	jmp		1b


/* function to set boot page table */
setup_page_table:
	/* 1. setup pml4[0] = dir_ptr0, pml4[511] = dir_ptr0 */
	leal	pd_ptr0,				%eax
	xorl	$(PTE_P|PTE_W|PTE_U),	%eax
	movl    %eax,					(pml4)
	movl    %eax,					(pml4+PAGE_SIZE-8)

	/* 2. setup pg_dir_ptr[0,1,2,3] = pg_dir0, pg_dir1, pg_dir2, pg_dir3 */
	leal    pd_ptr0,				%edi
	leal	pg_dirs,				%eax
	xorl	$(PTE_P|PTE_W|PTE_U),	%eax
	movl    $4,						%ecx
1:
	movl    %eax,					(%edi)
	addl	$8,						%edi
	addl	$0x00001000,			%eax
	decl	%ecx
	jnz		1b


	/* 3. setup pg_dirs, 0x183(PRESENT, WRITE, MBZ) */
	leal	pg_dirs,				%edi
	movl	$0x00000183,			%eax
	movl    $2048,					%ecx
1:
	movl	%eax,					(%edi)
	addl	$0x00200000,			%eax
	addl	$8,						%edi
	decl	%ecx
	jnz		1b

	/* 4.set pml4 to cr3 */
	leal    pml4,					%eax
	movl	%eax,					%cr3

	ret


/* start code of protected mode */
startup32:
	cli
	movl	$(SEG_KDATA<<3),		%eax
	movw	%ax,					%ds
	movw	%ax,					%es
	movw	%ax,					%ss
	xorl	%eax,					%eax
	movw	%ax,					%fs
	movw	%ax,					%gs
	movl	$STACK_PM_BOTTOM,		%esp

	/* draw a line at row 200 */
    movl    $(200*1024*3+2),		%edi
	call	draw_line


	/* setup boot page table */
	call	setup_page_table

	/* load gdt */
	lgdt	gdt_ptr

	/* set CR4.PAE=1 */
	movl    %cr4,					%eax
	btsl	$5,						%eax
	movl	%eax,					%cr4

	/* set IA32_EFER=1 */
	movl	$0xc0000080,			%ecx
	rdmsr
	btsl    $8,						%eax
	wrmsr

	/* push address of startup_64 to stack, prepare to jump to 64bit mode */
	pushl	$SEG_KCODE<<3
	leal	startup_64(%ebp),		%eax
	pushl	%eax

	/* set CR0.PG=1, CR0.PE=1 */
	xorl	%eax,					%eax
	btsl    $0,						%eax
	btsl    $31,					%eax
	movl	%eax,					%cr0


	/* Jump from 32bit compatibility mode into 64bit mode. */
	lret


	.code64
	.org	0x200
/* start code of 64bit mode */
startup_64:
	xor		%rax,					%rax
	movw	%ax,					%ss
	movw	%ax,					%ds
	movw	%ax,					%es
	movw	%ax,					%fs
	movw	%ax,					%gs
	leaq	boot_stack_end,			%rsp

	/* draw a line */
	movq	$(300*1024*3+3),		%rdi
	call    draw_line_64
	jmp		dead_loop


/* function to draw a line in 64 bit mode */
draw_line_64:
	pushq   %rsi
	pushq   %rcx
	pushq   %rax
    movq    (VIDEO_INFO_ADDR+8),    %rsi
    addq    %rdi,					%rsi
    movq    $1024,                  %rcx
1:
    movb    $0xff,                  (%rsi)
    addq    $3,                     %rsi
    decq    %rcx
    jnz		1b
	popq	%rax
	popq	%rcx
	popq	%rsi
	ret


	
/* 
 * gdt
 */
	.align	16
gdt_ptr:
	.word	gdt_end - gdt - 1
	.quad   gdt
gdt:
    .quad   0x0000000000000000    /* NULL descriptor */
    .quad   0x00af9a000000ffff    /* __KERNEL_CS */
    .quad   0x00cf92000000ffff    /* __KERNEL_DS */
gdt_end:


	.data
/* 
 * page table
 */
	.balign 4096
pml4:
	.fill PAGE_SIZE, 1, 0
pd_ptr0:
	.fill PAGE_SIZE, 1, 0
pg_dirs:
	.fill PAGE_SIZE, 1, 0
	.fill PAGE_SIZE, 1, 0
	.fill PAGE_SIZE, 1, 0
	.fill PAGE_SIZE, 1, 0


/* 
 * stack
 */
boot_stack:
	.fill BOOT_STACK_SIZE, 1, 0
boot_stack_end:

