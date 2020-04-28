/*
 *	babyos/bin/xiangqi/xq.h
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
 *  2020-04-28		created
 */


#include "bdl.h"


typedef struct move {
    int from;
    int to;
} move_t;


class xiangqi_t {
public:
    xiangqi_t();
    ~xiangqi_t();

    bool init();
    void loop();

private:
    bool load_images();
    void draw();
    void draw_qipan();
    void draw_qizi();
    void draw_one_qizi(int kind, int row, int col);
    int  get_position(int x, int y);

private:
    window_t*   m_window;
    renderer_t* m_renderer;
    texture_t*  m_qipan;
    texture_t*  m_pieces[15];

    char        m_board[256];
	int         m_selected_x;
	int         m_selected_y;

	bool        m_first_play;
	bool        m_game_start;
	bool        m_game_over;
	int         m_turn_to_play;

	move        m_last_move;
	move        m_moves_of_selected[20];

	int         m_n_moves_of_selected;

	int         m_value_of_qipan;
	int         m_search_depth;
	int         m_best_move_index;
	int         m_search_level;
	int         m_history_table[65536];
};
