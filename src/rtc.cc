/*
 *	babyos/kernel/rtc.cc
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


#include "rtc.h"
#include "i8254.h"
#include "x86.h"
#include "babyos.h"


rtc_t::rtc_t()
{
}

rtc_t::~rtc_t()
{
}

void rtc_t::init()
{
    m_tick_to_update = HZ;
    get_time();
}

void rtc_t::update()
{
    if (--m_tick_to_update != 0) {
        return;
    }

    m_tick_to_update = HZ;
    m_second++;

    /* every 1 minute, re-read rtc from cmos */
    if (m_second == 60) {
        get_time();
    }
}

void rtc_t::get_time()
{
    m_year  = bcd_to_binary(cmos_read(9));
    m_month = bcd_to_binary(cmos_read(8));
    m_day   = bcd_to_binary(cmos_read(7));
    m_hour  = bcd_to_binary(cmos_read(4));
    m_minute= bcd_to_binary(cmos_read(2));
    m_second= bcd_to_binary(cmos_read(0));
}

uint8 rtc_t::bcd_to_binary(uint8 bcd)
{
    return (bcd >> 4) * 10 + (bcd & 0xf);
}

