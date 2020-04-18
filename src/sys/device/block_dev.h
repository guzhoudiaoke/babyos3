/*
 *	babyos/kernel/block_dev.h
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
 *  2020-02-17		created
 */


#ifndef _BLOCK_DEV_H_
#define _BLOCK_DEV_H_

#include "babyos/types.h"
#include "dlist.h"
#include "sem.h"
#include "spinlock.h"
#include "kernel.h"
#include "io_buffer.h"


class block_dev_t {
public:
    void             init(uint32 dev);
    io_buffer_t*     read(uint32 block);
    int              write(io_buffer_t* b);
    void             release_block(io_buffer_t* b);

private:
    io_buffer_t*     get_block(uint32 block);
    io_buffer_t*     find_from_cache(uint32 block);

private:
    uint32        m_dev;
    uint32        m_buf_num;
    io_buffer_t*  m_bufs;
    dlist_t       m_used_list;
    dlist_t       m_free_list;
    spinlock_t    m_lock;
};

#endif
