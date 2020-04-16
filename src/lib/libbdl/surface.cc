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
#include <math.h>


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

int surface_t::lower_blit(surface_t* src, rect_t* srcrect, rect_t* dstrect)
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
    //printf("blit: %d %d %d %d\n", src_x, srcrect->y, dst_x, dstrect->y);
    while (dst_h--) {
        memcpy(dst_pixels + m_bpp*dst_x, src_pixels + m_bpp*src_x, m_bpp*srcrect->w);
        src_pixels += src_pitch;
        dst_pixels += dst_pitch;
        //printf("%d, ", dst_h);
    }

    return 0;
}

int surface_t::blit(surface_t* src, rect_t* srcrect, rect_t* dstrect)
{
    rect_t fulldst;
    int srcx, srcy, w, h;

    if (dstrect == nullptr) {
        fulldst.x = fulldst.y = 0;
        fulldst.w = m_width;
        fulldst.h = m_height;
        dstrect = &fulldst;
    }

    if (srcrect != nullptr) {
        int maxw, maxh;

        srcx = srcrect->x;
        w = srcrect->w;
        if (srcx < 0) {
            w += srcx;
            dstrect->x -= srcx;
        }

        maxw = src->width() - srcx;
        if (maxw < w)
            w = maxw;

        srcy = srcrect->y;
        h = srcrect->h;
        if (srcy < 0) {
            h += srcy;
            dstrect->y -= srcy;
            srcy = 0;
        }
        maxh = src->height() - srcy;
        if (maxh < h) {
            h = maxh;
        }
    }
    else {
        srcx = srcy = 0;
        w = src->width();
        h = src->height();
    }

    rect_t *clip = &m_clip_rect;
    int dx, dy;

    dx = clip->x - dstrect->x;
    if (dx > 0) {
        w -= dx;
        dstrect->x += dx;
        srcx += dx;
    }
    dx = dstrect->x + w - clip->x - clip->w;
    if (dx > 0) {
        w -= dx;
    }

    dy = clip->y - dstrect->y;
    if (dy > 0) {
        h -= dy;
        dstrect->y += dy;
        srcy += dy;
    }
    dy = dstrect->y + h - clip->y - clip->h;
    if (dy > 0) {
        h -= dy;
    }

    if (w > 0 && h > 0) {
        rect_t sr;
        sr.x = srcx;
        sr.y = srcy;
        sr.w = dstrect->w = w;
        sr.h = dstrect->h = h;
        return lower_blit(src, &sr, dstrect);
    }

    dstrect->w = dstrect->h = 0;
    return 0;
}

static void copy_row(uint32_t *src, int src_w, uint32_t *dst, int dst_w)
{                                           
    uint32_t pixel = 0;

    int pos = 0x10000;
    int inc = (src_w << 16) / dst_w;
    for (int i = dst_w; i > 0; --i) {
        while (pos >= 0x10000L) {
            pixel = *src++;
            pos -= 0x10000L;
        }
        *dst++ = pixel;
        pos += inc;
    }
}

int surface_t::lower_blit_scaled(surface_t* src, rect_t* srcrect, rect_t* dstrect)
{
    int pos = 0x10000;
    int inc = (srcrect->h << 16) / dstrect->h;
    int src_row = srcrect->y;
    int dst_row = dstrect->y;

    uint8_t* dstp = nullptr;
    uint8_t* srcp = nullptr;
    int dst_maxrow = dst_row + dstrect->h;
    //printf("sw: %d sh: %d dw: %d dh: %d dmaxrow: %d, src_row:%d, dst_row: %d, sx: %d, dx: %d\n", 
    //        srcrect->w, srcrect->h, dstrect->w, dstrect->h, dst_maxrow, src_row, dst_row, srcrect->x, dstrect->x);

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

int surface_t::blit_scaled(surface_t* src, rect_t* srcrect, rect_t* dstrect)
{
    if (src == nullptr) {
        return 0;
    }

    int src_w, src_h;
    if (srcrect == nullptr) {
        src_w = src->width();
        src_h = src->height();
    }
    else {
        src_w = srcrect->w;
        src_h = srcrect->h;
    }

    int dst_w, dst_h;
    if (dstrect == nullptr) {
        dst_w = width();
        dst_h = height();
    }
    else {
        dst_w = dstrect->w;
        dst_h = dstrect->h;
    }

    if (dst_w == src_w && dst_h == src_h) {
        return blit(src, srcrect, dstrect);
    }

    double scaling_w = (double) dst_w / src_w;
    double scaling_h = (double) dst_h / src_h;

    int dst_x0, dst_y0, dst_x1, dst_y1;
    if (dstrect == nullptr) {
        dst_x0 = 0;
        dst_y0 = 0;
        dst_x1 = dst_w - 1;
        dst_y1 = dst_h - 1;
    }
    else {
        dst_x0 = dstrect->x;
        dst_y0 = dstrect->y;
        dst_x1 = dst_x0 + dst_w - 1;
        dst_y1 = dst_y0 + dst_h - 1;
    }

    int src_x0, src_y0, src_x1, src_y1;
    if (srcrect == nullptr) {
        src_x0 = 0;
        src_y0 = 0;
        src_x1 = src_w - 1;
        src_y1 = src_h - 1;
    }
    else {
        src_x0 = srcrect->x;
        src_y0 = srcrect->y;
        src_x1 = src_x0 + src_w - 1;
        src_y1 = src_y0 + src_h - 1;

        if (src_x0 < 0) {
            dst_x0 -= src_x0 * scaling_w;
            src_x0 = 0;
        }

        if (src_x1 >= src->width()) {
            dst_x1 -= (src_x1 - src->width() + 1) * scaling_w;
            src_x1 = src->width() - 1;
        }

        if (src_y0 < 0) {
            dst_y0 -= src_y0 * scaling_h;
            src_y0 = 0;
        }

        if (src_y1 >= src->height()) {
            dst_y1 -= (src_y1 - src->height() + 1) * scaling_h;
            src_y1 = src->height() - 1;
        }
    }

    dst_x0 -= m_clip_rect.x;
    dst_x1 -= m_clip_rect.x;
    dst_y0 -= m_clip_rect.y;
    dst_y1 -= m_clip_rect.y;

    if (dst_x0 < 0) {
        src_x0 -= dst_x0 / scaling_w;
        dst_x0 = 0;
    }

    if (dst_x1 >= m_clip_rect.w) {
        src_x1 -= (dst_x1 - m_clip_rect.w + 1) / scaling_w;
        dst_x1 = m_clip_rect.w - 1;
    }

    if (dst_y0 < 0) {
        src_y0 -= dst_y0 / scaling_h;
        dst_y0 = 0;
    }

    if (dst_y1 >= m_clip_rect.h) {
        src_y1 -= (dst_y1 - m_clip_rect.h + 1) / scaling_h;
        dst_y1 = m_clip_rect.h - 1;
    }

    dst_x0 += m_clip_rect.x;
    dst_x1 += m_clip_rect.x;
    dst_y0 += m_clip_rect.y;
    dst_y1 += m_clip_rect.y;

    rect_t final_src, final_dst;
    final_src.x = (int) floor(src_x0 + 0.5);
    final_src.y = (int) floor(src_y0 + 0.5);
    final_src.w = (int) floor(src_x1 + 1 + 0.5) - (int) floor(src_x0 + 0.5);
    final_src.h = (int) floor(src_y1 + 1 + 0.5) - (int) floor(src_y0 + 0.5);

    final_dst.x = (int) floor(dst_x0 + 0.5);
    final_dst.y = (int) floor(dst_y0 + 0.5);
    final_dst.w = (int) floor(dst_x1 - dst_x0 + 1.5);
    final_dst.h = (int) floor(dst_y1 - dst_y0 + 1.5);

    if (final_dst.w < 0) {
        final_dst.w = 0;
    }
    if (final_dst.h < 0) {
        final_dst.h = 0;
    }

    if (dstrect) {
        *dstrect = final_dst;
    }
    
    if (final_dst.w == 0 || final_dst.h == 0 || final_src.w <= 0 || final_src.h <= 0) {
        return 0;
    }

    return lower_blit_scaled(src, &final_src, &final_dst);
}
