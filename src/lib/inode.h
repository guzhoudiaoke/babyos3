/*
 *	babyos/kernel/fs.cc
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
 *  2020-02-19		created
 */



#ifndef _INODE_H_
#define _INODE_H_

#include "types.h"


#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(unsigned int))
#define MAX_FILE_SIZE (NDIRECT + NINDIRECT)

class disk_inode_t {
public:

public:
    uint16 m_type;
    uint16 m_major;
    uint16 m_minor;
    uint16 m_nlinks;
    uint32 m_size;
    uint32 m_addrs[NDIRECT + 1];
};


class inode_t {
public:
    enum inode_type {
        I_TYPE_DIR = 1,
        I_TYPE_FILE,
        I_TYPE_DEV,
    };
};


#endif
