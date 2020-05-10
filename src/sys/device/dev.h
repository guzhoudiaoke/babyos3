/*
 *	babyos/kernel/device/dev.h
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
 *  2020-05-07		created
 */


#ifndef _DEV_H_
#define _DEV_H_


#include "file.h"


class dev_t : file_t {
public:
    ~dev_t();
    uint32 major();
    uint32 minor();

private:
    dev_t();

private:
    uint32 m_major;
    uint32 m_minor;
};


#endif