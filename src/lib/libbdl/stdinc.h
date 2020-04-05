/*
 *	babyos/bin/stdinc.cc
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
 *  2020-04-05		created
 */


#ifndef _STDINC_H_
#define _STDINC_H_


#include <stdint.h>


void memset4(void *dst, uint32_t val, size_t dwords)
{
    size_t _n = (dwords + 3) / 4;
    uint32_t *_p = (uint32_t *) dst;
    uint32_t _val = (val);
    if (dwords == 0)
        return;
    switch (dwords % 4)
    {
        case 0: do {    *_p++ = _val;   /* fallthrough */
        case 3:         *_p++ = _val;   /* fallthrough */
        case 2:         *_p++ = _val;   /* fallthrough */
        case 1:         *_p++ = _val;   /* fallthrough */
        } while ( --_n );
    }
}


#endif