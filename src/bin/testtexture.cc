/*
 *	babyos/bin/testtexture.cc
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


struct draw_states_t {
    window_t *window;
    renderer_t *renderer;
    texture_t *background;
    texture_t *sprite;
    rect_t sprite_rect;
    int scale_direction;
};

draw_states_t* drawstates = nullptr;


void loop(window_t* window, renderer_t* renderer)
{
    rect_t viewport;
    renderer->get_view_port(&viewport);
    renderer->copy_texture(drawstates->background, nullptr, nullptr);

    drawstates->sprite_rect.w += drawstates->scale_direction;
    drawstates->sprite_rect.h += drawstates->scale_direction;
    if (drawstates->scale_direction > 0) {
        if (drawstates->sprite_rect.w+10 >= viewport.w || drawstates->sprite_rect.h+10 >= viewport.h) {
            drawstates->scale_direction = -10;
        }
    }
    else {
        if (drawstates->sprite_rect.w <= 10 || drawstates->sprite_rect.h <= 10) {
            drawstates->scale_direction = 10;
        }
    }

    drawstates->sprite_rect.x = (viewport.w - drawstates->sprite_rect.w) / 2;
    drawstates->sprite_rect.y = (viewport.h - drawstates->sprite_rect.h) / 2;
    renderer->copy_texture(drawstates->sprite, nullptr, &drawstates->sprite_rect);
    renderer->present();
}

int main()
{
    drawstates = new draw_states_t();

    drawstates->window = video_t::create_window("Texture", 320, 240, 640, 480, 0);
    surface_t* surface = drawstates->window->get_surface();
    drawstates->renderer = surface->create_renderer(drawstates->window);
    drawstates->background = video_t::load_texture("/bin/sample.bmp", drawstates->renderer);
    drawstates->sprite = video_t::load_texture("/bin/sample.bmp", drawstates->renderer);
    drawstates->scale_direction = 10;

    drawstates->sprite_rect.w = drawstates->sprite->width();
    drawstates->sprite_rect.h = drawstates->sprite->height();

    while (true) {
        loop(drawstates->window, drawstates->renderer);
        sleep(1);
    }

    return 0;
}