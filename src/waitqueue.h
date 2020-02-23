/*
 *	babyos/kernel/waitqueue.h
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



#ifndef _WAIT_QUEUE_H_
#define _WAIT_QUEUE_H_


#include "types.h"
#include "spinlock.h"
#include "list.h"


class process_t;
class wait_queue_t {
public:
    void init();
    void add(process_t* proc);
    void remove(process_t* proc);
    void wake_up();
private:
    spinlock_t              m_lock;
    list_t<process_t *>     m_procs;
};


#endif
