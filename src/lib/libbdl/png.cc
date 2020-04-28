/*
 *	babyos/lib/libbdl/png.cc
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
 *  2020-04-27		created
 */


#include "png.h"
#include "stdio.h"
#include "string.h"
#include <cxx.h>
#include <video.h>


png_t::png_t()
{
    m_width = 0;
    m_height = 0;
    m_pixels = nullptr;
}

png_t::~png_t()
{
    if (m_pixels) {
        delete []m_pixels;
    }
}

bool png_t::load_rgba(png_structp png_ptr, png_infop info_ptr)
{
    m_width = png_get_image_width(png_ptr, info_ptr);
    m_height = png_get_image_height(png_ptr, info_ptr);
    m_pixels = new uint8_t[m_width * m_height * 4];
    m_pitch = m_width * 4;

    png_bytep* rows = png_get_rows(png_ptr, info_ptr);
    uint32_t* p = (uint32_t *) m_pixels;

    uint32_t index = 0;
    for (uint32_t y = 0; y < m_height; y++) {
        for (uint32_t x = 0; x < m_width*4; x += 4) {
            p[index++] = video_t::make_color(rows[y][x+0], rows[y][x+1], rows[y][x+2], rows[y][x+3]);
        }
    }

    return true;
}

bool png_t::load_rgb(png_structp png_ptr, png_infop info_ptr)
{
    m_width = png_get_image_width(png_ptr, info_ptr);
    m_height = png_get_image_height(png_ptr, info_ptr);
    m_pixels = new uint8_t[m_width * m_height * 4];
    m_pitch = m_width * 4;

    png_bytep* rows = png_get_rows(png_ptr, info_ptr);
    uint32_t* p = (uint32_t *) m_pixels;

    uint32_t index = 0;
    for (uint32_t y = 0; y < m_height; y++) {
        for (uint32_t x = 0; x < m_width*3; x += 3) {
            p[index++] = video_t::make_color(rows[y][x+0], rows[y][x+1], rows[y][x+2], 0xff);
        }
    }

    return true;
}

bool png_t::load(const char* path)
{
    bool ret = false;

    if (path == nullptr) {
        printf("error path\n");
        return ret;
    }

    FILE* fp = fopen(path, "r");
    if (fp == nullptr) {
        printf("open failed\n");
        return ret;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    png_init_io(png_ptr, fp);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

    png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);
    if (color_type == PNG_COLOR_TYPE_RGBA) {
        ret = load_rgba(png_ptr, info_ptr);
    }
    else if (color_type == PNG_COLOR_TYPE_RGB) {
        ret = load_rgb(png_ptr, info_ptr);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    fclose(fp);

    return ret;
}

uint32_t png_t::width()
{
    return m_width;
}

uint32_t png_t::height()
{
    return m_height;
}

uint32_t png_t::pitch()
{
    return m_pitch;
}

uint8_t* png_t::pixels()
{
    return m_pixels;
}