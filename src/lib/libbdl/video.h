/*
 *	babyos/lib/libbdl/video.h
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


#ifndef _VIDEO_H_
#define _VIDEO_H_


#include <stdint.h>
#include <window.h>
#include <surface.h>
#include <texture.h>
#include <video_device.h>
#include <cxx.h>


class video_t {
public:
    video_t();
    ~video_t();

    static int  init();
    static void quit();
    static video_device_t* device();

    static window_t* create_window(const char* title, int x, int y, int w, int h, uint32_t flags);
    static void destroy_window(window_t* window);

    static surface_t* create_rgb_surface_from(void* pixels, int width, int height, int pitch);
    static surface_t* create_surface_from_bmp(const char* path);
    static surface_t* create_surface_from_bmp(const char* path, const char* mask);
    static surface_t* create_surface_from_png(const char* path);
    static texture_t* create_texture_from_surface(surface_t* surface, renderer_t* renderer);
    static texture_t* load_texture_bmp(const char* path, renderer_t* renderer);
    static texture_t* load_texture_bmp(const char* path, const char* mask, renderer_t* renderer);
    static texture_t* load_texture_png(const char* path, renderer_t* renderer);

    static uint32_t make_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

private:
    static video_device_t* m_device;
};

#endif