/*
 *	babyos/kernel/keyboard.cc
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
 *  2020-02-16		created
 */


#include "keyboard.h"
#include "babyos.h"
#include "x86.h"


keyboard_t::keyboard_t()
{
}

keyboard_t::~keyboard_t()
{
}

void keyboard_t::init()
{
	os()->i8259a()->enable_irq(IRQ_KEYBOARD);		/* enable keyboard interrupt */

	m_shift_l = false;
	m_shift_r = false;
	m_leading_e0 = false;
}

char keyboard_t::parse_scan_code(uint8 scan_code)
{
	bool shift = false;
	uint32 col = 0, key = 0;

    /* set m_leading_e0 */
    if (scan_code == 0xe0) {
        m_leading_e0 = true;
        return 0;
    }

    /* key up */
    if (scan_code & 0x80) {
        key = keymap[scan_code & 0x7f][col];
        if (key == K_SHIFT_L) {
            m_shift_l = false;
        }
        if (key == K_SHIFT_R) {
            m_shift_r = false;
        }
        return 0;
    }

    shift = m_shift_l || m_shift_r;
    col = shift ? 1 : 0;
    if (m_leading_e0) {
        col = 2;
        m_leading_e0 = 0;
    }

    key = keymap[scan_code & 0x7f][col];
    if (key == K_SHIFT_L) {
        m_shift_l = true;
    }
    if (key == K_SHIFT_R) {
        m_shift_r = true;
    }
    shift = m_shift_l || m_shift_r;

    if (key == K_ENTER) {
        key = '\n';
    }
    else if (key == K_TAB) {
        key = '\t';
    }
    else if (key == K_BACKSPACE) {
        key = '\b';
    }
    else if (KEY_TYPE(key) != KT_ASCII) {
        return 0;
    }

    return key;
}

void keyboard_t::do_irq()
{
	uint8 scan_code = inb(0x60);       /* read scan code */
    char ch = parse_scan_code(scan_code);
    if (ch != '\0') {
        os()->console()->kprintf(WHITE, "%c", ch);
    }

	outb(0x20, 0x20);
}

int32 keyboard_t::read()
{
    return 0;
}

