/*
 *	babyos/sys/arch/fpu.cc
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
 *  2020-04-06		created
 */


#include "fpu.h"
#include "x86.h"


#define X86_CR0_PE_BIT	0 /* Protection Enable */
#define X86_CR0_PE		(1ull << X86_CR0_PE_BIT)
#define X86_CR0_MP_BIT	1 /* Monitor Coprocessor */
#define X86_CR0_MP		(1ull << X86_CR0_MP_BIT)
#define X86_CR0_EM_BIT	2 /* Emulation */
#define X86_CR0_EM		(1ull << X86_CR0_EM_BIT)
#define X86_CR0_TS_BIT	3 /* Task Switched */
#define X86_CR0_TS		(1ull << X86_CR0_TS_BIT)


#define X86_CR4_OSFXSR_BIT	    9 /* enable fast FPU save and restore */
#define X86_CR4_OSFXSR		    (1ull << X86_CR4_OSFXSR_BIT)
#define X86_CR4_OSXMMEXCPT_BIT	10 /* enable unmasked SSE exceptions */
#define X86_CR4_OSXMMEXCPT	    (1ull << X86_CR4_OSXMMEXCPT_BIT)


fpu_t::fpu_t()
{

}

fpu_t::~fpu_t()
{

}

void fpu_t::init()
{
 	__asm__ volatile ("clts");

    uint64 cr0 = get_cr0();
    cr0 &= ~(X86_CR0_TS | X86_CR0_EM);
    set_cr0(cr0);

    uint64 cr4 = get_cr4();
	cr4 |= (X86_CR4_OSFXSR | X86_CR4_OSXMMEXCPT);
    set_cr4(cr4);

    asm volatile("fninit");
}
