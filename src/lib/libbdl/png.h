/*
 *	babyos/lib/libbdl/png.h
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


#ifndef _PNG_H_
#define _PNG_H_


#include <stdint.h>
#include <window.h>
#include <surface.h>
#include <texture.h>
#include <video_device.h>
#include <cxx.h>
#include "../libpng/png.h"



class png_t {
public:
    png_t();
    ~png_t();

    bool load(const char* path);

    uint32_t width();
    uint32_t height();
    uint32_t pitch();
    uint8_t* pixels();

private:
    bool load_rgba(png_structp png_ptr, png_infop info_ptr);
    bool load_rgb(png_structp png_ptr, png_infop info_ptr);

private:
    uint32_t        m_width;
    uint32_t        m_height;
    uint32_t        m_pitch;
    uint8_t*        m_pixels;
};


#endif