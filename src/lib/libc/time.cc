/*
 *	babyos/lib/libc/time.cc
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
 *  2020-03-16		created
 */


#include "time.h"
#include "stdio.h"



int gettimeofday(struct timeval* __restrict__ tv, void* __restrict__)
{
    // TODO
    printf("%s not implemented\n", __FUNCTION__);
    return 0;
}

time_t time(time_t* tloc)
{
    // TODO
    //printf("%s not implemented\n", __FUNCTION__);
    return 0;
}

clock_t times(struct tms* buf)
{
    // TODO
    printf("%s not implemented\n", __FUNCTION__);
    return 0;
}
clock_t clock()
{
    // TODO
    printf("%s not implemented\n", __FUNCTION__);
    return 0;
}



static inline bool __is_leap_year(int year)
{
    return ((year % 4 == 0) && ((year % 100 != 0) || (year % 400) == 0));
}

static const int __days_per_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static const int __seconds_per_day = 60 * 60 * 24;

static void time_to_tm(struct tm* tm, time_t t)
{
    int days = t / __seconds_per_day;
    int remaining = t % __seconds_per_day;
    tm->tm_sec = remaining % 60;
    remaining /= 60;
    tm->tm_min = remaining % 60;
    tm->tm_hour = remaining / 60;
    tm->tm_wday = (4 + days) % 7;
    int year;
    for (year = 1970; days >= 365 + __is_leap_year(year); ++year)
        days -= 365 + __is_leap_year(year);
    tm->tm_year = year - 1900;
    tm->tm_yday = days;
    tm->tm_mday = 1;
    if (__is_leap_year(year) && days == 59)
        ++tm->tm_mday;
    if (__is_leap_year(year) && days >= 59)
        --days;
    int month;
    for (month = 0; month < 11 && days >= __days_per_month[month]; ++month)
        days -= __days_per_month[month];
    tm->tm_mon = month;
    tm->tm_mday += days;
}


struct tm* localtime_r(const time_t* t, struct tm* tm)
{
    time_to_tm(tm, *t);
    return tm;
}


struct tm* localtime(const time_t* t)
{
    if (!t)
        return nullptr;
    static struct tm tm_buf;
    return localtime_r(t, &tm_buf);
}


struct tm* gmtime(const time_t* t)
{
    // FIXME: This is obviously not correct. What about timezones bro?
    return localtime(t);
}

size_t strftime(char* destination, size_t, const char*, const struct tm*)
{
    return 0;
}




time_t mktime(struct tm* tm)
{
    int days = 0;
    int seconds = tm->tm_hour * 3600 + tm->tm_min * 60 + tm->tm_sec;
    for (int year = 70; year < tm->tm_year; ++year)
        days += 365 + __is_leap_year(1900 + year);
    tm->tm_yday = tm->tm_mday - 1;
    for (int month = 0; month < tm->tm_mon; ++month)
        tm->tm_yday += __days_per_month[month];
    days += tm->tm_yday;
    return days * __seconds_per_day + seconds;
}


