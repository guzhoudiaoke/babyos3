/*
 *	babyos/lib/libc/stdlib.cc
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
 *  2020-03-12		created
 */


#include "stdlib.h"
#include "signal.h"


int system(const char* command)
{
    printf("%s not implemented\n", __FUNCTION__);
    return 0;
}

char* getenv(const char* name)
{
    //printf("%s not implemented\n", __FUNCTION__);
    return nullptr;
}

double strtod(const char*, char** endptr)
{
    printf("%s not implemented\n", __FUNCTION__);
    return 0.0;
}

char* strpbrk(const char*, const char* accept)
{
    printf("%s not implemented\n", __FUNCTION__);
    return nullptr;
}

static unsigned long next = 1;
int rand()
{
    return ((next = next * 1103515245 + 12345) % ((unsigned long) RAND_MAX + 1));
}

void srand(unsigned seed)
{
    next = seed;
}

long int random()
{
    return rand();
}

void srandom(unsigned seed)
{
    srand(seed);
}

int abs(int val)
{
    return val < 0 ? -val : val;
}

void abort()
{
    raise(SIGABRT);
}

double atof (const char *__nptr)
{
    return strtod(__nptr, nullptr);
}
