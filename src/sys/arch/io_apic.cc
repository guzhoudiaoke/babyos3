/*
 *	babyos/kernel/io_apic.h
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
 *  2020-02-27		created
 */




#include "io_apic.h"
#include "traps.h"
#include "babyos.h"


#define IO_APIC_REG_ID      0x00
#define IO_APIC_REG_VER     0x01
#define IO_APIC_REG_TABLE   0x10    /* redirection table */

#define INT_DISABLED        0x00010000


void io_apic_t::init()
{
    uint32 count = (read(IO_APIC_REG_VER) >> 16) & 0xff;
    //uint32 id = (read(IO_APIC_REG_ID) >> 24);

    //console()->kprintf(GREEN, "****************** io apic ****************\n", id);
    //console()->kprintf(GREEN, "io apic id: %u\n", id);
    //console()->kprintf(GREEN, "io apic num of table: %u\n", count);
    //console()->kprintf(GREEN, "****************** io apic ****************\n", id);

    for (uint32 i = 0; i <= count; i++) {
        write(IO_APIC_REG_TABLE + 2*i,     INT_DISABLED | (IRQ_0 + i));
        write(IO_APIC_REG_TABLE + 2*i + 1, 0);
    }
}

void io_apic_t::enable_irq(uint32 irq, uint32 cpu_id)
{
    write(IO_APIC_REG_TABLE + 2*irq,     IRQ_0 + irq);
    write(IO_APIC_REG_TABLE + 2*irq + 1, cpu_id << 24);
}

uint32 io_apic_t::read(uint32 reg)
{
    uint32* base = (uint32 *) IO2V(IO_APIC_BASE);
    *base = reg;
    return *(base + 4);
}

void io_apic_t::write(uint32 reg, uint32 data)
{
    uint32* base = (uint32 *) IO2V(IO_APIC_BASE);
    *base = reg;
    *(base + 4) = data;
}

