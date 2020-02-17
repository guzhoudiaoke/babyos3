/*
 *	babyos/kernel/spinlock.cc
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


#include "spinlock.h"
#include "x86.h"
#include "babyos.h"

spinlock_t::spinlock_t()
{
}
spinlock_t::~spinlock_t()
{
}

void spinlock_t::init()
{
    m_locked = 0;
}

uint32 spinlock_t::holding()
{
    return (m_locked == 1);
}

void spinlock_t::lock()
{
    while (xchg(&m_locked, 1) != 0)
        ;

    __sync_synchronize();
    wmb();
}

void spinlock_t::unlock()
{
    if (!holding()) {
        os()->console()->kprintf(RED, "Not holding the lock when try to unlock\n");
        return;
    }

    __sync_synchronize();
    wmb();

    __asm__ volatile("movl $0, %0" : "+m" (m_locked));
}

void spinlock_t::lock_irq()
{
    cli();
    lock();
}

void spinlock_t::unlock_irq()
{
    unlock();
    sti();
}

void spinlock_t::lock_irqsave(uint64& flags)
{
    uint64 f;
    local_irq_save(f);
    lock();
    flags = f;
}

void spinlock_t::unlock_irqrestore(uint64 flags)
{
    unlock();
    local_irq_restore(flags);
}

/********************************************************************************************/

locker_t::locker_t(spinlock_t& lock) : m_lock(lock)
{
    m_lock.lock_irqsave(m_flags);
}

locker_t::~locker_t()
{
    m_lock.unlock_irqrestore(m_flags);
}

/********************************************************************************************/
