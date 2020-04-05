/*
 *	babyos/lib/libbdl/video_device.cc
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


#include "unistd.h"
#include "fb_ioctl.h"
#include "stdio.h"
#include <video_device.h>


video_device_t::video_device_t()
{

}

video_device_t::~video_device_t()
{

}

int video_device_t::create_window(window_t* window)
{
    return 0;
}


int video_device_t::update_window_frame_buffer(window_t* window, rect_t* rects, int numrects)
{
    return 0;
}

int video_device_t::create_window_frame_buffer(window_t* window, void** pixels, int* pitch)
{
    if (open("/dev/fb", MODE_RDWR) < 0) {
        mknod("/dev/fb", 1, 1);
    }

    int fd = open("/dev/fb", MODE_RDWR);
    if (fd < 0) {
        printf("Failed to mknod /dev/fb\n");
    }

    unsigned long width = 0, height = 0, depth = 0;
    uint8* buffer = nullptr;

    ioctl(fd, FB_GET_WIDTH, (uint64) &width);
    ioctl(fd, FB_GET_HEIGHT, (uint64) &height);
    ioctl(fd, FB_GET_DEPTH, (uint64) &depth);
    ioctl(fd, FB_MAP, (uint64) &buffer);

    printf("fb width: %d, height: %d, depth: %d, buffer: %p\n", width, height, depth, buffer);

    *pixels = buffer;
    *pitch = depth * width;

    return 0;
}