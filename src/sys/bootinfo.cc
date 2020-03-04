/*
 *	babyos/kernel/bootinfo.cc
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
 *  2020-02-14		created
 */


#include "bootinfo.h"
#include "bootmem.h"


bootinfo_t::bootinfo_t()
{
}

bootinfo_t::~bootinfo_t()
{
}

void bootinfo_t::init()
{
    m_video_info = (video_info_t *) bootmem_t::early_pa2va(VIDEO_INFO_ADDR);
    m_mem_layout = (memory_layout_t *) bootmem_t::early_pa2va(MEM_INFO_ADDR);
    m_asc16_font = (void *) bootmem_t::early_pa2va(FONT_ASC16_ADDR);
}

video_info_t* bootinfo_t::video_info()
{
    return m_video_info;
}

memory_layout_t* bootinfo_t::memory_layout()
{
    return m_mem_layout;
}

void* bootinfo_t::asc16_font()
{
    return m_asc16_font;
}
