/*
 *	babyos/kernel/delay.cc
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


#include "delay.h"
#include "x86.h"


bool   delay_t::s_inited = 0;
uint64 delay_t::s_cpu_freq = 0;
uint64 delay_t::s_cpu_freq_mhz = 0;

void delay_t::init(uint64 freq)
{
    if (!s_inited) {
        s_cpu_freq = freq;
        s_cpu_freq_mhz = freq / 1000000;
        s_inited = true;
    }
}

void delay_t::ms_delay(uint64 ms)
{
    us_delay(ms*1000);
}

void delay_t::us_delay(uint64 us)
{
    rdtsc_delay((uint64) us * s_cpu_freq_mhz);
}

void delay_t::rdtsc_delay(uint64 delta)
{
    uint64 prev, now;
    prev = rdtsc();
    do {
        nop();
        now = rdtsc();
    } while (now - prev < delta);
}

