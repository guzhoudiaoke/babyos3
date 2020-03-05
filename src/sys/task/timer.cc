/*
 *	babyos/kernel/timer.cc
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



#include "timer.h"
#include "babyos.h"
#include "x86.h"



timer_t::timer_t()
{
}

timer_t::~timer_t()
{
}

void timer_t::init(uint64 expires, uint64 data, void (*func)(uint64))
{
    m_expires = expires;
    m_data = data;
    m_function = func;
}

bool timer_t::update()
{
    if (m_expires == 0) {
        return false;
    }

    --m_expires;
    if (m_expires == 0) {
        m_function(m_data);
        return true;
    }

    return false;
}

dlist_node_t* timer_t::get_list_node()
{
    return &m_list_node;
}
