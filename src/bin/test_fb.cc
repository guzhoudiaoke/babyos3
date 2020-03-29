/*
 *	babyos/bin/test_fb.cc
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
 *  2020-03-29		created
 */



#include "unistd.h"
#include "fb_ioctl.h"
#include "stdio.h"

void test_draw(uint8* buffer, uint32 width, uint32 height, uint32 depth)
{
    int x = 100, y = 100;
    uint32* p = (uint32 *) buffer;
    for (int i = y; i < y + 100; i++) {
        for (int j = x; j < x + 100; j++) {
            *(p + (width*i + j)) = 0x0000ff0000;
        }
    }
}

int main(int argc, char** argv)
{
    if (open("/dev/framebuffer", MODE_RDWR) < 0) {
        mknod("/dev/framebuffer", 1, 1);
    }

    int fd = open("/dev/framebuffer", MODE_RDWR);
    if (fd < 0) {
        printf("Failed to mknod /dev/framebuffer\n");
    }

    unsigned long width = 0, height = 0, depth = 0;
    uint8* buffer = nullptr;
    printf("&width: %p, &height: %p, &depth: %p, &buffer: %p\n", &width, &height, &depth, &buffer);

    ioctl(fd, FB_GET_WIDTH, (uint64) &width);
    ioctl(fd, FB_GET_HEIGHT, (uint64) &height);
    ioctl(fd, FB_GET_DEPTH, (uint64) &depth);
    ioctl(fd, FB_MAP, (uint64) &buffer);

    printf("fb width: %d, height: %d, depth: %d, buffer: %p\n", width, height, depth, buffer);
    test_draw(buffer, width, height, depth);

    return 0;
}