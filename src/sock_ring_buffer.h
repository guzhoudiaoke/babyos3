/*
 *	babyos/kernel/sock_ring_buffer.h
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
 *  2020-02-26		created
 */




#ifndef _SOCK_BUFFER_H_
#define _SOCK_BUFFER_H_


#include "types.h"
#include "spinlock.h"
#include "sem.h"


class socket_t;
class sock_ring_buffer_t {
public:
    void   init(socket_t* socket, uint32 order);
    int32  get_char(uint8& ch);
    int32  put_char(uint8 ch);
    uint32 get_free_space();

    int32  get_data(uint8* data, uint32 max);
    int32  put_data(uint8* data, uint32 len);

private:
    socket_t*   m_socket;
    uint8*      m_buffer;
    uint32      m_length;
    uint32      m_left;
    uint32      m_read_index;
    uint32      m_write_index;
    spinlock_t  m_lock;
    semaphore_t m_wait_space;
    semaphore_t m_wait_item;
};

#endif
