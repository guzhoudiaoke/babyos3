/*
 *	babyos/kernel/spinlock.h
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


#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

#include "types.h"


#define local_irq_save(x)	\
    __asm__ __volatile__(   \
        "pushfq ; popq %0 ; cli" \
        :"=g" (x)           \
        : /* no input */    \
        :"memory")

#define restore_flags(x)    \
    __asm__ __volatile__(   \
        "pushq %0 ; popfq"  \
        : /* no output */   \
        :"g" (x)            \
        :"memory", "cc")

#define local_irq_restore(x) \
    restore_flags(x)


class spinlock_t {
public:
    spinlock_t();
    ~spinlock_t();

    void init();
    bool holding();
    void lock();
    void unlock();

    void lock_irq();
    void unlock_irq();

    void lock_irqsave(uint64& flags);
    void unlock_irqrestore(uint64 flags);

private:
    uint32  m_locked;
    uint32  m_flags;
};

class locker_t {
public:
    locker_t(spinlock_t& lock);
    ~locker_t();

private:
    uint64      m_flags;
    spinlock_t& m_lock;
};


#endif
