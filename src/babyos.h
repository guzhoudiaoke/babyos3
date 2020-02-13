/*
 *	babyos/kernel/babyos.h
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

#ifndef _BABYOS_H_
#define _BABYOS_H_

#include "types.h"
#include "uart.h"
#include "vbe.h"
#include "console.h"

class babyos_t {
public:
    static babyos_t* get_instance();

    babyos_t();
    ~babyos_t();

    void init();
    void run();

    uart_t*    uart();
    vbe_t*     vbe();
    console_t* console();

private:
    uart_t    m_uart;
    vbe_t     m_vbe;
    console_t m_console;
};

#define os() babyos_t::get_instance()

#endif
