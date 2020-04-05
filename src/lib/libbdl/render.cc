/*
 *	babyos/lib/libbdl/render.cc
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


#include "math.h"
#include "assert.h"
#include <render.h>
#include <surface.h>


renderer_t::renderer_t(surface_t* surface)
{
    m_surface = surface;
    m_pixels = nullptr;
    //m_scale.x = 1.0f;
    //m_scale.y = 1.0f;
}

renderer_t::~renderer_t()
{

}

void renderer_t::get_view_port(rect_t* rect)
{
    if (rect != nullptr) {
        rect->x = m_viewport.x;
        rect->y = m_viewport.y;
        rect->h = m_viewport.h;
        rect->w = m_viewport.w;
    }
}

int renderer_t::set_view_port(rect_t* rect)
{
    if (rect != nullptr) {
        //m_viewport.x = (int) floor(rect->x * m_scale.x);
        //m_viewport.y = (int) floor(rect->y * m_scale.y);
        //m_viewport.w = (int) floor(rect->w * m_scale.x);
        //m_viewport.h = (int) floor(rect->h * m_scale.y);
    }
    else {
        m_viewport.x = 0;
        m_viewport.y = 0;
        m_viewport.w = m_surface->width();
        m_viewport.h = m_surface->height();
    }

    /* TODO: queue cmd set view port */
    return 0;
}

int renderer_t::set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    m_r = r;
    m_g = g;
    m_b = b;
    m_a = a;
    return 0;   
}

uint32_t renderer_t::get_color()
{
    return (uint32_t) m_a << 24 | (uint32_t) m_r << 16 | (uint32_t) m_g << 8 | m_b;
}

void renderer_t::clear()
{
    /* TODO: queue cmd clear */
}

int renderer_t::fill_rect(const rect_t* rect)
{
    uint32_t color = get_color();
    return m_surface->fill_rect(rect, color);

#if 0
    frect_t frect;

    if (rect != nullptr) {
        frect.x = (float) rect->x;
        frect.y = (float) rect->y;
        frect.w = (float) rect->w;
        frect.h = (float) rect->h;
    }   
    else {
        rect_t r;
        get_view_port(&r);
        frect.x = 0.0f;
        frect.y = 0.0f;
        frect.w = (float) r.w;
        frect.h = (float) r.h;
    }
    fill_frects(&frect, 1);
#endif
    return 0;
}

int renderer_t::fill_frects(const frect_t* rects, int count)
{
    assert(rects != nullptr);

    if (count < 1) {
        return 0;
    }

    frect_t* frects = new frect_t[count];
    if (frects == nullptr) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        frects[i].x = rects[i].x * m_scale.x;
        frects[i].y = rects[i].y * m_scale.y;
        frects[i].w = rects[i].w * m_scale.x;
        frects[i].h = rects[i].h * m_scale.y;
    }

    /* TODO: queue cmd fill rects */

    delete frects;
    return 0;
}

int renderer_t::draw_point(int x, int y)
{
    uint32_t color = get_color();
    return m_surface->draw_point(x, y, color);
}
