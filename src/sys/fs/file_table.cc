/*
 *	babyos/kernel/file_descriptor_table.cc
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
 *  2020-02-19		created
 */



#include "file_table.h"
#include "kstring.h"
#include "babyos.h"
#include "pipe.h"
#include "socket.h"



void file_descriptor_table_t::init()
{
    m_lock.init();
    memset(m_file_descriptor_table, 0, sizeof(file_descriptor_t) * MAX_FILE_NUM);
}

file_descriptor_t* file_descriptor_table_t::alloc()
{
    file_descriptor_t* fd = nullptr;
    uint64 flags;
    m_lock.lock_irqsave(flags);
    for (int i = 0; i < MAX_FILE_NUM; i++) {
        if (m_file_descriptor_table[i].m_ref == 0) {
            fd = &m_file_descriptor_table[i];
            fd->m_ref = 1;
            break;
        }
    }
    m_lock.unlock_irqrestore(flags);

    return fd;
}

int file_descriptor_table_t::free(file_descriptor_t* fd)
{
    file_descriptor_t f;

    uint64 flags;
    m_lock.lock_irqsave(flags);
    if (fd->m_ref < 1) {
        os()->panic("ref < 1 when file free");
    }

    if (--fd->m_ref > 0) {
        m_lock.unlock_irqrestore(flags);
        return 0;
    }

    f = *fd;
    fd->m_type = file_descriptor_t::TYPE_NONE;
    m_lock.unlock_irqrestore(flags);

    if (f.m_type == file_descriptor_t::TYPE_PIPE) {
        f.m_pipe->close(f.m_writeable);
        os()->mm()->pipe_cache()->free(f.m_pipe);
        f.m_pipe = nullptr;
    }
    else if (f.m_type == file_descriptor_t::TYPE_SOCKET) {
        f.m_socket->release();
    }
    else if (f.m_type == file_descriptor_t::TYPE_INODE) {
        os()->fs()->put_inode(f.m_inode);
    }

    return 0;
}

file_descriptor_t* file_descriptor_table_t::dup_file(file_descriptor_t* fd)
{
    uint64 flags;
    m_lock.lock_irqsave(flags);
    if (fd->m_ref < 1) {
        os()->panic("ref < 1 when file dup");
    }
    fd->m_ref++;
    m_lock.unlock_irqrestore(flags);

    return fd;
}
