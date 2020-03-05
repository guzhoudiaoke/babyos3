/*
 *	babyos/kernel/console.h
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

#ifndef _CONSOLE_H_
#define _CONSOLE_H_


#include "types.h"
#include "color.h"
#include "string.h"
#include "spinlock.h"
#include "waitqueue.h"


#define BUFFER_SIZE     1024
typedef struct input_buffer_s {
    void init();
    void input(char ch);

    unsigned    m_read_index;
    unsigned    m_write_index;
    unsigned    m_edit_index;
    char        m_buffer[BUFFER_SIZE];
} input_buffer_t;



class console_t {
public:
	console_t();
	~console_t();

	void init();
	void kprintf(color_ref_t color, const char *fmt, ...);
    void update();
    void do_input(char ch);
    int  read(void* buf, int size);
    int  write(void* buf, int size);
	void putc(int c, color_ref_t color);
    void wakeup_reader();

private:
	void draw_background();
    void draw_cursor();
    void draw_time();

	void put_char(char c, color_ref_t color);
    void unput_char();
    void backspace();
	void print_int(int32 n, int32 base, int32 sign, color_ref_t color);
    void scroll();

	uint32          m_row_num;
	uint32          m_col_num;
	uint32          m_row;
	uint32          m_col;
    uint32          m_tick_to_update;
    bool            m_show_cursor;
    spinlock_t      m_lock;
    input_buffer_t  m_input_buffer;
    wait_queue_t    m_wait_queue;
};


#endif
