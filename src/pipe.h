/*
 *	babyos/kernel/pipe.h
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
 *  2020-02-26		created
 */


#ifndef _PIPE_H_
#define _PIPE_H_


#include "types.h"
#include "sem.h"
#include "spinlock.h"


#define PIPE_BUF_SIZE 512


class pipe_t {
public:
    void  init();
    int   get_char(char& ch);
    int   put_char(char ch);

    int32 read(void* buf, uint32 size);
    int32 write(void* buf, uint32 size);
    void  close(bool write_end);

private:
    char        m_buffer[PIPE_BUF_SIZE];
    uint32      m_read_index;
    uint32      m_write_index;
    spinlock_t  m_lock;
    semaphore_t m_space;    // how many space can use to put
    semaphore_t m_item;     // how many item can get

    bool        m_readable;
    bool        m_writable;
};

#endif
