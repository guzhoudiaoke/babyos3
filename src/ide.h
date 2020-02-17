/*
 *	babyos/kernel/ide.h
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


#ifndef _IDE_H_
#define _IDE_H_


#include "types.h"
#include "io_buffer.h"
#include "list.h"


#define HD_STATE_READY  0x40
#define HD_STATE_BUSY   0x80

#define IO_CMD_READ     0x20
#define IO_CMD_WRITE    0x30


class request_t {
public:
    void init(uint32 dev, uint32 lba, uint32 cmd, io_buffer_t* b);

public:
    enum {
        CMD_READ = 0,
        CMD_WRITE,
    };

    uint32          m_dev;
    uint32          m_lba;
    uint32          m_cmd;
    io_buffer_t*    m_buffer;
};


class ide_t {
public:
    ide_t();
    ~ide_t();

    void init(uint32 dev);
    void add_request(request_t* req);
    void do_irq();
    void wait();
    void do_request();
    void end_request();

private:
    spinlock_t          m_lock;
    list_t<request_t *> m_req_list;
    request_t*          m_current;
};


#endif
