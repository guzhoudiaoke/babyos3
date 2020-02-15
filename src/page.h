/*
 *	babyos/kernel/page.h
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
 *  2020-02-15		created
 */

#ifndef _PAGE_H_
#define _PAGE_H_

#include "types.h"
#include "kernel.h"



#define KB                  (1024)
#define MB                  (1024*KB)
#define GB                  (1024*GB)


/* page size and so on */
#define PAGE_SHIFT          (12)
#define PAGE_SIZE           (1UL << PAGE_SHIFT)
#define PAGE_MASK           (~(PAGE_SIZE-1))
#define PAGE_ALIGN(addr)    (((uint64)(addr)+PAGE_SIZE-1) & PAGE_MASK)

#define PTE_INDEX(va)       (((uint64)va >> 12) & 0x1ff)
#define PDE_INDEX(va)       (((uint64)va >> 21) & 0x1ff)
#define PDPE_INDEX(va)      (((uint64)va >> 30) & 0x1ff)
#define PML4E_INDEX(va)     (((uint64)va >> 39) & 0x1ff)

#define PTRS_PER_PTE        512
#define PTRS_PER_PDE        512
#define PTRS_PER_PDPE       512
#define PTRS_PER_PML4E      512


/* kernel base address */
#define KERNEL_BASE         (0xffff800000000000ull)


/* kernel va->pa, pa->va */
#define VA2PA(x)	        (((uint64)(x)) - KERNEL_BASE)
#define PA2VA(x)	        ((void *)(((uint64)x) + KERNEL_BASE))


/* pml4 table entry */
typedef uint64 pml4e_t;
/* page directory pointer table entry */
typedef uint64 pdpe_t;
/* page directory table entry */
typedef uint64 pde_t;
/* page table entry */
typedef uint64 pte_t;


#endif
