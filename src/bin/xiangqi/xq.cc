/*
 *	babyos/bin/xiangqi/xq.cc
 *
 *  Copyright (C) <2020>  <Ruyi Liu>
 *
 *  This program is free software: you can ristribute it and/or modify
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
 *  2020-04-28		created
 */



#include "xq.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "video.h"
#include "assert.h"
#include "cxx.h"
#include "data.h"
#include "string.h"



xiangqi_t::xiangqi_t()
{
	m_first_play = true;
	m_game_start = true;
	m_game_over = false;
	m_turn_to_play = RED;
}

xiangqi_t::~xiangqi_t()
{

}

bool xiangqi_t::load_images()
{
	const char* names[] = { 
        "/bin/board.bmp", "/bin/r_shuai.bmp", "/bin/r_shi.bmp", "/bin/r_xiang.bmp", 
		"/bin/r_ma.bmp", "/bin/r_ju.bmp", "/bin/r_pao.bmp", "/bin/r_bing.bmp", 
        "/bin/b_jiang.bmp", "/bin/b_shi.bmp", "/bin/b_xiang.bmp", "/bin/b_ma.bmp", 
        "/bin/b_ju.bmp", "/bin/b_pao.bmp", "/bin/b_zu.bmp", "/bin/mask.bmp"
    };

    m_qipan = video_t::load_texture_bmp(names[0], m_renderer);
    if (m_qipan == nullptr) {
        printf("load %s failed\n", names[0]);
        return false;
    }

	for (int i = 1; i < 15; i++) {
        m_pieces[i] = video_t::load_texture_bmp(names[i], names[15], m_renderer);
        if (m_pieces[i] == nullptr) {
            printf("load %s failed\n", names[i]);
            return false;
        }
    }

    return true;
}

void xiangqi_t::draw()
{
    draw_qipan();
    draw_qizi();
    m_renderer->present();
}

void xiangqi_t::draw_qipan()
{
    m_renderer->copy_texture(m_qipan, nullptr, nullptr);
}

int xiangqi_t::get_position(int y, int x)
{
	int position = 16 * y + x;
	if (!m_first_play) {
        position = 254 - position;
    }

	return position;
}

void xiangqi_t::draw_qizi()
{
	int position, qizi_no;
	for (int x = QIPAN_LEFT; x <= QIPAN_RIGHT; x++) {
		for (int y = QIPAN_TOP; y <= QIPAN_BOTTOM; y++) {
			position = get_position(y, x);
			if (m_board[position]) {
				qizi_no = m_board[position] - 7;
				if (m_board[position] >= 16) {
                    qizi_no--;
                }

				draw_one_qizi(qizi_no, y - QIPAN_TOP, x - QIPAN_LEFT);
			}
		}
	}
}

void xiangqi_t::draw_one_qizi(int kind, int row, int col)
{
    rect_t rc;
    rc.w = m_pieces[kind]->width();
    rc.h = m_pieces[kind]->height();
    rc.x = 37 + 50*col;
    rc.y = 41 + 50*row;

    m_renderer->copy_texture(m_pieces[kind], nullptr, &rc);
}

void xiangqi_t::loop()
{
    while (true) {
        draw();
    }
}

bool xiangqi_t::init()
{
    m_window = video_t::create_window("Texture", 375, 187, 530, 585, 0);
    surface_t* surface = m_window->get_surface();
    m_renderer = surface->create_renderer(m_window);

    if (!load_images()) {
        return false;
    }

    memcpy(m_board, init_qipan, 256);

    return true;
}


int main()
{
    xiangqi_t* xq = new xiangqi_t();
    if (!xq->init()) {
        printf("init failed\n");
        return -1;
    }

    xq->loop();

    return 0;
}

