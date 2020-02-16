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

buddy_t* babyos_t::buddy()
{
    return &m_buddy;
}

i8259a_t* babyos_t::i8259a()
{
    return &m_i8259a;
}

i8254_t* babyos_t::i8254()
{
    return &m_i8254;
}

cpu_t* babyos_t::cpu()
{
    return &m_cpu;
}

rtc_t* babyos_t::rtc()
{
    return &m_rtc;
}

keyboard_t* babyos_t::keyboard()
{
    return &m_keyboard;
}


void test_buddy()
{
    os()->buddy()->dump();
    for (int i = 0; i < 10; i++) {
        uint64 pa = os()->buddy()->alloc_pages(0);
        os()->console()->kprintf(GREEN, "allocated page: %lx\n", pa);
        os()->uart()->kprintf("allocated page: %lx\n", pa);
        os()->buddy()->dump();
    }
}

void babyos_t::init()
{
    /* serial port */
    m_uart.early_init();
    uart()->puts("Hello babyos..\n");

    /* bootmem */
    m_bootmem.init();
    uart()->puts("boot mem init done\n");

    /* buddy */
    m_buddy.init();
    uart()->puts("buddy init done\n");

    /* VBE */
    m_vbe.init();
    uart()->puts("VBE init done\n");

    /* console */
    m_console.init();
    uart()->puts("console init done\n");
    console()->kprintf(YELLOW, "Welcome to babyos!\n");

    /* 8259a */
    m_i8259a.init();
    uart()->puts("8259a init done\n");

    /* 8254 */
    m_i8254.init();
    uart()->puts("8254 init done\n");

    /* cpu */
    m_cpu.init();
    m_cpu.startup();
    uart()->puts("cpu startup done\n");

    /* rtc */
    m_rtc.init();
    uart()->puts("rtc init done\n");

    /*keyboard */
    m_keyboard.init();

    /* start interrupt */
    sti();
    uart()->puts("sti done\n");
}

void babyos_t::run()
{
    while (true) {
        halt();
    }
}

void show_time_now()
{
    uint32 year, month, day, h, m, s;
    year = os()->rtc()->year();
    month = os()->rtc()->month();
    day = os()->rtc()->day();
    h = os()->rtc()->hour();
    m = os()->rtc()->minute();
    s = os()->rtc()->second();
    os()->uart()->kprintf("%d-%d-%d %2d:%2d:%2d\n", 2000+year, month, day, h, m, s);
}

void babyos_t::update(uint64 tick)
{
    rtc()->update();

    if (tick % 100 == 0) {
        show_time_now();
    }
}

