/*
 *	babyos/kernel/i8254.h
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


#include "i8254.h"
#include "babyos.h"
#include "x86.h"
#include "traps.h"


i8254_t::i8254_t()
{
}

i8254_t::~i8254_t()
{
}

void i8254_t::init()
{
    m_tick = 0;

    uint32 val = CLOCK_TICK_RATE / HZ;

    /* control */
    outb(0x43, 0x34);

    /* clock 0 */
    outb(0x40, uint8(val & 0xff));
    outb(0x40, uint8(val >> 8));

	os()->i8259a()->enable_irq(IRQ_TIMER);		/* enable keyboard interrupt */
}

void i8254_t::do_irq()
{
    m_tick++;
    os()->update(m_tick);

    /* EOI */
    outb(0x20, 0x20);
}

uint32 i8254_t::get_timer_count()
{
    uint32 count = 0;
    outb(0x43, 0x00);
    count = inb(0x40);
    count |= inb(0x40) << 8;

    return count;
}

