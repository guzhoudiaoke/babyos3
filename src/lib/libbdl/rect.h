/*
 *	babyos/lib/libbdl/rect.h
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
 *  2020-04-04		created
 */


#ifndef _RECT_H_
#define _RECT_H_


typedef struct rect_s
{
    int x;
    int y;
    int w;
    int h;
} rect_t;


typedef struct frect_s
{
    float x;
    float y;
    float w;
    float h;
} frect_t;


typedef struct fpoint_s
{
    float x;
    float y;
} fpoint_t;


#endif