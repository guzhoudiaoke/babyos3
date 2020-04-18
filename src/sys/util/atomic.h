/*
 *	babyos/kernel/atomic.h
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
 *  2020-02-15		created
 *  2020-02-16		change to 64 bit
 */


#ifndef _ATOMIC_H_
#define _ATOMIC_H_


#include "babyos/types.h"


typedef struct atomic_s {
    volatile int64 counter;
} atomic_t;


#define ATOMIC_INIT(i)	 { (i) }
#define atomic_read(v)	 ((v)->counter)
#define atomic_set(v, i) (((v)->counter) = (i))


static __inline__ void atomic_add(int64 i, atomic_t *v)
{
    __asm__ __volatile__(
            "lock; addq %1,%0"
            :"=m" (v->counter)
            :"ir" (i), "m" (v->counter));
}

static __inline__ void atomic_sub(int64 i, atomic_t *v)
{
    __asm__ __volatile__(
            "lock; subq %1,%0"
            :"=m" (v->counter)
            :"ir" (i), "m" (v->counter));
}

static __inline__ int64 atomic_sub_and_test(int64 i, atomic_t *v)
{
    unsigned char c;

    __asm__ __volatile__(
            "lock; subq %2,%0; sete %1"
            :"=m" (v->counter), "=qm" (c)
            :"ir" (i), "m" (v->counter) : "memory");
    return c;
}

static __inline__ void atomic_inc(atomic_t *v)
{
    __asm__ __volatile__(
            "lock; incq %0"
            :"=m" (v->counter)
            :"m" (v->counter));
}

static __inline__ void atomic_dec(atomic_t *v)
{
    __asm__ __volatile__(
            "lock; decq %0"
            :"=m" (v->counter)
            :"m" (v->counter));
}

static __inline__ int64 atomic_dec_and_test(atomic_t *v)
{
    unsigned char c;

    __asm__ __volatile__(
            "lock; decq %0; sete %1"
            :"=m" (v->counter), "=qm" (c)
            :"m" (v->counter) : "memory");
    return c != 0;
}

static __inline__ int64 atomic_inc_and_test(atomic_t *v)
{
    unsigned char c;

    __asm__ __volatile__(
            "lock; incq %0; sete %1"
            :"=m" (v->counter), "=qm" (c)
            :"m" (v->counter) : "memory");
    return c != 0;
}

#endif
