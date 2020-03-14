#ifndef _TIME_H_
#define _TIME_H_


#include "stddef.h"

struct tm {
    int tm_sec;   /* Seconds (0-60) */
    int tm_min;   /* Minutes (0-59) */
    int tm_hour;  /* Hours (0-23) */
    int tm_mday;  /* Day of the month (1-31) */
    int tm_mon;   /* Month (0-11) */
    int tm_year;  /* Year - 1900 */
    int tm_wday;  /* Day of the week (0-6, Sunday = 0) */
    int tm_yday;  /* Day in the year (0-365, 1 Jan = 0) */
    int tm_isdst; /* Daylight saving time */
};

extern long timezone;
extern long altzone;
extern char* tzname[2];
extern int daylight;

typedef uint32_t clock_t;
typedef int64_t time_t;
time_t time(time_t* tloc);

typedef int32_t suseconds_t;
struct timeval {
    time_t tv_sec;
    suseconds_t tv_usec;
};

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};




#define CLOCKS_PER_SEC 1000
time_t time(time_t* tloc);
clock_t times(struct tms* buf);
clock_t clock();



struct tm* gmtime(const time_t* t);
struct tm* localtime(const time_t* t);
size_t strftime(char* destination, size_t, const char*, const struct tm*);
time_t mktime(struct tm* tm);

double difftime(time_t, time_t);
#define difftime(t1, t0) (double)(t1 - t0)


#endif
