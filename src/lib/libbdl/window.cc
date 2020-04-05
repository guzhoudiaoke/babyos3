/*
 *	babyos/lib/libbdl/window.cc
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

#include "string.h"
#include <window.h>
#include <video.h>


window_t::window_t(const char* title, int x, int y, int w, int h)
{
    m_x = x;
    m_y = y;
    m_width  = w;
    m_height = h;
    m_title = new char[strlen(title) + 1];
    strcpy(m_title, title);
    m_surface = nullptr;
}

window_t::~window_t()
{
    delete m_title;
}

surface_t* window_t::get_surface()
{
    if (m_surface == nullptr) {
        m_surface = create_frame_buffer();
    }
    return m_surface;
}

surface_t* window_t::create_frame_buffer()
{
    void* pixels;
    int pitch;

    if (video_t::device()->create_window_frame_buffer(this, &pixels, &pitch) < 0) {
        return nullptr;
    }

    return video_t::create_rgb_surface_from(pixels, m_width, m_height, pitch);
}

int window_t::update_surface()
{
    rect_t full_rect;
    full_rect.x = 0;
    full_rect.y = 0;
    full_rect.w = m_width;
    full_rect.h = m_height;
    return update_surface_rects(&full_rect, 1);
}

int window_t::update_surface_rects(rect_t* rects, int numrects)
{
    return video_t::device()->update_window_frame_buffer(this, rects, numrects);
}
