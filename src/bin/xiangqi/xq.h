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

	
typedef struct hash_node {
    int64_t check;
    int value;
    int depth;
} hash_node_t;				//哈希表结构


class xiangqi_t {
public:
    xiangqi_t();
    ~xiangqi_t();

    bool init();
    void loop();

private:
    bool load_images();
    void draw();
    void draw_board();
    void draw_pieces();
    void draw_one_piece(int kind, int row, int col);
    void draw_mouse();
    void draw_select();

    int  get_position(int x, int y);
    int  get_event();

    void mouse_move();
    void left_button_down();
    void computer_play();

    bool is_self_piece(int pos);
    bool is_opposite_piece(int pos);
    void select_piece(int x, int y);

    bool can_piece_move(move_t& m);
    void move_piece(move_t& m, int& eat);
    void back_move_piece(move m, int eat);
    void add_qizi(int qizi_no, int position);
    void del_qizi(int qizi_no, int position);
    void change_side();


    bool can_js_move(move m);
    bool can_shi_move(move m);
    bool can_xiang_move(move m);
    bool can_ma_move(move m);
    bool can_ju_move(move m);
    bool can_pao_move(move m);
    bool can_bz_move(move m);
    bool at_self_side(int position);
    bool at_opposite_side(int position);
    bool in_same_row(int from, int to);
    bool in_same_column(int from, int to);
    int get_forward(int from);
    bool is_jiangjun();
    bool is_jiangjun_by_jujs(int position);
    bool is_jiangjun_by_ma(int position);
    bool is_jiangjun_by_pao(int position);
    bool is_jiangjun_by_bz(int position);
    void generate_all_moves(move m[], int& num);
    void generate_moves_of_js(int position, move m[], int& num);
    void generate_moves_of_shi(int position, move m[], int& num);
    void generate_moves_of_xiang(int position, move m[], int& num);
    void generate_moves_of_ma(int position, move m[], int& num);
    void generate_moves_of_ju(int position, move m[], int& num);
    void generate_moves_of_pao(int position, move m[], int& num);
    void generate_moves_of_bz(int position, move m[], int& num);
    void add_move(move m[], int& num, int from, int to);
    void generate_moves_of_selected(move m[], int& num);
    bool test_move(int from, int to);
    bool is_siqi(move m[], int num);
    int comput_eat_first(move m[], int num);
    int min_max(int depth);
    int nega_max(int depth);
    int alpha_beta(int alpha, int beta, int depth);
    void sort_by_historytable(move* m, int num);
    int quiescent_search(int alpha, int beta);
    void generate_all_eat_moves(move m[], int& num);
    void generate_eat_moves_of_js(int position, move m[], int& num);
    void generate_eat_moves_of_shi(int position, move m[], int& num);
    void generate_eat_moves_of_xiang(int position, move m[], int& num);
    void generate_eat_moves_of_ma(int position, move m[], int& num);
    void generate_eat_moves_of_ju(int position, move m[], int& num);
    void generate_eat_moves_of_pao(int position, move m[], int& num);
    void generate_eat_moves_of_bz(int position, move m[], int& num);
    int32_t rand32();
    int64_t rand64();
    void init_zobrist();
    void write_hash_table(int value, int depth);
    int read_hash_table(int depth);


private:
    int         m_mouse_fd;
    window_t*   m_window;
    renderer_t* m_renderer;
    texture_t*  m_texture_board;
    texture_t*  m_texture_pieces[15];
    texture_t*  m_texture_mouse;
    texture_t*  m_texture_select;

    char        m_board[256];

    int         m_mouse_x;
    int         m_mouse_y;
    int         m_mouse_prev_x;
    int         m_mouse_prev_y;
	int         m_selected_x;
	int         m_selected_y;

	bool        m_first_play;
	bool        m_game_start;
	bool        m_game_over;
	int         m_turn_to_play;

	move        m_last_move;
	move        m_moves_of_selected[20];

	int         m_n_moves_of_selected;

	int         m_value_of_board;
	int         m_search_depth;
	int         m_best_move_index;
	int         m_search_level;
	int         m_history_table[65536];

	int32_t     m_zobrist_player;				//32位的走棋方键值
	int64_t     m_zobrist_player_check;		//64位的走棋方校验值
	int32_t     m_zobrist_table[7][256];		//32位的棋子在棋盘各个位置键值
	int64_t     m_zobrist_table_check[7][256];//64位的棋子在棋盘各个位置校验值
	int32_t     m_zobrist_key;		//局面键值
	int64_t     m_zobrist_key_check;	//局面校验值
	hash_node_t* m_hash_table;		//哈希表
	int32_t     m_hash_size;			//哈希表的大小
};
