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
#include "cdev.h"


class mouse_t : public cdev_t {
public:
    mouse_t();

    void init();
    void wait();
    void do_irq();
    int  read(void* buf, int size);

    virtual void create();
    virtual void open(int flags);
    virtual void close();
    virtual uint64 read(file_descriptor_t* fd, void* buffer, uint64 size);
    virtual uint64 write(file_descriptor_t* fd, void* buffer, uint64 size);
    virtual uint64 ioctl(file_descriptor_t* fd, uint32 cmd, uint64 arg);

private:
    void init_kb_controller();
    void enable();
    void process_data(uint8 data);

private:
    spinlock_t              m_spinlock;
    uint8                   m_data[3];
    uint8                   m_phase;
    queue_t<mouse_packet_t> m_queue;
};


#endif