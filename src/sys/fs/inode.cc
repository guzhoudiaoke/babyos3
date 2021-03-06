/*
 *	babyos/kernel/inode.cc
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



#include "inode.h"
#include "babyos.h"
#include "block_dev.h"
#include "kstring.h"


void inode_t::init(uint16 major, uint16 minor, uint16 nlink)
{
    m_major = major;
    m_minor = minor;
    m_nlinks = nlink;
    m_size = 0;
    for (int i = 0; i < NDIRECT+1; i++) {
        m_addrs[i] = 0;
    }

    m_sem.init(1);
}

void inode_t::lock()
{
    m_sem.down();
}

void inode_t::unlock()
{
    m_sem.up();
}

void inode_t::read_from_disk(int id)
{
    if (m_valid == 0) {
        unsigned block = os()->fs()->inode_block(id);
        unsigned offset = os()->fs()->inode_offset(id);

        disk_inode_t dinode;
        io_buffer_t* b = os()->block_dev()->read(block);
        memcpy(&dinode, b->m_buffer + offset*sizeof(disk_inode_t), sizeof(disk_inode_t));
        os()->block_dev()->release_block(b);

        m_type = dinode.m_type;
        m_major = dinode.m_major;
        m_minor = dinode.m_minor;
        m_nlinks = dinode.m_nlinks;
        m_size = dinode.m_size;
        memcpy(m_addrs, dinode.m_addrs, sizeof(uint32) * (NDIRECT + 1));
        m_valid = 1;
    }
}

void inode_t::write_to_disk()
{
    struct disk_inode_t* disk_inode;
    unsigned block = (m_inum / (BSIZE / sizeof(disk_inode_t)) + 2);
    unsigned offset = m_inum % (BSIZE / sizeof(disk_inode_t));

    io_buffer_t* b = os()->block_dev()->read(block);
    disk_inode = (disk_inode_t *) b->m_buffer + offset;
    disk_inode->m_type = m_type;
    disk_inode->m_major = m_major;
    disk_inode->m_minor = m_minor;
    disk_inode->m_nlinks = m_nlinks;
    disk_inode->m_size = m_size;
    memcpy(disk_inode->m_addrs, m_addrs, sizeof(m_addrs));
    os()->block_dev()->write(b);
    os()->block_dev()->release_block(b);
}

