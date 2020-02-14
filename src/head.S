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


/*
 * function to set boot page table
 * map [0, 4GB) -> [0, 4GB)
 * map [0xffffffff80100000, 0xffffffffffffffff] -> [0, 2GB)
 * map [0xffff800000000000, 0xffff800000000000+4GB] -> [0, 4GB)
 */
setup_page_table:
	/*
     * 1. setup pml4[0] = dir_ptr0,
     *    setup pml4[256] = dir_ptr0
     *    setup pml4[511] = dir_ptr0
     */
	movl    $BOOT_PD_PTR0,			%eax
	xorl	$(PTE_P|PTE_W|PTE_U),	%eax
	movl	$BOOT_PML4,				%edi
	movl    %eax,					(%edi)

	addl    $(256*8),			    %edi
	movl    %eax,					(%edi)

	movl	$BOOT_PML4,				%edi
	addl    $(511*8),			    %edi
	movl    %eax,					(%edi)

	/*
     * 2. setup pg_dir_ptr[0,1,2,3] = pg_dir0, pg_dir1, pg_dir2, pg_dir3
     *    setup pg_dir_ptr[510,511] = pg_dir0, pg_dir1
	 */
	movl    $BOOT_PD_PTR0,			%edi
	movl	$BOOT_PG_DIRS,			%eax
	xorl	$(PTE_P|PTE_W|PTE_U),	%eax
	movl    $4,						%ecx
1:
	movl    %eax,					(%edi)
	addl	$8,						%edi
	addl	$0x00001000,			%eax
	decl	%ecx
	jnz		1b

	movl    $BOOT_PD_PTR0,			%edi
	movl	$BOOT_PG_DIRS,			%eax
	xorl	$(PTE_P|PTE_W|PTE_U),	%eax
	addl    $(510*8),			    %edi
    movl    $2,						%ecx
1:
	movl    %eax,					(%edi)
	addl	$8,						%edi
	addl	$0x00001000,			%eax
	decl	%ecx
	jnz		1b

	/* 3. setup pg_dirs, 0x183(PRESENT, WRITE, MBZ) */
	movl	$BOOT_PG_DIRS,			%edi
	movl	$0x00000183,			%eax
	movl    $2048,					%ecx
1:
	movl	%eax,					(%edi)
	addl	$0x00200000,			%eax
	addl	$8,						%edi
	decl	%ecx
	jnz		1b

	/* 4.set pml4 to cr3 */
	movl    $BOOT_PML4,				%eax
	movl	%eax,					%cr3

	ret


/* start code of protected mode */
startup32:
	cli
	movl	$KERNEL_DS,				%eax
	movw	%ax,					%ds
	movw	%ax,					%es
	movw	%ax,					%ss
	xorl	%eax,					%eax
	movw	%ax,					%fs
	movw	%ax,					%gs
	movl	$0x10000,				%esp	# temp kernel stack at 64k

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
	pushl	$KERNEL_CS
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
    movq	$KERNEL_STACK_BOTTOM,   %rsp

	/* call load main to load elf kernel */
	call	loadmain


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

