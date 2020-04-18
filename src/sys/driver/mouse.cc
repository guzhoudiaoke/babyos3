/*
 *	babyos/sys/driver/mouse.cc
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

#include "mouse.h"
#include "x86.h"
#include "babyos.h"


#define KB_PORT_DATA            0x60
#define KB_PORT_STATE           0x64
#define KB_PORT_CMD             0x64

#define KB_STATE_SEND_NOT_READY 0x02

#define KB_CONTROLLER_MODE      0x47
#define KB_CMD_SET_MODE         0x60
#define KB_CMD_SENDTO_MOUSE     0xd4
#define MOUSE_CMD_ENABLE        0xf4



void* allocator_alloc(uint32 size)
{
    return os()->mm()->kmalloc(size);
}

void allocator_free(void* p)
{
    os()->mm()->kfree(p);
}

allocator_t allocator = {
    allocator_alloc,
    allocator_free
};


mouse_t::mouse_t()
{
}

mouse_t::~mouse_t()
{

}

void mouse_t::init_kb_controller()
{
    wait();
    outb(KB_PORT_CMD, KB_CMD_SET_MODE);

    wait();
    outb(KB_PORT_DATA, KB_CONTROLLER_MODE);
}

void mouse_t::wait()
{
    while (inb(KB_PORT_STATE) & KB_STATE_SEND_NOT_READY)
        ;
}

void mouse_t::enable()
{
    wait();
    outb(KB_PORT_CMD, KB_CMD_SENDTO_MOUSE);

    wait();
    outb(KB_PORT_DATA, MOUSE_CMD_ENABLE);
}

void mouse_t::do_irq()
{
    os()->uart()->kprintf("m\n");

    uint8 data = inb(0x60);
    if (!m_queue.full()) {
        m_queue.push(data);
    }

    read();
}

void mouse_t::init()
{
    m_mouse_data.phase = 0;
    m_spinlock.init();
    m_queue.init(1024, allocator);

    init_kb_controller();
    enable();

    os()->io_apic()->enable_irq(IRQ_MOUSE, 0);
}

void mouse_t::process_data(uint8 data)
{
    if (m_mouse_data.phase == 0) {
        if (data == 0xfa) {
            m_mouse_data.phase = 1;
        }
    }
    else if (m_mouse_data.phase == 1) {
        if ((data & 0xc8) != 0x08) {
            return;
        }

        m_mouse_data.data[0] = data;
        m_mouse_data.phase = 2;
    }
    else if (m_mouse_data.phase == 2) {
        m_mouse_data.data[1] = data;
        m_mouse_data.phase = 3;
    }
    else if (m_mouse_data.phase == 3) {
        m_mouse_data.data[2] = data;
        m_mouse_data.phase = 1;

        m_mouse_data.button = m_mouse_data.data[0] & 0x07;
        m_mouse_data.x = m_mouse_data.data[1];
        m_mouse_data.y = m_mouse_data.data[2];

        if ((m_mouse_data.data[0] & 0x10) != 0) {
            m_mouse_data.x |= 0xffffff00;
        }
        if ((m_mouse_data.data[0] & 0x20) != 0) {
            m_mouse_data.y |= 0xffffff00;
        }
        m_mouse_data.y = -m_mouse_data.y;

        os()->uart()->kprintf("button: %d, x: %d y: %d\n", m_mouse_data.button, m_mouse_data.x, m_mouse_data.y);

        //mouse_x_pre = mouse_x;
        //mouse_y_pre = mouse_y;
        //mouse_x += mouse_data.x;
        //mouse_y += mouse_data.y;
        //if (mouse_x < 0) mouse_x = 0;
        //if (mouse_x >= 1024-MOUSE_IMG_WIDTH) mouse_x = 1024-MOUSE_IMG_WIDTH-1;
        //if (mouse_y < 0) mouse_y = 0;
        //if (mouse_y >= 768-MOUSE_IMG_HEIGHT) mouse_y = 768-MOUSE_IMG_HEIGHT-1;

        //if (mouse_x != mouse_x_pre || mouse_y != mouse_y_pre)
        //{
        //    mouse_move();
        //}
    }
}

void mouse_t::read()
{
    uint8 data = 0;
    uint64 flags = 0;
    bool ok = false;

    m_spinlock.lock_irqsave(flags);
    if (!m_queue.empty()) {
        data = m_queue.front();
        m_queue.pop();
        ok = true;
    }
    m_spinlock.unlock_irqrestore(flags);

    if (ok) {
        process_data(data);
    }
}