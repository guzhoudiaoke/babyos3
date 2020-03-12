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
#include "sem.h"

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
    void init(uint16 major, uint16 minor, uint16 nlink);
    void lock();
    void unlock();
    void read_from_disk(int id);
    void write_to_disk();

public:
    uint32 m_dev;           // device number
    uint32 m_inum;          // inode number
    uint32 m_ref;           // reference count
    uint32 m_valid;
    semaphore_t m_sem;

    uint16 m_type;          // copy of disk inode
    uint16 m_major;
    uint16 m_minor;
    uint16 m_nlinks;
    uint32 m_size;
    uint32 m_addrs[NDIRECT+1];
};


#endif
