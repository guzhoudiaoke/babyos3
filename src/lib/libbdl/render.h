/*
 *	babyos/lib/libbdl/render.h
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


#ifndef _RENDER_H_
#define _RENDER_H_


#include "stdint.h"
#include "rect.h"


class surface_t;
class renderer_t {
public:
    renderer_t(surface_t* surface);
    ~renderer_t();

    void get_view_port(rect_t* rect);
    int  set_view_port(rect_t* rect);
    int  set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    uint32_t get_color();

    void clear();
    int  fill_rect(const rect_t* rect);
    int  fill_frects(const frect_t* rects, int count);

    int  draw_point(int x, int y);

private:
    void* m_pixels;
    rect_t m_viewport;
    fpoint_t m_scale;
    surface_t* m_surface;

    uint8_t m_r;
    uint8_t m_g;
    uint8_t m_b;
    uint8_t m_a;
};

#endif