/*
 *	babyos/kernel/local_apic.h
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



#ifndef _LOCAL_APIC_H_
#define _LOCAL_APIC_H_


#include "babyos/types.h"


#define APIC_BASE       (0xfee00000ull)


class local_apic_t {
public:
    int  check();
    void global_enable();
    void global_disable();
    void software_enable();
    void software_disable();

    uint32 calibrate_clock();
    void do_timer_irq();
    void eoi();

    void test();
    int  init_timer();
    int  init();
    void start_ap(uint32 id, uint32 addr);
    uint32 get_clocks();
    uint64 get_tick();

    static uint32 get_apic_id();

private:
    uint32  m_clocks;
    uint32  m_id;
    uint64  m_tick;
};

#endif
