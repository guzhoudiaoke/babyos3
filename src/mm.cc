/*
 *	babyos/kernel/mm.cc
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
 *  2020-02-28		created
 */


#include "mm.h"


mm_t::mm_t()
{
}

mm_t::~mm_t()
{
}

void mm_t::init()
{
    m_bootmem.init();
}

void* mm_t::boot_mem_alloc(uint32 size, bool page_align)
{
    return P2V(m_bootmem.mem_alloc(size, page_align));
}

bootmem_t* mm_t::bootmem()
{
    return &m_bootmem;
}
