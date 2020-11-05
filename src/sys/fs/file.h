/*
 *	babyos/kernel/fs/file.h
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


#ifndef _FILE_H_
#define _FILE_H_


#include "babyos/types.h"
#include "fd.h"


class file_t {
public:
    file_t();

    virtual void create() = 0;
    virtual void open(int flags) = 0;
    virtual void close() = 0;
    virtual uint64 read(file_descriptor_t* fd, void* buffer, uint64 size) = 0;
    virtual uint64 write(file_descriptor_t* fd, void* buffer, uint64 size) = 0;
    virtual uint64 ioctl(file_descriptor_t* fd, uint32 cmd, uint64 arg) = 0;
};


#endif