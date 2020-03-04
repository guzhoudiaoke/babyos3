/*
 *	babyos/kernel/buddy.cc
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
 *  2020-02-15		created
 */


#include "math.h"


uint32 math_t::log(int32 x, int32 n)
{
    int32 ret = 0, num = 1;
    while (num < n) {
        num *= x;
        ret++;
    }

    return ret;
}

uint32 math_t::min(uint32 a, uint32 b)
{
    return a < b ? a : b;
}

uint32 math_t::max(uint32 a, uint32 b)
{
    return a > b ? a : b;
}

uint64 math_t::pow(int32 x, int32 p)
{
    uint64 ret = 1;
    for (int32 i = 0; i < p; i++) {
        ret *= x;
    }
    return ret;
}

