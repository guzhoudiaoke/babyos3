/*
 *	babyos/kernel/pipe.cc
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


#include "pipe.h"
#include "babyos.h"


void pipe_t::init()
{
    m_read_index = 0;
    m_write_index = 0;
    m_lock.init();
    m_space.init(PIPE_BUF_SIZE);
    m_item.init(0);
    m_readable = true;
    m_writable = true;
    m_buffer = (char *) os()->mm()->kmalloc(BUFFER_SIZE);
}

void pipe_t::destroy()
{
    if (m_buffer != nullptr) {
        os()->mm()->kfree(m_buffer);
        m_buffer = nullptr;
    }
}

int pipe_t::get_char(char& ch)
{
    int ret = -1;
    m_item.down();
    if (m_writable) {
        ch = m_buffer[m_read_index];
        m_read_index = (m_read_index + 1) % PIPE_BUF_SIZE;
        ret = 0;
    }
    m_space.up();

    return ret;
}

int pipe_t::put_char(char ch)
{
    int ret = -1;
    m_space.down();
    if (m_readable) {
        m_buffer[m_write_index] = ch;
        m_write_index = (m_write_index + 1) % PIPE_BUF_SIZE;
        ret = 0;
    }
    m_item.up();

    return ret;
}

int32 pipe_t::read(void* buf, uint32 size)
{
    os()->uart()->kprintf("%d read: %d\n", current->m_pid, size);

    int n = 0;
    char* p = (char *) buf;

    m_lock.lock();
    while (m_write_index == m_read_index && m_writable) {
        m_lock.unlock();
        m_item.down();
        m_lock.lock();
    }

    for (uint32 i = 0; i < size; i++) {
        if (m_read_index == m_write_index) {
            break;
        }
        n++;
        p[i] = m_buffer[m_read_index++ % BUFFER_SIZE];
    }
    m_space.up();
    m_lock.unlock();

    os()->uart()->kprintf("%d read done: %d\n", current->m_pid, n);
    return n;
}

int32 pipe_t::write(void* buf, uint32 size)
{
    os()->uart()->kprintf("%d write: %d\n", current->m_pid, size);

    int n = 0;
    char* p = (char *) buf;

    m_lock.lock();
    for (uint32 i = 0; i < size; i++) {
        while (m_write_index == m_read_index + BUFFER_SIZE) {
            if (!m_readable) {
                m_lock.unlock();
                return n;
            }

            m_lock.unlock();
            m_space.down();
            m_lock.lock();
        }
        n++;
        m_buffer[m_write_index++ % BUFFER_SIZE] = p[i];
    }

    m_item.up();
    m_lock.unlock();

    os()->uart()->kprintf("%d write done: %d\n", current->m_pid, n);
    return n;
}

void pipe_t::close(bool write_end)
{
    os()->uart()->kprintf("%d close: %d\n", current->m_pid, write_end);

    uint64 flags;
    m_lock.lock_irqsave(flags);

    if (write_end) {
        m_writable = false;
        m_item.up();
    }
    else {
        m_readable = false;
        m_space.up();
    }

    if (!m_readable && !m_writable) {
        m_lock.unlock_irqrestore(flags);
        destroy();
    }
    else {
        m_lock.unlock_irqrestore(flags);
    }
}

