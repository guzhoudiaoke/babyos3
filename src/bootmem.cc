/*
 *	babyos/kernel/bootmem.cc
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


#include "bootmem.h"
#include "babyos.h"
#include "kernel.h"
#include "mm.h"


extern uint8 _data[];    // defined by kernel.ld
extern uint8 _end[];     // defined by kernel.ld


bootmem_t::bootmem_t()
{
}

bootmem_t::~bootmem_t()
{
}

void bootmem_t::init()
{
    init_mem_range();
}

void bootmem_t::init_mem_range()
{
    m_mem_layout = (memory_layout_t *) PA2VA(MEM_INFO_ADDR);
    os()->console()->kprintf(WHITE, "the memory info from int 0x15, eax=0xe820:\n");
    os()->console()->kprintf(WHITE, "type\t\taddress\t\t\t\tlength\n");

    /* print mem layout */
    for (uint32 i = 0; i < m_mem_layout->num_of_range; i++) {
        uint64 addr = ((uint64)m_mem_layout->ranges[i].base_addr_high << 32) + m_mem_layout->ranges[i].base_addr_low;
        uint64 length = ((uint64)m_mem_layout->ranges[i].length_high << 32) + m_mem_layout->ranges[i].length_low;
        os()->console()->kprintf(RED, "0x%8x\t0x%16lx\t0x%16lx\n",
                                 m_mem_layout->ranges[i].type,
                                 addr,
                                 addr + length);
    }

    /* print usable memory above 1M */
    for (uint32 i = 0; i < m_mem_layout->num_of_range; i++) {
        uint64 addr = ((uint64)m_mem_layout->ranges[i].base_addr_high << 32) + m_mem_layout->ranges[i].base_addr_low;
        uint64 length = ((uint64)m_mem_layout->ranges[i].length_high << 32) + m_mem_layout->ranges[i].length_low;
        if (m_mem_layout->ranges[i].type == 1 && addr >= 1*MB) {
            os()->console()->kprintf(WHITE, "usable memory: from %luMB, to %luMB\n",
                                     addr / (1*MB), (addr+length) / (1*MB));
        }
    }

    /* set the end of kernel code/data as start of boot memory */
    m_mem_start = _end;
    os()->console()->kprintf(WHITE, "mem_start: 0x%16lx\n", m_mem_start);
}
