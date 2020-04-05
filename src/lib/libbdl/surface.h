/*
 *	babyos/lib/libbdl/surface.h
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


#ifndef _SURFACE_H_
#define _SURFACE_H_


#include <render.h>


class surface_t {
public:
    surface_t(void* pixels, int width, int height, int pitch);
    ~surface_t();

    renderer_t* create_software_renderer();
    int width();
    int height();

    int fill_rect(const rect_t* rect, uint32_t color);
    int fill_rects(const rect_t* rects, int count, uint32_t color);

private:

private:
    void* m_pixels;
    int m_width;
    int m_height;
    int m_pitch;
    int m_bpp;
};


#endif