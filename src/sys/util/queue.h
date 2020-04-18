/*
 *	babyos/sys/util/queue.h
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
 *  2020-04-18		created
 */


#ifndef _QUEUE_H_
#define _QUEUE_H_


#include "types.h"


typedef struct allocator_s {
    void* (*alloc) (uint32 size);
    void  (*free)  (void* p);
} allocator_t;


template<typename T>
class queue_t {
public:
    queue_t() {
        m_capacity = 0;
        m_front = 0;
        m_rear = 0;
        m_buffer = nullptr;
    }

    ~queue_t() {
        m_allocator.free(m_buffer);
        m_buffer = nullptr;
        m_capacity = 0;
    }

    void init(uint32 capacity, const allocator_t& allocator) {
        m_capacity = capacity;
        m_allocator = allocator;
        m_buffer = (T *) m_allocator.alloc(sizeof(T) * capacity);
        m_front = 0;
        m_rear = 0;
    }

    bool empty() {
        return m_front == m_rear;
    }

    bool full() {
        return (m_rear+1) % m_capacity == m_front;
    }

    bool push(const T& val) {
        if (full()) {
            return false;
        }
        m_buffer[m_rear] = val;
        m_rear = (m_rear+1) % m_capacity;
        return true;
    }
    bool pop() {
        if (empty()) {
            return false;
        }

        m_front = (m_front+1) % m_capacity;
        return true;
    }

    T& front() {
        return m_buffer[m_front];
    }

    const T& front() const {
        return m_buffer[m_front];
    }

private:
    allocator_t m_allocator;
    uint32      m_capacity;
    uint32      m_front;
    uint32      m_rear;
    T*          m_buffer;
};

#endif