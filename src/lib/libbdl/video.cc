/*
 *	babyos/lib/libbdl/video.cc
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


#include <video.h>
#include <render.h>
#include <rect.h>
#include <bitmap.h>


video_device_t* video_t::m_device = nullptr;


video_t::video_t()
{
    m_device = nullptr;
}

video_t::~video_t()
{

}

int video_t::init()
{
    if (m_device != nullptr) {
        quit();
    }

    /* TODO: ticks init */

    /* TODO: sub system init: keyboard, mouse .. */

    m_device = new video_device_t();

    return 0;
}

void video_t::quit()
{

}

video_device_t* video_t::device()
{
    return m_device;
}

window_t* video_t::create_window(const char* title, int x, int y, int w, int h, uint32_t flags)
{
    if (m_device == nullptr) {
        if (init() < 0) {
            return nullptr;
        }
    }

    if (w < 1) {
        w = 1;
    }
    if (h < 1) {
        h = 1;
    }

    window_t* window = new window_t(title, x, y, w, h);
    if (window == nullptr) {
        return nullptr;
    }

    if (m_device->create_window(window) < 0) {
        destroy_window(window);
        return nullptr;
    }

    return window;
}

void video_t::destroy_window(window_t* window)
{

}

uint32_t video_t::make_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return (uint32_t) a << 24 | (uint32_t) r << 16 | (uint32_t) g << 8 | b;
}

surface_t* video_t::create_rgb_surface_from(void* pixels, int width, int height, int pitch)
{
    if (width <= 0 || height <= 0) {
        return nullptr;
    }

    surface_t* surface = new surface_t(pixels, width, height, pitch);
    return surface;
}

surface_t* video_t::create_surface_from_bitmap(const char* path)
{
    bitmap_t bmp;
    if (!bmp.load(path)) {
        return nullptr;
    }

    int32_t pitch = 4 * bmp.width();
    uint8_t* pixels = new uint8_t[pitch * bmp.height()];

    uint32_t* dst = (uint32_t *) pixels;
    uint8_t* src = bmp.pixels();
    for (int32_t y = bmp.height()-1; y >= 0; y--) {
        uint8_t* s = src + y * bmp.pitch();
        for (uint32_t x = 0; x < bmp.width(); x++) {
            uint8_t r = s[2], g = s[1], b = s[0];
            uint32_t color = make_color(r, g, b, 0xff);
            dst[x] = color;
            s += 3;
        }
        dst += bmp.width();
    }

    surface_t* surface = new surface_t(pixels, bmp.width(), bmp.height(), pitch);
    return surface;
}
