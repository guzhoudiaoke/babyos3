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
 *  2020-02-19		created
 */


#include "file.h"


//void file_t::init(uint32 type, inode_t* inode, pipe_t* pipe, uint32 offset, uint16 readable, uint16 writeable)
void file_t::init(uint32 type, inode_t* inode, uint64 offset, uint16 readable, uint16 writeable)
{
    m_type = type;
    m_ref = 1;
    m_readable = readable;
    m_writeable = writeable;
    m_inode = inode;
    //m_pipe = pipe;
    m_offset = 0;
    //m_socket = NULL;
}

//void file_t::init(uint32 type, socket_t* socket)
//{
//    m_type = TYPE_SOCKET;
//    m_ref = 1;
//    m_readable = 1;
//    m_writeable = 1;
//    m_inode = NULL;
//    m_pipe = NULL;
//    m_offset = 0;
//    m_socket = socket;
//}
