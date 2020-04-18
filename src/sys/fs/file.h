/*
 *	babyos/kernel/file.cc
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
 *  2020-02-18		created
 */


#ifndef _FILE_H_
#define _FILE_H_


#include "babyos/types.h"
#include "babyos/filemode.h"


class inode_t;
class pipe_t;
class socket_t;

class file_t {
public:
    enum {
        TYPE_NONE,
        TYPE_PIPE,
        TYPE_INODE,
        TYPE_SOCKET,
    };

    void init(uint32 type, inode_t* inode, pipe_t* pipe, uint32 offset, uint16 readable, uint16 writeable);
    void init(uint32 type, socket_t* socket);

public:
    uint32      m_type;
    uint32      m_ref;
    uint16      m_readable;
    uint16      m_writeable;
    inode_t*    m_inode;
    pipe_t*     m_pipe;
    socket_t*   m_socket;
    uint32      m_offset;
};

typedef struct dev_op_s {
    int (*read)  (inode_t*, void*, int);
    int (*write) (inode_t*, void*, int);
    int (*ioctl) (inode_t*, int, uint64);
} dev_op_t;

#endif
