/*
 *	babyos/kernel/sem.cc
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


#include "sem.h"
#include "babyos.h"
#include "process.h"


void semaphore_t::init(uint32 count)
{
    m_count = count;
    m_lock.init();
    m_wait_list.init(os()->get_obj_pool_of_size());
}

void semaphore_t::down()
{
    uint64 flags;
    m_lock.lock_irqsave(flags);
    if (m_count > 0) {
        m_count--;
    }
    else {
        down_common();
    }
    m_lock.unlock_irqrestore(flags);
}

void semaphore_t::up()
{
    uint64 flags;
    m_lock.lock_irqsave(flags);
    if (m_wait_list.empty()) {
        m_count++;
    }
    else {
        up_common();
    }
    m_lock.unlock_irqrestore(flags);
}

void semaphore_t::down_common()
{
    sem_waiter_t waiter;
    waiter.m_proc = current;
    waiter.m_up = false;

    m_wait_list.push_back(&waiter);

    while (true) {
        current->m_state = process_t::SLEEP;
        m_lock.unlock_irq();

        os()->cpu()->schedule();

        m_lock.lock_irq();
        if (waiter.m_up) {
            break;
        }
    }
}

void semaphore_t::up_common()
{
    sem_waiter_t* waiter = *m_wait_list.begin();
    m_wait_list.pop_front();
    waiter->m_up = true;
    os()->process_mgr()->wake_up_process(waiter->m_proc);
}

