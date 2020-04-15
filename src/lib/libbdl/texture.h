/*
 *	babyos/lib/libbdl/texture.h
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


#ifndef _TEXTURE_H_
#define _TEXTURE_H_


#include <surface.h>


class texture_t {
public:
    texture_t(int w, int h, surface_t* surface);
    ~texture_t();

    int width() { return m_width; }
    int height() { return m_height; }
    surface_t* get_surface() { return m_surface; }

private:
    int         m_width;
    int         m_height;
    surface_t*  m_surface;
};


#endif