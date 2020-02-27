/*
 *	babyos/kernel/ide.cc
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



#include "ide.h"
#include "babyos.h"
#include "x86.h"


void request_t::init(uint32 dev, uint32 lba, uint32 cmd, io_buffer_t* b)
{
    m_dev = dev;
    m_lba = lba;
    m_cmd = cmd;
    m_buffer = b;
}

/***********************************************************/

ide_t::ide_t()
{
}

ide_t::~ide_t()
{
}

void ide_t::init(uint32 dev)
{
    m_lock.init();
    m_req_list.init(os()->get_obj_pool_of_size());
    m_current = NULL;

    //os()->i8259a()->enable_irq(IRQ_HARDDISK);
    os()->io_apic()->enable_irq(IRQ_HARDDISK, 0);
    wait();
    outb(0x1f6, 0xe0 | (dev << 4));
}

void ide_t::add_request(request_t* req)
{
    if (m_current == NULL) {
        m_current = req;
        do_request();
    }
    else {
        uint64 flags;
        m_lock.lock_irqsave(flags);
        m_req_list.push_back(req);
        os()->uart()->puts("push_back\n");
        m_lock.unlock_irqrestore(flags);
    }
}

void ide_t::do_request()
{
    if (m_current == NULL) {
        return;
    }

    uint32 lba = m_current->m_lba;
    wait();

    outb(0x3f6, 0);     // generate interrupt
    outb(0x1f2, 1);     // sector num
    outb(0x1f3, lba & 0xff);
    outb(0x1f4, (lba >> 8)  & 0xff);
    outb(0x1f5, (lba >> 16) & 0xff);
    outb(0x1f6, 0xe0 | ((m_current->m_dev & 0x1) << 4) | ((lba >> 24) & 0xff));

    if (m_current->m_cmd == request_t::CMD_READ) {
        outb(0x1f7, IO_CMD_READ);
    }
    else {
        outb(0x1f7, IO_CMD_WRITE);
        outsl(0x1f0, m_current->m_buffer->m_buffer, SECT_SIZE/4);
    }
}

void ide_t::end_request()
{
    if (m_current->m_cmd == request_t::CMD_READ) {
        insl(0x1f0, m_current->m_buffer->m_buffer, SECT_SIZE/4);
    }

    m_current->m_buffer->done();

    m_current = NULL;
    if (!m_req_list.empty()) {
        m_current = *m_req_list.begin();
        m_req_list.pop_front();
    }

    /* EOI */
    //outb(0x20, 0x20);
    //outb(0xa0, 0x20);
    os()->cpu()->local_apic()->eoi();

    do_request();
}

void ide_t::wait()
{
    while ((inb(0x1f7) & (HD_STATE_BUSY | HD_STATE_READY)) != HD_STATE_READY) {
        ;
    }
}

void ide_t::do_irq()
{
    end_request();
}


