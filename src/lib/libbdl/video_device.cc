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
#include "string.h"
#include <video_device.h>


video_device_t::video_device_t()
{
    m_fd = -1;
    m_width = 0;
    m_height = 0;
    m_depth = 0;
    m_buffer[0] = nullptr;
    m_buffer[1] = nullptr;
}

video_device_t::~video_device_t()
{

}

int video_device_t::create_window(window_t* window)
{
    printf("device, create window\n");
    m_fd = open("/dev/fb", MODE_RDWR);
    if (m_fd < 0) {
        mknod("/dev/fb", 1, 1);
        m_fd = open("/dev/fb", MODE_RDWR);
    }

    if (m_fd < 0) {
        printf("Failed to mknod /dev/fb\n");
    }

    uint8* buffer = nullptr;

    printf("device, create window1\n");
    ioctl(m_fd, FB_GET_WIDTH, (uint64) &m_width);
    ioctl(m_fd, FB_GET_HEIGHT, (uint64) &m_height);
    ioctl(m_fd, FB_GET_DEPTH, (uint64) &m_depth);
    ioctl(m_fd, FB_MAP, (uint64) &buffer);

    printf("fb width: %d, height: %d, depth: %d, buffer: %p\n", m_width, m_height, m_depth, buffer);
    m_buffer_id = 0;
    m_buffer[0] = buffer;
    m_buffer[1] = buffer + m_width * m_height * m_depth;
    memcpy(m_buffer[1], m_buffer[0], m_width*m_height*m_depth);

    return 0;
}

int video_device_t::update_window_frame_buffer(window_t* window, rect_t* rects, int numrects)
{
    m_buffer_id = !m_buffer_id;
    uint8_t* dst = m_buffer[m_buffer_id];
    dst += (m_width*window->y() + window->x()) * m_depth;

    surface_t* surface = window->get_surface();
    uint8_t* src = (uint8_t *) surface->pixels();
    for (int i = 0; i < window->height(); i++) {
        memcpy(dst, src, window->width() * m_depth);
        dst += m_width * m_depth;
        src += window->width() * m_depth;
    }

    ioctl(m_fd, FB_SWAP_BUFFER, 0);
    return 0;
}

int video_device_t::create_window_frame_buffer(window_t* window, void** pixels, int* pitch)
{
    *pitch = window->width() * m_depth;
    *pixels = new char[*pitch * window->height()];
    memset(*pixels, 0, *pitch * window->height());

    return 0;
}