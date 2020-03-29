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
#define KERNEL_CS           (0x8)
#define KERNEL_DS           (0x10)


/* boot memory layout */
#define BOOT_PML4           (0x3000)
#define BOOT_PD_PTR0        (0x4000)
#define BOOT_PG_DIRS        (0x5000)    /* 4 page dirs, [0x5000, 0x9000) */
#define BOOT_STACK_BOTTOM   (0xe000)    /* size 8kb,   [0xc000, 0xe000) */

/* video info */
#define BOOT_INFO_ADDR		(0xe000)    /* 56k, boot info address, should under 64k */
#define VIDEO_INFO_ADDR	    (BOOT_INFO_ADDR)
#define VIDEO_INFO_SIZE	    (12)

/* memory info */
#define MEM_INFO_ADDR	    (VIDEO_INFO_ADDR + VIDEO_INFO_SIZE)
#define MEM_INFO_SIZE	    (4+256)

/* disk */
#define	SECT_SIZE			(512)
#define LOADER_LBA          (0)
#define KERNEL_ELF_LBA      (3)


/* for cr0 */
#define CR0_PE              (1 << 0)
#define CR0_PG              (1 << 31)
#define CR4_PAE             (1 << 5)

#define MSR_EFER		    0xc0000080  /* extended feature register */
#define EFER_LME            (1 << 8)    /* Long mode enable */



/* page table, page directory entry flag */
#define PTE_P               0x001		/* present */
#define PTE_W               0x002		/* writeable */
#define PTE_U               0x004		/* user */

/* kernel load base */
#define KERNEL_LOAD_BASE    0xffffffff80000000ULL
#define KERNEL_LOAD_EXT     0x0000000000100000ULL
#define KERNEL_LOAD_ADDR    (KERNEL_LOAD_BASE + KERNEL_LOAD_EXT)

/* kernel stack */
#define KERNEL_STACK_BOTTOM (KERNEL_LOAD_BASE + BOOT_STACK_BOTTOM)
#define KERNEL_STACK_SIZE   (8192)

/* kernel base address */
#define KERNEL_BASE         (0xffff800000000000ull)
#define USER_STACK_TOP      (0x0000800000000000ull)

/* kernel va->pa, pa->va */
#define V2P(x)	            (((uint64)(x)) - KERNEL_BASE)
#define P2V(x)	            ((void *)(((uint64)(x)) + KERNEL_BASE))


/* for device mmap */
#define DEV_BASE            (0xffffffff00000000ull) /* First device virtual address */
#define IO2V(x)             ((void *)(((uint64)(x)) + DEV_BASE))


#define NR_CPU              16

#endif
