/*
 *	babyos/kernel/rtc.h
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


#ifndef _RTC_H_
#define _RTC_H_


#include "types.h"


class rtc_t {
public:
    rtc_t();
    ~rtc_t();

    void init();
    void update();

    uint32 year() { return m_year; }
    uint32 month() { return m_month; }
    uint32 day() { return m_day; }
    uint32 hour() { return m_hour; }
    uint32 minute() { return m_minute; }
    uint32 second() { return m_second; }

private:
    void get_time();
    uint8 bcd_to_binary(uint8 bcd);

private:
    uint8 m_year;
    uint8 m_month;
    uint8 m_day;
    uint8 m_hour;
    uint8 m_minute;
    uint8 m_second;

    uint32 m_tick_to_update;
};


#endif
