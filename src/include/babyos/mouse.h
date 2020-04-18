/*
 *	babyos/include/babyos/mouse.h
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
 *  2020-04-18		created
 */


#ifndef _INCLUDE_MOUSE_H_
#define _INCLUDE_MOUSE_H_


typedef enum mouse_button_s {
    LEFT   = 0x1,
    RIGHT  = 0x2,
    MIDDLE = 0x4,
} mouse_button_t;


typedef struct mouse_packet_s {
    int             dx;
    int             dy;
    mouse_button_t  button;
} mouse_packet_t;


#endif