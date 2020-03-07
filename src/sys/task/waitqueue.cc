/*
 *	babyos/kernel/waitqueue.cc
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


#include "waitqueue.h"
#include "process.h"
#include "babyos.h"


void wait_queue_t::init()
{
    m_lock.init();
    m_procs.init();
}

void wait_queue_t::add(process_t* proc)
{
    os()->uart()->kprintf("wake add %p %d %s\n", proc, proc->m_pid, proc->m_name);
    locker_t locker(m_lock);
    m_procs.add_tail(&proc->m_wq_list_node);
}

void wait_queue_t::remove(process_t* proc)
{
    locker_t locker(m_lock);
    m_procs.remove(&proc->m_wq_list_node);
}

void wait_queue_t::wake_up()
{
    os()->uart()->kprintf("wake up1\n");
    locker_t locker(m_lock);
    os()->uart()->kprintf("wake up2\n");
    if (!m_procs.empty()) {
        process_t* p = list_entry(m_procs.head(), process_t, m_wq_list_node);
        os()->uart()->kprintf("wake up3 :%p\n", p);
        os()->process_mgr()->wake_up_process(p);
    }
}

