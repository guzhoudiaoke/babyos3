/*
 *	babyos/bin/testmouse.cc
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
 *  2020-04-09		created
 */



#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "video.h"
#include "bdl.h"
#include "assert.h"
#include "cxx.h"
#include "mouse.h"


struct draw_states_t {
    window_t *window;
    renderer_t *renderer;
    texture_t *background;
    texture_t *mouse;
    int mouse_x;
    int mouse_y;
    int mouse_prev_x;
    int mouse_prev_y;
};

draw_states_t* drawstates = nullptr;


void draw(window_t* window, renderer_t* renderer)
{
    rect_t viewport, mouse_rect;
    renderer->get_view_port(&viewport);
    renderer->copy_texture(drawstates->background, nullptr, nullptr);

    mouse_rect.x = drawstates->mouse_x;
    mouse_rect.y = drawstates->mouse_y;
    mouse_rect.h = 16;
    mouse_rect.w = 16;
    renderer->copy_texture(drawstates->mouse, nullptr, &mouse_rect);
    renderer->present();
}

mouse_packet_t packet[1024];
int main()
{
    int fd = open("/dev/mouse", MODE_RDWR);
    if (fd < 0) {
        mknod("/dev/mouse", 2, 1);
        fd = open("/dev/mouse", MODE_RDWR);
    }

    if (fd < 0) {
        return -1;
    }

    drawstates = new draw_states_t();

    drawstates->window = video_t::create_window("Mouse", 0, 0, 1280, 960, 0);
    surface_t* surface = drawstates->window->get_surface();
    drawstates->renderer = surface->create_renderer(drawstates->window);
    drawstates->background = video_t::load_texture_bmp("/bin/sample.bmp", drawstates->renderer);
    drawstates->mouse = video_t::load_texture_bmp("/bin/arrow.bmp", drawstates->renderer);
    drawstates->mouse_x = 0;
    drawstates->mouse_y = 0;

    while (true) {
        int count = read(fd, packet, 1024);
        if (count != 0) {
            int dx = 0, dy = 0;
            for (int i = 0; i < count; i++) {
                dx += packet[i].dx;
                dy += packet[i].dy;
            }

            drawstates->mouse_prev_x = drawstates->mouse_x;
            drawstates->mouse_prev_y = drawstates->mouse_y;
            if (dx != 0 || dy != 0) {
                drawstates->mouse_x += dx;
                drawstates->mouse_y += dy;

                if (drawstates->mouse_x < 0) {
                    drawstates->mouse_x = 0;
                }
                if (drawstates->mouse_y < 0) {
                    drawstates->mouse_y = 0;
                }
                if (drawstates->mouse_x >= 1280) {
                    drawstates->mouse_x = 1280;
                }
                if (drawstates->mouse_y >= 960) {
                    drawstates->mouse_y = 960;
                }
            }

            if (drawstates->mouse_prev_x != drawstates->mouse_x || drawstates->mouse_prev_y != drawstates->mouse_y) {
                draw(drawstates->window, drawstates->renderer);
            }
        }
    }

    return 0;
}
