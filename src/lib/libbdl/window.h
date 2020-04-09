/*
 *	babyos/lib/libbdl/window.h
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


#ifndef _WINDOW_H_
#define _WINDOW_H_


#include <surface.h>
#include <rect.h>


class window_t {
public:
    window_t(const char* title, int x, int y, int w, int h);
    ~window_t();

    int x();
    int y();
    int width();
    int height();

    surface_t* get_surface();
    void set_surface(surface_t* surface);

    int update_surface();
    int update_surface_rects(rect_t* rects, int numrects);

private:
    surface_t* create_frame_buffer();

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;

    char* m_title;
    surface_t* m_surface;
};


#endif