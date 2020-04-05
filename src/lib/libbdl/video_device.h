/*
 *	babyos/lib/libbdl/video_device.h
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

#ifndef _VIDEO_DEVICE_H_
#define _VIDEO_DEVICE_H_


#include "stdint.h"
#include <window.h>
#include <surface.h>


class video_device_t {
public:
    video_device_t();
    ~video_device_t();

    int create_window(window_t* window);

    int create_window_frame_buffer(window_t* window, void** pixels, int* pitch);
    int update_window_frame_buffer(window_t* window, rect_t* rects, int numrects);

private:
    int      m_fd;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_depth;

    uint32_t m_buffer_id;
    uint8_t* m_buffer[2];
};


#endif