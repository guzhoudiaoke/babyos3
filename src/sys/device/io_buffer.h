/*
 *	babyos/kernel/io_buffer.h
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



#ifndef _IO_BUFFER_H_
#define _IO_BUFFER_H_

#include "types.h"
#include "kernel.h"
#include "sem.h"
#include "dlist.h"
#include "fs.h"


class io_buffer_t {
public:

    void init();
    void lock();
    void unlock();
    void wait();
    void done();

public:
    uint32          m_block;
    uint32          m_done;
    semaphore_t     m_sem;
    semaphore_t     m_sem_wait_done;
    uint8           m_buffer[BSIZE];

    dlist_node_t    m_used_list_node;
    dlist_node_t    m_free_list_node;
};


#endif
