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
const color_ref_t c_cursor_color     = RGB(0xff, 0xff, 0x00);



static int console_read(inode_t* inode, void* buf, int size)
{
    return os()->console()->read(buf, size);
}

static int console_write(inode_t* inode, void* buf, int size)
{
    return os()->console()->write(buf, size);
}

/****************************************************************/

void input_buffer_t::init()
{
    m_read_index = 0;
    m_write_index = 0;
    m_edit_index = 0;
    memset(m_buffer, 0, BUFFER_SIZE);
}

void input_buffer_t::input(char ch)
{
    os()->uart()->kprintf("input: %c\n", ch);
    if (ch == '\b') {
        if (m_edit_index == m_write_index) {
            return;
        }
        m_edit_index--;
    }
    else if (ch == '\n') {
        m_buffer[m_edit_index++ % BUFFER_SIZE] = ch;
        m_write_index = m_edit_index;
        os()->console()->wakeup_reader();
    }
    else {
        m_buffer[m_edit_index++ % BUFFER_SIZE] = ch;
    }
    os()->console()->write(&ch, 1);
}

/****************************************************************/


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

    m_tick_to_update = HZ;
    m_show_cursor = true;

    m_lock.init();
    m_input_buffer.init();
    m_wait_queue.init();

    os()->get_dev(DEV_CONSOLE)->read = console_read;
    os()->get_dev(DEV_CONSOLE)->write = console_write;

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

    uint64 flags;
    m_lock.lock_irqsave(flags);

    memset(buffer, 0, c_buffer_size);
    va_list ap;
    va_start(ap, fmt);
    int total = vsprintf(buffer, fmt, ap);
    va_end(ap);

    for (int i = 0; i < total; i++) {
        putc(buffer[i], color);
    }

    m_lock.unlock_irqrestore(flags);
}

void console_t::update()
{
    if (--m_tick_to_update != 0) {
        return;
    }

    /* reset tick to update */
    m_tick_to_update = HZ;
    m_show_cursor = !m_show_cursor;
    draw_cursor();
}

void console_t::do_input(char ch)
{
    if (ch != 0) {
        m_tick_to_update = HZ;
        m_show_cursor = true;

        if (ch == '\t') {
            for (uint32 i = 0; i < (4 - m_col % 4); i++) {
                m_input_buffer.input(' ');
            }
        }
        else {
            m_input_buffer.input(ch);
        }
    }
}

int console_t::read(void* buf, int size)
{
    char *p = (char *) buf;
    int left = size;
    while (left > 0) {
        if (m_input_buffer.m_read_index == m_input_buffer.m_write_index) {
            current->sleep_on(&m_wait_queue);
        }
        char c = m_input_buffer.m_buffer[m_input_buffer.m_read_index++ % BUFFER_SIZE];
        os()->uart()->kprintf("read: %c\n", c);
        *p++ = c;
        --left;
        if (c == '\n') {
            break;
        }
    }

    return size - left;
}

int console_t::write(void* buf, int size)
{
    uint64 flags;
    m_lock.lock_irqsave(flags);
    for (int i = 0; i < size; i++) {
        putc(((char *) buf)[i], WHITE);
    }
    m_lock.unlock_irqrestore(flags);

    return size;
}

void console_t::wakeup_reader()
{
    os()->uart()->kprintf("wakeup reader\n");
    m_wait_queue.wake_up();
}

