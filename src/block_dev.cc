/*
 *	babyos/kernel/block_dev.cc
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


#include "block_dev.h"
#include "babyos.h"
#include "x86.h"
#include "ide.h"


#define BUFFER_PAGES        64
#define BUFFER_PAGES_ORDER  6


/*********************************************************/

void block_dev_t::init(uint32 dev)
{
    m_lock.init();
    m_used_list.init(os()->get_obj_pool_of_size());
    m_free_list.init(os()->get_obj_pool_of_size());

    m_dev = dev;
    m_buf_num = PAGE_SIZE * BUFFER_PAGES / sizeof(io_buffer_t);
    m_bufs = (io_buffer_t *) PA2VA(os()->buddy()->alloc_pages(BUFFER_PAGES_ORDER));
    for (uint32 i = 0; i < m_buf_num; i++) {
        m_bufs[i].init();
        m_free_list.push_back(m_bufs + i);
    }
}

io_buffer_t* block_dev_t::get_block(uint32 lba)
{
    io_buffer_t* b = NULL;

    uint64 flags;
    m_lock.lock_irqsave(flags);

    /* first, find from used list, if find it means this block cached */
    list_t<io_buffer_t *>::iterator it = m_used_list.begin();
    while (it != m_used_list.end() && (*it)->m_lba != lba) {
        it++;
    }

    /* find */
    if (it != m_used_list.end()) {
        b = *it;
    }
    else {
        /* get one from free list */
        if (!m_free_list.empty()) {
            b = *(m_free_list.begin());
            m_free_list.pop_front();
        }
        else {
            /* get front from used list */
            b = *(m_used_list.begin());
            m_used_list.pop_front();
        }

        b->m_lba = lba;
        b->m_done = 0;
    }
    m_lock.unlock_irqrestore(flags);

    if (b != NULL) {
        b->lock();
    }
    return b;
}

void block_dev_t::release_block(io_buffer_t* b)
{
    uint64 flags;
    m_lock.lock_irqsave(flags);

    list_t<io_buffer_t *>::iterator it = m_used_list.begin();
    while (it != m_used_list.end() && (*it)->m_lba != b->m_lba) {
        it++;
    }

    if (it == m_used_list.end()) {
        m_used_list.push_back(b);
    }

    m_lock.unlock_irqrestore(flags);
    b->unlock();
}

io_buffer_t* block_dev_t::read(uint32 lba)
{
    io_buffer_t* b = get_block(lba);
    if (b->m_done) {
        return b;
    }

    request_t req;
    req.init(m_dev, lba, request_t::CMD_READ, b);
    os()->ide()->add_request(&req);

    b->wait();

    return b;
}

int block_dev_t::write(io_buffer_t* b)
{
    request_t req;
    req.init(m_dev, b->m_lba, request_t::CMD_WRITE, b);
    os()->ide()->add_request(&req);

    b->wait();

    return 0;
}

