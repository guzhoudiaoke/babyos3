/*
 *	babyos/bin/testdraw.cc
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
 *  2020-04-05		created
 */



#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "video.h"
#include "bdl.h"
#include "assert.h"


static int num_objects = 100;
static int current_color = 255;
static int cycle_direction = 1;


void draw_rects(renderer_t* renderer)
{
    rect_t rect, viewport;
    renderer->get_view_port(&viewport);

    for (int i = 0; i < num_objects / 4; i++) {
        current_color += cycle_direction;
        if (current_color < 0) {
            current_color = 0;
            cycle_direction = -cycle_direction;
        }
        if (current_color > 255) {
            current_color = 255;
            cycle_direction = -cycle_direction;
        }

        renderer->set_draw_color(255, (uint8_t) current_color, (uint8_t) current_color, 255);

        rect.w = rand() % (viewport.h / 2);
        rect.h = rand() % (viewport.h / 2);
        rect.x = (rand() % (viewport.w * 2) - viewport.w) - (rect.w / 2);
        rect.y = (rand() % (viewport.h * 2) - viewport.h) - (rect.h / 2);

        renderer->fill_rect(&rect);
    }
}

void draw_points(renderer_t* renderer)
{
    rect_t viewport;
    renderer->get_view_port(&viewport);

    for (int y = 0; y < 480; y++) {
        for (int i = 0; i < num_objects; i++) {
            renderer->set_draw_color(rand() % 255, rand() % 255, rand() % 255, 255);
            renderer->draw_point(rand()%viewport.w, y);
        }
    }
}


void draw_points2(renderer_t* renderer)
{
    rect_t viewport;
    renderer->get_view_port(&viewport);

    for (int i = 0; i < 1280; i++) {
        for (int j = 0; j < 960; j++) {
            renderer->set_draw_color(rand() % 255, rand() % 255, rand() % 255, 255);
            renderer->draw_point(i, j);
        }
    }
}

void loop(window_t* window, renderer_t* renderer)
{
    //draw_rects(renderer);
    draw_points2(renderer);
    window->update_surface();
}

int main(int argc, char** argv)
{
    window_t* window = video_t::create_window("Board", 320, 240, 640, 480, 0);
    surface_t* surface = window->get_surface();
    renderer_t* renderer = surface->create_software_renderer();

    renderer->set_draw_color(0xff, 0xff, 0xff, 0xff);
    renderer->clear();

    srand(0x1234);

    while (true) {
        loop(window, renderer);
        sleep(1);
    }

    return 0;
}
