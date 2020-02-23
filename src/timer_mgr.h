/*
 *	babyos/kernel/timer_mgr.h
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




#ifndef _TIMER_MGR_H_
#define _TIMER_MGR_H_


#include "types.h"
#include "list.h"
#include "timer.h"
#include "spinlock.h"


class timer_mgr_t {
public:
    void init();
    void update();
    void add_timer(timer_t* timer);
    void remove_timer(timer_t* timer);

private:
    list_t<timer_t *>    m_timer_list;
    spinlock_t           m_timer_list_lock;
};

#endif
