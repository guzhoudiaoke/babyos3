/*
 *	babyos/kernel/console.cc
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

#include "console.h"
#include "babyos.h"
#include "vbe.h"


const uint32 c_buffer_size = 1024;
const color_ref_t c_background_color = RGB(0x40, 0, 0x30);
const color_ref_t c_cursor_color = RGB(0xff, 0xff, 0x00);


console_t::console_t()
{
}
console_t::~console_t()
{
}

void console_t::init()
{
    m_row_num = os()->vbe()->height() / c_asc16_height;
    m_col_num = os()->vbe()->width() / c_asc16_width;
    m_row     = 0;
    m_col     = 0;
    m_show_cursor = true;

    draw_background();
    draw_cursor();
}

void console_t::draw_background()
{
    rect_t rc = { 0, 0, os()->vbe()->width(), os()->vbe()->height() };
    os()->vbe()->fill_rectangle(rc, c_background_color);
}

void console_t::draw_cursor()
{
    rect_t rc = {(int32)m_col*c_asc16_width,
                 (int32)m_row*c_asc16_height,
                 c_asc16_width,
                 c_asc16_height
    };
    os()->vbe()->fill_rectangle(rc, m_show_cursor ? c_cursor_color : c_background_color);
}

void console_t::scroll()
{
    if (m_row < m_row_num) {
        return;
    }

    /* scroll screen */
    os()->vbe()->scroll();

    /* clear last line */
    rect_t rc = {0,
                 (int32)(m_row-1)*c_asc16_height,
                 c_asc16_width*m_col_num,
                 c_asc16_height
    };
    os()->vbe()->fill_rectangle(rc, c_background_color);

    m_row--;
    m_col = 0;
    draw_cursor();
}

void console_t::put_char(char c, color_ref_t color)
{
    rect_t rc = { (int32)m_col*c_asc16_width,
                  (int32)m_row*c_asc16_height,
                  c_asc16_width,
                  c_asc16_height
    };
    os()->vbe()->fill_rectangle(rc, c_background_color);
    os()->vbe()->draw_asc16((char) c, m_col*c_asc16_width, m_row*c_asc16_height, color);
    m_col++;
    if (m_col == m_col_num) {
        m_row++;
        m_col = 0;
        scroll();
    }
    draw_cursor();
}

void console_t::unput_char()
{
    rect_t rc = {(int32)m_col*c_asc16_width,
                 (int32)m_row*c_asc16_height,
                 c_asc16_width,
                 c_asc16_height
    };
    os()->vbe()->fill_rectangle(rc, c_background_color);
}

void console_t::backspace()
{
    unput_char();
    if (m_col == 0) {
        if (m_row == 0) {
            return;
        }
        else {
            m_row--;
            m_col = m_col_num-1;
        }
    }
    else {
        m_col--;
    }
    unput_char();
}

void console_t::putc(int c, color_ref_t color)
{
    uint32 num;
    rect_t rc;

    switch (c) {
        case '\n':
            rc = {(int32)m_col*c_asc16_width,
                  (int32)m_row*c_asc16_height,
                  c_asc16_width,
                  c_asc16_height
            };
            os()->vbe()->fill_rectangle(rc, c_background_color);
            m_row++;
            m_col = 0;
            scroll();
            break;
        case '\t':
            num = (4 - m_col % 4);
            while (num--) {
                put_char(' ', color);
            }
            break;
        case '\b':
            backspace();
            break;
        default:
            put_char((char) c, color);
            break;
    }
    draw_cursor();
}

/* only support %d %u %x %p %c %s, and seems enough for now */
void console_t::kprintf(color_ref_t color, const char *fmt, ...)
{
    static char buffer[c_buffer_size] = {0};
    if (fmt == NULL) {
        return;
    }

    memset(buffer, 0, c_buffer_size);
    va_list ap;
    va_start(ap, fmt);
    int total = vsprintf(buffer, fmt, ap);
    va_end(ap);

    for (int i = 0; i < total; i++) {
        putc(buffer[i], color);
    }
}

