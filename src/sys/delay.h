/*
 *	babyos/kernel/delay.h
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

#ifndef _DELAY_H_
#define _DELAY_H_

#include "types.h"

class delay_t {
public:
    static void init(uint64 freq);
    static void ms_delay(uint64 ms);
    static void us_delay(uint64 us);

private:
    static void rdtsc_delay(uint64 delta);

private:
    static bool   s_inited;
    static uint64 s_cpu_freq;
    static uint64 s_cpu_freq_mhz;
};

#endif
