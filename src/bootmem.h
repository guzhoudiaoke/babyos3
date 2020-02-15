/*
 *	babyos/kernel/bootmem.h
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
 *  2020-02-14		created
 */


#ifndef _BOOTMEM_H_
#define _BOOTMEM_H_


#include "types.h"
#include "mm.h"

/*
Offset in Bytes		Name		       Description
0	                BaseAddrLow        Low 32 Bits of Base Address
4	                BaseAddrHigh	   High 32 Bits of Base Address
8	                LengthLow		   Low 32 Bits of Length in Bytes
12	                LengthHigh		   High 32 Bits of Length in Bytes
16	                Type		       Address type of  this range.
The BaseAddrLow and BaseAddrHigh together are the 64 bit BaseAddress of this range.
The BaseAddress is the physical address of the start of the range being specified.
The LengthLow and LengthHigh together are the 64 bit Length of this range.
The Length is the physical contiguous length in bytes of a range being specified.

The Type field describes the usage of the described address range as defined in the table below.

Value	Pneumonic		        Description
1	    AddressRangeMemory	    This run is available RAM usable by the operating system.
2	    AddressRangeReserved	This run of addresses is in use or reserved
				                by the system, and must not be used by the
				                operating system.
Other	Undefined		        Undefined - Reserved for future use.  Any
                                range of this type must be treated by the
                                OS as if the type returned was
                                AddressRangeReserved.
*/
typedef struct address_range_s {
	uint32	base_addr_low;
	uint32	base_addr_high;
	uint32	length_low;
	uint32	length_high;
    uint32  type;
} __attribute__((packed, aligned(4))) address_range_t;


typedef struct memory_layout_s {
	uint32 num_of_range;
	address_range_t ranges[32];
} __attribute__((packed, aligned(4))) memory_layout_t;


class bootmem_t {
public:
    bootmem_t();
    ~bootmem_t();

    void init();

private:
    void   init_mem_range();
    void   init_page_map();
    uint64 mem_alloc(uint32 size, bool page_align);
    void   map_pages(void* va, uint64 pa, uint64 length, uint32 perm);
    uint64 early_va2pa(void* va);
    void*  early_pa2va(uint64 pa);

private:
    memory_layout_t* m_mem_layout;
    uint64           m_start_pa;
    uint64           m_pml4_pa;
};


#endif
