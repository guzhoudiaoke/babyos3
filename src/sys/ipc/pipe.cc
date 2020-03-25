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
#include "x86.h"


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
    m_reader_wq.init();
    m_writer_wq.init();
}

void pipe_t::destroy()
{
    if (m_buffer != nullptr) {
        os()->mm()->kfree(m_buffer);
        m_buffer = nullptr;
    }
}

int32 pipe_t::read(void* buf, uint32 size)
{
    os()->uart()->kprintf("%d read: %d, at %lx\n", current->m_pid, size, rdtsc());

    int n = 0;
    char* p = (char *) buf;

    uint64 flags;
    m_lock.lock_irqsave(flags);
    os()->uart()->kprintf("%d read: locked at %lx\n", current->m_pid, rdtsc());
    while (m_write_index == m_read_index && m_writable) {
        m_lock.unlock_irqrestore(flags);
        current->sleep_on(&m_reader_wq);
        m_lock.lock_irqsave(flags);
    }

    for (uint32 i = 0; i < size; i++) {
        if (m_read_index == m_write_index) {
            break;
        }
        n++;
        p[i] = m_buffer[m_read_index++ % BUFFER_SIZE];
    }

    os()->uart()->kprintf("%d read: copy data done at %lx\n", current->m_pid, rdtsc());
    m_writer_wq.wake_up();
    os()->uart()->kprintf("%d read: wake up done at %lx\n", current->m_pid, rdtsc());
    m_lock.unlock_irqrestore(flags);
    os()->uart()->kprintf("%d read: unlocked done at %lx\n", current->m_pid, rdtsc());

    os()->uart()->kprintf("%d read done: %d\n", current->m_pid, n);
    return n;
}

int32 pipe_t::write(void* buf, uint32 size)
{
    os()->uart()->kprintf("%d write: %d at %lx\n", current->m_pid, size, rdtsc());

    int n = 0;
    char* p = (char *) buf;

    uint64 flags;
    m_lock.lock_irqsave(flags);
    os()->uart()->kprintf("%d write: locked done at %lx\n", current->m_pid, rdtsc());
    for (uint32 i = 0; i < size; i++) {
        while (m_write_index == m_read_index + BUFFER_SIZE) {
            if (!m_readable) {
                m_lock.unlock_irqrestore(flags);
                return n;
            }

            m_reader_wq.wake_up();

            m_lock.unlock_irqrestore(flags);
            current->sleep_on(&m_writer_wq);
            m_lock.lock_irqsave(flags);
        }
        n++;
        m_buffer[m_write_index++ % BUFFER_SIZE] = p[i];
    }
    os()->uart()->kprintf("%d write: copy data done at %lx\n", current->m_pid, rdtsc());

    m_reader_wq.wake_up();
    os()->uart()->kprintf("%d write: wake up done at %lx\n", current->m_pid, rdtsc());
    m_lock.unlock_irqrestore(flags);
    os()->uart()->kprintf("%d write: unlocked done at %lx\n", current->m_pid, rdtsc());

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
        m_reader_wq.wake_up();
    }
    else {
        m_readable = false;
        m_writer_wq.wake_up();
    }

    if (!m_readable && !m_writable) {
        m_lock.unlock_irqrestore(flags);
        destroy();
    }
    else {
        m_lock.unlock_irqrestore(flags);
    }
}
