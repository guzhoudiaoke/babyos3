/*
 *	babyos/lib/libbdl/surface.cc
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


#include <stdio.h>
#include <stdinc.h>
#include <string.h>
#include <surface.h>


surface_t::surface_t(void* pixels, int width, int height, int pitch)
{
    m_pixels = pixels;
    m_width = width;
    m_height = height;
    m_pitch = pitch;
    m_bpp = 4;

    m_clip_rect.x = 0;
    m_clip_rect.y = 0;
    m_clip_rect.w = width;
    m_clip_rect.h = height;
}

surface_t::~surface_t()
{

}

renderer_t* surface_t::create_renderer(window_t* window)
{
    renderer_t* renderer = new renderer_t(window, this);
    if (renderer == nullptr) {
        return nullptr;
    }

    renderer->set_view_port(nullptr);
    return renderer;
}

int surface_t::width()
{
    return m_width;
}

int surface_t::height()
{
    return m_height;
}

void* surface_t::pixels()
{
    return m_pixels;
}

int surface_t::fill_rect(const rect_t* rect, uint32_t color)
{
    rect_t clipped;

    if (rect != nullptr) {
        if (!rect->intersect(m_clip_rect, clipped)) {
            return 0;
        }
        rect = &clipped;
    }
    else {
        rect = &m_clip_rect;
    }

    if (m_pixels == nullptr) {
        printf("m_pixels is null will not draw anything!\n");
        return -1;
    }

    uint8_t* pixels = (uint8_t *) m_pixels + rect->y * m_pitch + 
                                             rect->x * m_bpp;

    int h = rect->h;
    while (h--) {
        memset4(pixels, color, rect->w);
        pixels += m_pitch;
    }

    return 0;
}

int surface_t::fill_rects(const rect_t* rects, int count, uint32_t color)
{
    if (rects == nullptr) {
        return -1;
    }

    int status = 0;
    for (int i = 0; i < count; i++) {
        status += fill_rect(&rects[i], color);
    }

    return status;
}

int surface_t::draw_point(int x, int y, uint32_t color)
{
    int minx, miny, maxx, maxy;

    minx = 0;
    miny = 0;
    maxx = m_width-1;
    maxy = m_height-1;
    
    if (x < minx || x > maxx || y < miny || y > maxy) {
        return 0;;
    }

    *((uint32_t *) ((uint8_t *) m_pixels + y*m_pitch + x*m_bpp)) = color;
    return 0;
}

int surface_t::draw_points(point_t* points, int count, uint32_t color)
{
    int minx, miny, maxx, maxy;

    minx = 0;
    miny = 0;
    maxx = m_width-1;
    maxy = m_height-1;

    for (int i = 0; i < count; i++) {
        int x = points[i].x;
        int y = points[i].y;

        if (x < minx || x > maxx || y < miny || y > maxy) {
            continue;
        }

        *((uint32_t *) ((uint8_t *) m_pixels + y*m_pitch + x*m_bpp)) = color;
    }

    return 0;
}

int surface_t::blit(surface_t* src, rect_t* srcrect, rect_t* dstrect)
{
    if (srcrect->w != dstrect->w || srcrect->h != dstrect->h) {
        return -1;
    }

    int src_pitch = src->m_pitch;
    int dst_pitch = m_pitch;

    int src_x = srcrect->x;
    int dst_x = dstrect->x;

    uint8_t* src_pixels = (uint8_t *) src->m_pixels + src_pitch*srcrect->y;
    uint8_t* dst_pixels = (uint8_t *) m_pixels + dst_pitch*dstrect->y;

    int dst_h = dstrect->h;
    while (dst_h--) {
        memcpy(dst_pixels + m_bpp*dst_x, src_pixels + m_bpp*src_x, m_bpp*srcrect->w);
        src_pixels += src_pitch;
        dst_pixels += dst_pitch;
    }

    return 0;
}

static void copy_row(uint32_t *src, int src_w, uint32_t *dst, int dst_w)
{                                           
    int i;
    int pos, inc;
    uint32_t pixel = 0;

    pos = 0x10000;
    inc = (src_w << 16) / dst_w;
    for (i = dst_w; i > 0; --i) {
        while (pos >= 0x10000L) {
            pixel = *src++;
            pos -= 0x10000L;
        }
        *dst++ = pixel;
        pos += inc;
    }
}

int surface_t::blit_scaled(surface_t* src, rect_t* srcrect, rect_t* dstrect)
{
    int pos = 0x10000;
    int inc = (srcrect->h << 16) / dstrect->h;
    int src_row = srcrect->y;
    int dst_row = dstrect->y;

    uint8_t* dstp = nullptr;
    uint8_t* srcp = nullptr;
    int dst_maxrow = dst_row + dstrect->h;
    for (; dst_row < dst_maxrow; ++dst_row) {
        dstp = (uint8_t *) m_pixels + (dst_row * m_pitch) + (dstrect->x * m_bpp);
        while (pos >= 0x10000L) {
            srcp = (uint8_t *) src->m_pixels + (src_row * src->m_pitch) + (srcrect->x * m_bpp);
            ++src_row;
            pos -= 0x10000L;
        }

        copy_row((uint32_t *) srcp, srcrect->w, (uint32_t *) dstp, dstrect->w);
        pos += inc;
    }

    return 0;
}