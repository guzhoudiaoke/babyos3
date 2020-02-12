/*
 *	babyos/kernel/kernel.h
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


#ifndef _KERNEL_H_
#define _KENERL_H_

/* segs */
#define SEG_NULL			(0)
#define SEG_KCODE           (1)
#define SEG_KDATA           (2)
#define SEG_UCODE           (3)
#define SEG_UDATA           (4)
#define SEG_TSS				(5)
#define GDT_LEN			    (6)
#define IDT_LEN				(256)

#define	SECT_SIZE			(512)
#define KSTACK_SIZE         (8192)
#define STACK_BOOT			(0x1000)
#define STACK_PM_BOTTOM     (0x10000)

#define BOOT_STACK_SIZE     (0x4000)

/* boot information */
#define BOOT_INFO_ADDR		(0xe000)    /* 0xa000 */
#define BOOT_INFO_SEG		(BOOT_INFO_ADDR >> 4)
#define VIDEO_INFO_ADDR	    (BOOT_INFO_ADDR)
#define VIDEO_INFO_OFFSET	(VIDEO_INFO_ADDR - BOOT_INFO_ADDR)
#define VIDEO_INFO_SIZE	    (12)

/* disk */
#define LOADER_SECT_NUM     (1)
#define ELF_SECT_NUM        (10)
#define FONT_ASC16_SECT_NUM (8)

#define LOADER_LBA          (0)
#define KERNEL_ELF_LBA      (2)

#define KERNEL_BASE         (0xffffffff80000000)


/* for cr0 */
#define CR0_PE              0x00000001
#define CR0_PG              0x80000000
#define CR0_WP              0x00010000


/* page table, page directory entry flag */
#define PTE_P               0x001		// present
#define PTE_W               0x002		// writeable
#define PTE_U               0x004		// user


/* page size and so on */
#define PAGE_SHIFT          (12)
#define PAGE_SIZE           (1UL << PAGE_SHIFT)
#define PAGE_MASK           (~(PAGE_SIZE-1))
#define PAGE_ALIGN(addr)    (((uint64)(addr)+PAGE_SIZE-1) & PAGE_MASK)

#define PGDIR_SHIFT	        (21)
#define PGDIR_SIZE	        (1UL << PGDIR_SHIFT)


#endif
