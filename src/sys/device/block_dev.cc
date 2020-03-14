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
    m_used_list.init();
    m_free_list.init();
    m_request_cache.create(sizeof(request_t), 8, 1);

    m_dev = dev;
    m_buf_num = PAGE_SIZE * BUFFER_PAGES / sizeof(io_buffer_t);
    m_bufs = (io_buffer_t *) P2V(os()->mm()->alloc_pages(BUFFER_PAGES_ORDER));
    for (uint32 i = 0; i < m_buf_num; i++) {
        m_bufs[i].init();
        m_free_list.add_tail(&m_bufs[i].m_free_list_node);
    }
}

io_buffer_t* block_dev_t::find_from_cache(uint32 lba)
{
    dlist_node_t* node = m_used_list.head();
    while (node != nullptr) {
        io_buffer_t* buffer = list_entry(node, io_buffer_t, m_used_list_node);
        if (buffer->m_lba == lba) {
            return buffer;
        }
        node = node->next();
    }

    return nullptr;
}

io_buffer_t* block_dev_t::get_block(uint32 lba)
{
    uint64 flags;
    m_lock.lock_irqsave(flags);

    /* first, find from used list, if find it means this block cached */
    io_buffer_t* b = find_from_cache(lba);

    /* find */
    if (b == nullptr) {
        /* get one from free list */
        if (!m_free_list.empty()) {
            b = list_entry(m_free_list.remove_head(), io_buffer_t, m_free_list_node);
        }
        else {
            /* get front from used list */
            b = list_entry(m_used_list.remove_head(), io_buffer_t, m_used_list_node);
        }

        b->m_lba = lba;
        b->m_done = 0;
    }
    m_lock.unlock_irqrestore(flags);

    if (b != nullptr) {
        b->lock();
    }
    return b;
}

void block_dev_t::release_block(io_buffer_t* b)
{
    uint64 flags;
    m_lock.lock_irqsave(flags);
    if (!find_from_cache(b->m_lba)) {
        m_used_list.add_tail(&b->m_used_list_node);
    }
    m_lock.unlock_irqrestore(flags);
    b->unlock();
}

io_buffer_t* block_dev_t::read(uint32 lba)
{
    lba *= 4;
    io_buffer_t* b = get_block(lba);
    if (b->m_done) {
        return b;
    }

    for (int i = 0; i < 4; i++) {
        request_t* req = (request_t *)m_request_cache.alloc();
        req->init(m_dev, lba, i, request_t::CMD_READ, b);
        os()->ide()->add_request(req);
        b->wait();
        m_request_cache.free(req);
    }

    return b;
}

int block_dev_t::write(io_buffer_t* b)
{
    request_t req;

    for (int i = 0; i < 4; i++) {
        req.init(m_dev, b->m_lba, i, request_t::CMD_WRITE, b);
        os()->ide()->add_request(&req);
        b->wait();
    }

    return 0;
}

