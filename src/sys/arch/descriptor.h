/*
 *	babyos/kernel/descriptor.h
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


#ifndef _DESCRIPTOR_H_
#define _DESCRIPTOR_H_


#include "babyos/types.h"
#include "page.h"


#define SEG_NULL			(0)
#define SEG_KCODE           (1)
#define SEG_KDATA           (2)
#define SEG_UCODE           (3)
#define SEG_UDATA           (4)
#define SEG_TSS0            (5)
#define SEG_TSS1			(6)

#define GDT_LEN			    (7)
#define IDT_LEN				(256)


#define TRAP_GATE_FLAG      (0x00008f0000000000ULL)
#define INTERRUPT_GATE_FLAG (0x00008e0000000000ULL)
#define SYSTEM_GATE_FLAG    (0x0000ef0000000000ULL)


/*
 * IO-bitmap sizes:
 */
#define IO_BITMAP_BITS			 65536
#define IO_BITMAP_BYTES			 (IO_BITMAP_BITS/8)
#define IO_BITMAP_LONGS			 (IO_BITMAP_BYTES/sizeof(long))
#define IO_BITMAP_OFFSET		 (offsetof(struct tss_struct, io_bitmap) - offsetof(struct tss_struct, x86_tss))
#define INVALID_IO_BITMAP_OFFSET 0x8000



typedef struct global_descriptor_s {
    uint64 descriptor;
} global_descriptor_t;


typedef struct gate_descriptor_s {
    uint64 descriptor0;
    uint64 descriptor1;
} gate_descriptor_t;


/*
 * tss struct defined in linux,
 * arch/x86/include/asm/processor.h
 */
typedef struct tss_hw_s {
    uint32          reserved1;
    uint64          sp0;

	/*
	 * We store cpu_current_top_of_stack in sp1 so it's always accessible.
	 * Linux does not use ring 1, so sp1 is not otherwise needed.
	 */
	uint64			sp1;

	/*
	 * Since Linux does not use ring 2, the 'sp2' slot is unused by
	 * hardware.  entry_SYSCALL_64 uses it as scratch space to stash
	 * the user RSP value.
	 */
	uint64			sp2;

	uint64			reserved2;
	uint64			ist[7];
	uint32			reserved3;
	uint32			reserved4;
	uint16			reserved5;
	uint16			io_bitmap_base;
} __attribute__((packed)) tss_hw_t;


typedef struct tss_s {
	/*
	 * The fixed hardware portion.  This must not cross a page boundary
	 * at risk of violating the SDM's advice and potentially triggering
	 * errata.
	 */
	tss_hw_t	     x86_tss;

	/*
	 * The extra 1 is there because the CPU will access an
	 * additional byte beyond the end of the IO permission
	 * bitmap. The extra byte must be all 1 bits, and must
	 * be within the limit.
	 */
	uint64	         io_bitmap[IO_BITMAP_LONGS + 1];
} __attribute__((__aligned__(PAGE_SIZE))) tss_t;



inline void set_global_descriptor(global_descriptor_t* desc, uint64 val)
{
    desc->descriptor = val;
}


inline void set_gate(gate_descriptor_t* desc, uint64 addr, uint64 flag)
{
    desc->descriptor0 = flag;                                            /* gate type */
    desc->descriptor0 |= ((uint64)addr) & 0xffff;                        /* low 16 bits of address */
    desc->descriptor0 |= (uint64)((SEG_KCODE << 3) << 16);               /* code segment selector */
    desc->descriptor0 |= ((uint64)addr << 32) & 0xffff000000000000ULL;   /* high 16 bits of address */

    desc->descriptor1 = addr >> 32;
}

inline void set_trap_gate(gate_descriptor_t* desc, uint64 addr)
{
    set_gate(desc, addr, TRAP_GATE_FLAG);
}

inline void set_intr_gate(gate_descriptor_t* desc, uint64 addr)
{
    set_gate(desc, addr, INTERRUPT_GATE_FLAG);
}

inline void set_system_gate(gate_descriptor_t* desc, uint64 addr)
{
    set_gate(desc, addr, SYSTEM_GATE_FLAG);
}

inline void get_tss_descriptor(uint64 base, uint64* desc0, uint64* desc1)
{
    uint64 limit = (uint64)(sizeof(tss_t) - 1);
    *desc0 = (limit & 0xffff) |				        // limit 15-0
        ((base & 0xffffff) << 16) |					// base  23-0
        ((0x9ull) << 40) |							// type
        ((0xeull) << 44) |							// P(1), DPL(11), 0
        (((limit >> 16) & 0xf) << 48) |				// limit 19-16
        ((0x00ULL) << 52) |							// G(0), 0, 0, AVL(0)
        (((base) >> 24) & 0xff) << 56;				// base  31-24

    *desc1 = (base >> 32);
}

#endif
