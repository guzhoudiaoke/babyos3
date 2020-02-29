/*
 *	babyos/kernel/file_table.cc
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
#include "string.h"
#include "babyos.h"
#include "pipe.h"
#include "socket.h"



void file_table_t::init()
{
    m_lock.init();
    memset(m_file_table, 0, sizeof(file_t) * MAX_FILE_NUM);
}

file_t* file_table_t::alloc()
{
    file_t* file = NULL;
    uint64 flags;
    m_lock.lock_irqsave(flags);
    for (int i = 0; i < MAX_FILE_NUM; i++) {
        if (m_file_table[i].m_ref == 0) {
            file = &m_file_table[i];
            file->m_ref = 1;
            break;
        }
    }
    m_lock.unlock_irqrestore(flags);

    return file;
}

int file_table_t::free(file_t* file)
{
    file_t f;

    uint64 flags;
    m_lock.lock_irqsave(flags);
    if (file->m_ref < 1) {
        os()->panic("ref < 1 when file free");
    }

    if (--file->m_ref > 0) {
        m_lock.unlock_irqrestore(flags);
        return 0;
    }

    f = *file;
    file->m_type = file_t::TYPE_NONE;
    m_lock.unlock_irqrestore(flags);

    if (f.m_type == file_t::TYPE_PIPE) {
        f.m_pipe->close(f.m_writeable);
        f.m_pipe->destroy();
        os()->mm()->pipe_cache()->free(f.m_pipe);
        f.m_pipe = NULL;
    }
    else if (f.m_type == file_t::TYPE_SOCKET) {
        f.m_socket->release();
    }
    else if (f.m_type == file_t::TYPE_INODE) {
        os()->fs()->put_inode(f.m_inode);
    }

    return 0;
}

file_t* file_table_t::dup_file(file_t* file)
{
    uint64 flags;
    m_lock.lock_irqsave(flags);
    if (file->m_ref < 1) {
        os()->panic("ref < 1 when file dup");
    }
    file->m_ref++;
    m_lock.unlock_irqrestore(flags);

    return file;
}

