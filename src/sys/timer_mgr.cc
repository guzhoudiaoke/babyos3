/*
 *	babyos/kernel/timer_mgr.cc
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



#include "timer_mgr.h"
#include "babyos.h"


void timer_mgr_t::init()
{
    m_timer_list.init();
    m_timer_list_lock.init();
}

void timer_mgr_t::update()
{
    dlist_node_t* node = m_timer_list.head();
    while (node != NULL) {
        timer_t* timer = list_entry(node, timer_t, m_list_node);
        timer->update();
        node = node->next();
    }
}

void timer_mgr_t::add_timer(timer_t* timer)
{
    uint64 flags;
    m_timer_list_lock.lock_irqsave(flags);
    m_timer_list.add_tail(timer->get_list_node());
    m_timer_list_lock.unlock_irqrestore(flags);
}

void timer_mgr_t::remove_timer(timer_t* timer)
{
    uint64 flags;
    m_timer_list_lock.lock_irqsave(flags);
    m_timer_list.remove(timer->get_list_node());
    m_timer_list_lock.unlock_irqrestore(flags);
}

