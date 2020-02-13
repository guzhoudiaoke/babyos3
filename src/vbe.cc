/*
 *	babyos/kernel/vbe.cc
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
 *  2020-02-13		created
 */


#include "vbe.h"
#include "types.h"
#include "kernel.h"
#include "string.h"
#include "mm.h"


vbe_t::vbe_t()
{
}

vbe_t::~vbe_t()
{
}

void vbe_t::init()
{
    video_info_t *info = (video_info_t *) PA2VA(VIDEO_INFO_ADDR);
    m_width    = info->width;
    m_height   = info->height;
    m_bytes_pp = info->bits_per_pixel / 8;
    m_base     = (uint8 *)PA2VA(info->vram_base_addr);
    m_asc16_addr = (uint8 *) PA2VA(FONT_ASC16_ADDR);
}

uint32 vbe_t::width()
{
    return m_width;
}

uint32 vbe_t::height()
{
    return m_height;
}

uint8* vbe_t::vram()
{
    return m_base;
}

void vbe_t::set_pixel(uint32 x, uint32 y, color_ref_t color)
{
    if (x < m_width && y < m_height) {
        uint8* pvram = m_base + m_bytes_pp*y*m_width + m_bytes_pp*x;
        pvram[0] = RGB_GET_B(color);
        pvram[1] = RGB_GET_G(color);
        pvram[2] = RGB_GET_R(color);
    }
}

void vbe_t::draw_asc16(char ch, uint32 left, uint32 top, color_ref_t color)
{
    uint8* p_asc = m_asc16_addr + ch * c_asc16_size;

    for (int32 y = 0; y < c_asc16_height; y++) {
        uint8 test_bit = 1 << 7;
        for (int32 x = 0; x < c_asc16_width; x++) {
            if (*p_asc & test_bit) {
                set_pixel(left+x, top+y, color);
            }

            test_bit >>= 1;
        }
        p_asc++;
    }
}

void vbe_t::fill_rectangle(rect_t rect, color_ref_t color)
{
    for (uint32 y = 0; y < rect.height; ++y)
    {
        for (uint32 x = 0; x < rect.width; ++x) {
            set_pixel(rect.left + x, rect.top + y, color);
        }
    }
}

void vbe_t::scroll()
{
    memmov(m_base,
           m_base + m_width*m_bytes_pp*c_asc16_height,
           m_width*m_bytes_pp*(m_height-c_asc16_height));
}
