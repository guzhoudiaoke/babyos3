/*
 *	babyos/kernel/sem.h
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


#ifndef _SEM_H_
#define _SEM_H_

#include "types.h"
#include "list.h"
#include "spinlock.h"

class process_t;
typedef struct sem_waiter_s {
    process_t*  m_proc;
    bool        m_up;
} sem_waiter_t;

class semaphore_t {
public:
    void init(uint32 count);
    void down();
    void up();

private:
    void down_common();
    void up_common();

private:
    spinlock_t              m_lock;
    uint32                  m_count;
    list_t<sem_waiter_t*>   m_wait_list;
};

#endif
