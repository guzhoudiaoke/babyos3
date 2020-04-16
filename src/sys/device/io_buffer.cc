/*
 *	babyos/kernel/io_buffer.h
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




#include "io_buffer.h"


void io_buffer_t::init()
{
    m_block = 0;
    m_done = 0;
    m_sem.init(1);
    m_sem_wait_done.init(0);
}

void io_buffer_t::lock()
{
    m_sem.down();
}

void io_buffer_t::unlock()
{
    m_sem.up();
}

void io_buffer_t::wait()
{
    m_sem_wait_done.down();
}

void io_buffer_t::done()
{
    m_done = 1;
    m_sem_wait_done.up();
}

