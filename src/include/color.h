/*
 *	babyos/kernel/color.h
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

#ifndef _COLOR_H_
#define _COLOR_H_

#include "stddef.h"


#define RGB(r, g, b)   ( ((unsigned int) (b) << 16) | ((unsigned int) (g)) << 8 | r )
#define RGB_GET_R(rgb) ( (unsigned char) (rgb) )
#define RGB_GET_G(rgb) ( (unsigned char) ((unsigned short)rgb >> 8) )
#define RGB_GET_B(rgb) ( (unsigned char) (rgb >> 16) )

#define RED     RGB(0xff, 0x00, 0x00)
#define GREEN   RGB(0x00, 0xff, 0x00)
#define BLUE    RGB(0x00, 0x00, 0xff)
#define WHITE   RGB(0xff, 0xff, 0xff)
#define GRAY    RGB(0xcc, 0xdd, 0xcc)
#define BLACK   RGB(0x00, 0x00, 0x00)
#define PINK    RGB(0xff, 0xc0, 0xcb)
#define PURPLE  RGB(0x80, 0x00, 0x80)
#define CYAN    RGB(0x00, 0xff, 0xff)
#define YELLOW  RGB(0xff, 0xff, 0x00)

typedef struct rgb_s {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb_t;

typedef unsigned color_ref_t;


#endif
