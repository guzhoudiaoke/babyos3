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
    m_procs.init(os()->get_obj_pool_of_size());
}

void wait_queue_t::add(process_t* proc)
{
    locker_t locker(m_lock);
    m_procs.push_back(proc);
}

void wait_queue_t::remove(process_t* proc)
{
    locker_t locker(m_lock);
    list_t<process_t *>::iterator it = m_procs.begin();
    while (it != m_procs.end()) {
        if (*it == proc) {
            m_procs.erase(it);
            break;
        }
        it++;
    }
}

void wait_queue_t::wake_up()
{
    locker_t locker(m_lock);
    if (!m_procs.empty()) {
        list_t<process_t *>::iterator it = m_procs.begin();
        os()->process_mgr()->wake_up_process((*it));
    }
}

