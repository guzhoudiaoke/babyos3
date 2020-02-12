/*
 *	babyos/kernel/main.cc
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
 *  2020-02-12		created
 */


#include "types.h"
#include "kernel.h"

void draw_line(uint32 y, uint32 length, uint8 r, uint8 g, uint8 b);

extern "C"
int main(void)
{
    draw_line(600, 1024, 0xff, 0xff, 0xff);
    while (1) {
    }

    return 0;
}

bool is_pixel_valid(int32 x, int32 y)
{
    video_info_t* video_info = (video_info_t *)VIDEO_INFO_ADDR;
    uint32 width = video_info->width;
    uint32 height = video_info->height;

    if (x < 0 || y < 0 || x >= width || y >= height) {
        while (1) {}
        return false;
    }

    return true;
}


void set_pixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b)
{
    video_info_t* video_info = (video_info_t *)VIDEO_INFO_ADDR;
    uint8* vram_base_addr = video_info->vram_base_addr;
    uint32 width = video_info->width;
    uint32 bytes_per_pixel = video_info->bits_per_pixel/8;

    uint8* pvram = NULL;
    if (is_pixel_valid(x, y)) {
        pvram = vram_base_addr + bytes_per_pixel*(y*width + x);
        pvram[0] = b;
        pvram[1] = g;
        pvram[2] = r;
    }
}

void draw_line(uint32 y, uint32 length, uint8 r, uint8 g, uint8 b)
{
    video_info_t* video_info = (video_info_t *)VIDEO_INFO_ADDR;
    uint32 width = video_info->width;
    for (int i = (width-length) / 2; i < (width+length) / 2; i++) {
        set_pixel(i, y, r, g, b);
    }
}



