/*
 *	babyos/kernel/mm.h
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
 *  2020-02-13		created
 */

#ifndef _MM_H_
#define _MM_H_

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

#define KERNEL_BASE         (0xffff800000000000ull)

#define VA2PA(x)	        (((uint64)(x)) - KERNEL_BASE)
#define PA2VA(x)	        ((void *)(((uint64)x) + KERNEL_BASE))


#endif
