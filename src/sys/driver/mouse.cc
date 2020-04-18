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
#include "errno.h"


#define KB_PORT_DATA            0x60
#define KB_PORT_STATE           0x64
#define KB_PORT_CMD             0x64

#define KB_STATE_SEND_NOT_READY 0x02

#define KB_CONTROLLER_MODE      0x47
#define KB_CMD_SET_MODE         0x60
#define KB_CMD_SENDTO_MOUSE     0xd4
#define MOUSE_CMD_ENABLE        0xf4


static int mouse_read(inode_t* inode, void* buf, int size)
{
    return os()->mouse()->read(buf, size);
}

static int mouse_write(inode_t* inode, void* buf, int size)
{
    return -EPERM;
}

/*************************************************************************/


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
    uint8 data = inb(0x60);
    uint64 flags;

    m_spinlock.lock_irqsave(flags);
    process_data(data);
    m_spinlock.unlock_irqrestore(flags);
}

void mouse_t::init()
{
    m_phase = 0;
    m_spinlock.init();
    m_queue.init(1024, allocator);

    os()->get_dev(DEV_MOUSE)->read = mouse_read;
    os()->get_dev(DEV_MOUSE)->write = mouse_write;

    init_kb_controller();
    enable();

    os()->io_apic()->enable_irq(IRQ_MOUSE, 0);
}

void mouse_t::process_data(uint8 data)
{
    if (m_phase == 0) {
        if (data == 0xfa) {
            m_phase = 1;
        }
    }
    else if (m_phase == 1) {
        if ((data & 0xc8) != 0x08) {
            return;
        }

        m_data[0] = data;
        m_phase = 2;
    }
    else if (m_phase == 2) {
        m_data[1] = data;
        m_phase = 3;
    }
    else if (m_phase == 3) {
        m_data[2] = data;
        m_phase = 1;

        mouse_packet_t packet;

        packet.button = mouse_button_t(m_data[0] & 0x07);
        packet.dx = m_data[1];
        packet.dy = m_data[2];

        if ((m_data[0] & 0x10) != 0) {
            packet.dx |= 0xffffff00;
        }
        if ((m_data[0] & 0x20) != 0) {
            packet.dy |= 0xffffff00;
        }
        packet.dy = -packet.dy;

        //os()->uart()->kprintf("button: %d, x: %d y: %d\n", packet.button, packet.dx, packet.dy);

        if (m_queue.full()) {
            m_queue.pop();
        }
        m_queue.push(packet);
    }
}

int mouse_t::read(void* buf, int size)
{
    uint64 flags;
    int ret = 0;
    mouse_packet_t* p = (mouse_packet_t *) buf;

    m_spinlock.lock_irqsave(flags);
    while (!m_queue.empty() && size > 0) {
        mouse_packet_t packet = m_queue.front();
        m_queue.pop();
        memcpy(p + ret, &packet, sizeof(mouse_packet_t));
        ret += 1;
    }
    m_spinlock.unlock_irqrestore(flags);

    return ret;
}
