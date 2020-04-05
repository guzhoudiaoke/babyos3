/*
 *	babyos/bin/testbdl.cc
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
#include "stdio.h"
#include "video.h"
#include "bdl.h"

int t = 0;

void draw_board(renderer_t* renderer)
{
    rect_t rect, darea;
    renderer->get_view_port(&darea);

    renderer->set_draw_color(0xff, 0xff, 0xff, 0xff);
    renderer->fill_rect(&darea);

    for (int row = 0; row < 8; row++) {
        int col = row % 2;
        int x = col;
        for (; col < 4+(row%2); col++) {
            renderer->set_draw_color(0+t*10, 0+t*10, 0+t*10, 0xff);
            rect.w = darea.w / 8;
            rect.h = darea.h / 8;
            rect.x = x * rect.w;
            rect.y = row * rect.h;
            x += 2;
            renderer->fill_rect(&rect);
        }
    }

    t++;
}

void loop(window_t* window, renderer_t* renderer)
{
    draw_board(renderer);
    window->update_surface();
}


int main(int argc, char** argv)
{
    printf("begin\n");

    window_t* window = video_t::create_window("Board", 0, 0, 1280, 960, 0);
    printf("create window: %p\n", window);

    surface_t* surface = window->get_surface();
    printf("window get surface: %p\n", surface);

    renderer_t* renderer = surface->create_software_renderer();
    printf("surface create software renderer: %p\n", renderer);

    renderer->set_draw_color(0xff, 0xff, 0xff, 0xff);
    printf("renderer set_draw_color\n");

    renderer->clear();
    printf("renderer clear\n");

    while (true) {
        loop(window, renderer);
        sleep(1);
    }

    return 0;
}