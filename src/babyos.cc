/*
 *	babyos/kernel/babyos.cc
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

#include "babyos.h"
#include "x86.h"
#include "delay.h"

static babyos_t babyos;


babyos_t* babyos_t::get_instance()
{
    return &babyos;
}

babyos_t::babyos_t()
{
}

babyos_t::~babyos_t()
{
}

uart_t* babyos_t::uart()
{
    return &m_uart;
}

vbe_t* babyos_t::vbe()
{
    return &m_vbe;
}

console_t* babyos_t::console()
{
    return &m_console;
}

bootmem_t* babyos_t::bootmem()
{
    return &m_bootmem;
}

void babyos_t::init()
{
    /* serial port */
    m_uart.early_init();
    uart()->puts("Hello babyos..\n");

    /* VBE */
    m_vbe.init();

    /* console */
    m_console.init();
    console()->kprintf(YELLOW, "Welcome to babyos!\n");

    /* bootmem */
    m_bootmem.init();
    uart()->puts("boot mem init done\n");
}

void babyos_t::run()
{
    while (true) {
        halt();
    }
}
