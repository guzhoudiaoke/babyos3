/*
 *	babyos/sys/device/framebuffer.h
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
 *  2020-03-29		created
 */


#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include "babyos/types.h"


class framebuffer_t {
public:
    framebuffer_t();
    ~framebuffer_t();
    void init();
    int ioctl(int cmd, uint64 arg);

private:
};


#endif