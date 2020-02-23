/*
 *	babyos/kernel/uart.cc
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

#include "uart.h"
#include "x86.h"
#include "delay.h"
#include <stdarg.h>
#include "string.h"


#define COM1   0x3f8
const uint32 c_buffer_size = 1024;


uart_t::uart_t()
{
}

uart_t::~uart_t()
{
}

void uart_t::early_init()
{
    this->exist = false;

    /* turn off the FIFO */
    outb(COM1+2, 0);

    /* 9600 baud, 8 data bits, 1 stop bit, parity off. */
    outb(COM1+3, 0x80);
    outb(COM1+0, 115200/9600);
    outb(COM1+1, 0);
    outb(COM1+3, 0x3);
    outb(COM1+4, 0);
    outb(COM1+1, 0x01);

    if (inb(COM1+5) == 0xff) {
        return;
    }

    this->exist = true;
    m_lock.init();
}

void uart_t::init()
{

}

void uart_t::putc(int c)
{
    if (!this->exist) {
        return;
    }

    for (int i = 0; i < 128 && !(inb(COM1+5) & 0x20); i++) {
        delay_t::us_delay(10);
    }

    outb(COM1+0, c);
}

void uart_t::puts(const char* s)
{
    uint64 flag = 0;
    m_lock.lock_irqsave(flag);

    const char* p = s;
    for(; *p != '\0'; p++) {
        putc(*p);
    }

    m_lock.unlock_irqrestore(flag);
}

/* only support %d %u %x %p %c %s, and seems enough for now */
void uart_t::kprintf(const char *fmt, ...)
{
    static char buffer[c_buffer_size] = {0};
    if (fmt == NULL) {
        return;
    }

    uint64 flag = 0;
    m_lock.lock_irqsave(flag);

    memset(buffer, 0, c_buffer_size);
    va_list ap;
    va_start(ap, fmt);
    int total = vsprintf(buffer, fmt, ap);
    va_end(ap);

    for (int i = 0; i < total; i++) {
        putc(buffer[i]);
    }

    m_lock.unlock_irqrestore(flag);
}
