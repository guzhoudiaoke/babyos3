/*
 *	babyos/sys/driver/mouse.h
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


#ifndef _MOUSE_H_
#define _MOUSE_H_


#include "babyos/mouse.h"
#include "queue.h"
#include "spinlock.h"


typedef struct mouse_data {
    uint8 data[3];
    uint8 phase;
    uint8 button;
    int32 x;
    int32 y;
} mouse_data_t;


class mouse_t {
public:
    mouse_t();
    ~mouse_t();

    void init();
    void wait();
    void read();
    void do_irq();

private:
    void init_kb_controller();
    void enable();
    void process_data(uint8 data);

private:
    queue_t<uint8> m_queue;
    mouse_data_t   m_mouse_data;
    spinlock_t     m_spinlock;
};


#endif