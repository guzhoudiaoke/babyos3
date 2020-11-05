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
#include "mouse.h"
#include "time.h"


xiangqi_t::xiangqi_t()
{
	m_first_play = true;
	m_game_start = true;
	m_game_over = false;
	m_turn_to_play = RED;
    m_search_level = 3;

    m_mouse_x = 0;
    m_mouse_y = 0;
    m_mouse_prev_x = 0;
    m_mouse_prev_y = 0;
    m_selected_x = 0;
    m_selected_y = 0;
}

xiangqi_t::~xiangqi_t()
{

}

bool xiangqi_t::load_images()
{
    m_texture_board = video_t::load_texture_bmp(image_names[0], m_renderer);
    if (m_texture_board == nullptr) {
        printf("load %s failed\n", image_names[0]);
        return false;
    }

	for (int i = 1; i < 15; i++) {
        m_texture_pieces[i] = video_t::load_texture_bmp(image_names[i], image_names[15], m_renderer);
        if (m_texture_pieces[i] == nullptr) {
            printf("load %s failed\n", image_names[i]);
            return false;
        }
    }

    m_texture_mouse = video_t::load_texture_png(image_names[16], m_renderer);
    if (m_texture_mouse == nullptr) {
        printf("load %s failed\n", image_names[16]);
        return false;
    }

    m_texture_select = video_t::load_texture_bmp(image_names[17], m_renderer);
    if (m_texture_select == nullptr) {
        printf("load %s failed\n", image_names[17]);
        return false;
    }

    return true;
}

void xiangqi_t::draw()
{
    draw_board();
    draw_select();
    draw_pieces();
    draw_mouse();
    m_renderer->present();
}

void xiangqi_t::draw_board()
{
    m_renderer->copy_texture(m_texture_board, nullptr, nullptr);
}

int xiangqi_t::get_position(int x, int y)
{
	int position = 16 * y + x;
	if (!m_first_play) {
        position = 254 - position;
    }

	return position;
}

void xiangqi_t::draw_pieces()
{
	int position, no;
	for (int x = BOARD_LEFT; x <= BOARD_RIGHT; x++) {
		for (int y = BOARD_TOP; y <= BOARD_BOTTOM; y++) {
			position = get_position(x, y);
			if (m_board[position]) {
				no = m_board[position] - 7;
				if (m_board[position] >= 16) {
                    no--;
                }

				draw_one_piece(no, y - BOARD_TOP, x - BOARD_LEFT);
			}
		}
	}
}

void xiangqi_t::draw_one_piece(int kind, int row, int col)
{
    rect_t rc;
    rc.w = m_texture_pieces[kind]->width();
    rc.h = m_texture_pieces[kind]->height();
    rc.x = 37 + 50*col;
    rc.y = 41 + 50*row;

    m_renderer->copy_texture(m_texture_pieces[kind], nullptr, &rc);
}

void xiangqi_t::draw_select()
{
    if (m_selected_x == 0) {
        return ;
    }

    rect_t rc;
    rc.w = 50;
    rc.h = 50;
    rc.x = 37 + 50*(m_selected_x-3);
    rc.y = 41 + 50*(m_selected_y-3);

    //m_renderer->copy_texture(m_texture_select, nullptr, &rc);
    m_renderer->set_draw_color(0, 0xff, 0, 0xff);
    m_renderer->fill_rect(&rc);
}

void xiangqi_t::draw_mouse()
{
    rect_t rc;
    rc.w = 24;
    rc.h = 24;
    rc.x = m_mouse_x;
    rc.y = m_mouse_y;

    m_renderer->copy_texture(m_texture_mouse, nullptr, &rc);
}

int xiangqi_t::get_event()
{
    mouse_packet_t packet[1024];
    uint64 size = read(m_mouse_fd, packet, 1024*sizeof(mouse_packet_t));
	uint64 count = size / sizeof(mouse_packet_t);
    int button = 0;
    if (count != 0) {
        int dx = 0, dy = 0;
        for (uint64 i = 0; i < count; i++) {
            dx += packet[i].dx;
            dy += packet[i].dy;
            if (packet[i].button == LEFT) {
                button = packet[i].button;
                break;
            }
        }

        m_mouse_prev_x = m_mouse_x;
        m_mouse_prev_y = m_mouse_y;
        if (dx != 0 || dy != 0) {
            m_mouse_x += dx;
            m_mouse_y += dy;

            if (m_mouse_x < 0) {
                m_mouse_x = 0;
            }
            if (m_mouse_y < 0) {
                m_mouse_y = 0;
            }
            if (m_mouse_x >= 1280-24) {
                m_mouse_x = 1280-24;
            }
            if (m_mouse_y >= 960-24) {
                m_mouse_y = 960-24;
            }
        }
    }

    if (button == LEFT) {
        return 1;
    }

    if (m_mouse_prev_x != m_mouse_x || m_mouse_prev_y != m_mouse_y) {
        return 2;
    }

    return 0;
}

void xiangqi_t::mouse_move()
{

}

bool xiangqi_t::can_piece_move(move_t& m)
{
	switch ( m_board[m.from] )
	{
		case 8:  //红帅
		case 16: //黑将
			return can_js_move( m );
		case 9:  //红仕
		case 17: //黑士
			return can_shi_move( m );
		case 10: //红相
		case 18: //黑相
			return can_xiang_move( m );
		case 11: //红马
		case 19: //黑马
			return can_ma_move( m );
		case 12: //红车
		case 20: //黑车
			return can_ju_move( m );
		case 13: //红炮
		case 21: //黑炮
			return can_pao_move( m );
		case 14: //红兵
		case 22: //黑卒
			return can_bz_move( m );
		default:
			return false;
	}
    return false;
}

void xiangqi_t::add_qizi(int qizi_no, int position)
{
	m_board[position] = qizi_no;
	if ( qizi_no < 16 ) //红子
    {
		m_value_of_board += 
				position_value[qizi_no - 8][position];
	}
	else
	{
		m_value_of_board -=
				position_value[qizi_no - 16][254 - position];
	}
}

//从棋盘拿掉一个棋子
void xiangqi_t::del_qizi(int qizi_no, int position)
{
	m_board[position] = 0;
	
	if ( qizi_no < 16 ) //红子
	{
		m_value_of_board -= 
				position_value[qizi_no - 8][position];
		/*zobrist_key ^= 
				zobrist_table[qizi_no - 8][position];
		zobrist_key_check ^= 
				zobrist_table_check[qizi_no - 8][position];*/
	}
	else
	{
		m_value_of_board +=
				position_value[qizi_no - 16][254 - position];
		/*zobrist_key ^= 
				zobrist_table[qizi_no - 16][position];
		zobrist_key_check ^= 
				zobrist_table_check[qizi_no - 16][position];*/
	}
}


void xiangqi_t::move_piece(move_t& m, int& eat)
{
	eat = m_board[m.to];
	if ( eat ) //要移动的位置原本有棋子
		del_qizi( m_board[m.to], m.to );
	add_qizi( m_board[m.from], m.to );
	del_qizi( m_board[m.from], m.from );
	
	m_search_depth++;
}

void xiangqi_t::back_move_piece( move m, int eat )
{
	add_qizi(m_board[m.to], m.from);
	del_qizi(m_board[m.to], m.to);
	if ( eat ) //有棋子被吃掉
		add_qizi(eat, m.to);
	
	m_search_depth--;
}

void xiangqi_t::change_side()
{
	m_turn_to_play = 1 - m_turn_to_play;
}


void xiangqi_t::left_button_down()
{
    int x = (m_mouse_x - 37) / 50 + 3;
    int y = (m_mouse_y - 41) / 50 + 3;

    if (m_selected_x) {
        move_t m;
        int eat = 0;
        m.from = get_position(m_selected_x, m_selected_y);
        m.to = get_position(x, y);

        if (is_self_piece(m.to)) {
            select_piece(x, y);
        }
        else if (can_piece_move(m)) {
            move_piece(m, eat);
            if (!is_jiangjun()) {
                m_selected_x = 0;
                m_selected_y = 0;

                change_side();
                draw();
                computer_play();
            }
        }
    }
    else {
        int p = get_position(x, y);
        if (is_self_piece(p)) {
            select_piece(x, y);
        }
    }
}

void xiangqi_t::computer_play()
{
	if ( m_game_over ) {
		return;
    }

	move moves_of_computer[128];
	int num = 0;
	generate_all_moves( moves_of_computer, num );
	if ( num == 0 )
	{
		m_game_over = true;
		return;
	}

	int eat = 0;
	m_search_depth = 0;
	memset( m_history_table, 0, 65536 * sizeof(int) ); // 清空历史表
	alpha_beta( -siqi_value, siqi_value, m_search_level );
	m_last_move = moves_of_computer[m_best_move_index];
	move_piece( moves_of_computer[m_best_move_index], eat );
	change_side();
    draw();

	move moves_of_player[128];
	num = 0;
	generate_all_moves( moves_of_player, num );
	if ( num == 0 )
	{
		m_game_over = true;
		return;
	}
}


bool xiangqi_t::is_self_piece(int pos)
{
	return ((8 + m_turn_to_play * 8) & m_board[pos]) != 0;
}

bool xiangqi_t::is_opposite_piece(int pos)
{
	return ((16 - m_turn_to_play * 8) & m_board[pos]) != 0;
}


void xiangqi_t::select_piece(int x, int y)
{
    m_selected_x = x;
    m_selected_y = y;
}

void xiangqi_t::loop()
{
    draw();

    while (true) {
        int e = get_event();
        if (e == 0) {
            continue;
        }
        if (e == 1) {
            left_button_down();
        }
        if (e == 2) {
            mouse_move();
        }

        draw();
    }
}

bool xiangqi_t::init()
{
    m_mouse_fd = open("/dev/mouse", MODE_RDWR);
    if (m_mouse_fd < 0) {
        mknod("/dev/mouse", 2, 1);
        m_mouse_fd = open("/dev/mouse", MODE_RDWR);
    }

    if (m_mouse_fd < 0) {
        return false;
    }

    m_window = video_t::create_window("Texture", 375, 187, 530, 585, 0);
    surface_t* surface = m_window->get_surface();
    m_renderer = surface->create_renderer(m_window);

    if (!load_images()) {
        return false;
    }

    memcpy(m_board, init_board, 256);

    return true;
}


//判断将（帅）能否移动
bool xiangqi_t::can_js_move( move m )
{
	if ( in_jiugong[m.to] )
		for ( int i = 0; i < 4; i++ )
			if ( m.from + js_move[i] == m.to )
				return true;
	return false;
}

//判断仕（士）能否移动
bool xiangqi_t::can_shi_move( move m )
{
	if ( in_jiugong[m.to] )
		for ( int i = 0; i < 4; i++ )
			if ( m.from + shi_move[i] == m.to )
				return true;
	return false;
}

//判断相（象）能否移动
bool xiangqi_t::can_xiang_move( move m )
{
	if ( in_qipan[m.to] && at_self_side( m.to ) )
		for ( int i = 0; i < 4; i++ )
			if ( m.from + xiang_move[i] == m.to && 
					!m_board[m.from + xiangyan[i]] )
				return true;
	return false;
}

//判断马能否移动
bool xiangqi_t::can_ma_move( move m )
{
	if ( in_qipan[m.to] )
		for ( int i = 0; i < 8; i++ )
			if ( m.from + ma_move[i] == m.to && 
					!m_board[m.from + matui[i]] )
				return true;
	return false;
}

//判断车能否移动
bool xiangqi_t::can_ju_move( move m )
{
	if ( in_qipan[m.to] == 0 )
		return false;
	int delta;
	if ( in_same_row( m.from, m.to ) ) // 同行
		delta = ( m.from < m.to ? 1 : -1 ); // 偏移量为1或-1
	else if ( in_same_column( m.from, m.to ) ) // 同列
		delta = ( m.from < m.to ? 16 : -16 ); // 偏移量为16或-16
	else // 不同行也不同列，必然不可移动
		return false;
	
	int position = m.from + delta;
	while ( position != m.to ) // 按上面得到的偏移量计算
	{
		if ( m_board[position] != 0 ) // 起点终点之间发现棋子
			return false;
		position += delta;
	}
	return true;
}

//判断炮能否移动
bool xiangqi_t::can_pao_move( move m )
{
	if ( in_qipan[m.to] == 0 )
			return false;
	int delta;
	if ( in_same_row( m.from, m.to ) )
		delta = ( m.from < m.to ? 1 : -1 );
	else if ( in_same_column( m.from, m.to ) )
		delta = ( m.from < m.to ? 16 : -16 );
	else //不在同一行，也不在同一列
		return false;
	
	int num = 0; //记录起点和终点间棋子的个数
	int position = m.from + delta;
	while ( position != m.to )
	{
		if ( m_board[position] != 0 )
			num++;
		position += delta;
	}
	if ( (num == 1 && m_board[m.to]) //吃子走法
			|| (num == 0 && m_board[m.to] == 0) ) //不吃子走法
		return true;
	return false;
}

//判断兵（卒）能否移动
bool xiangqi_t::can_bz_move( move m )
{
	if ( in_qipan[m.to] == 0 )
			return false;
	if ( at_opposite_side( m.to ) //已过河
			&& (m.to + 1 == m.from || m.to - 1 == m.from) )
		return true;
	if ( get_forward( m.from ) == m.to )
		return true;
	return false;
}

//在自己半边（未过河）
bool xiangqi_t::at_self_side( int position )
{
	return m_turn_to_play == RED ? 
			position > 128 : position < 128;
}

//在对方半边（已过河）
bool xiangqi_t::at_opposite_side( int position )
{
	return m_turn_to_play == RED ? position < 128 : position > 128;
}

//判断是否同行
bool xiangqi_t::in_same_row( int from, int to )
{
	return from / 16 == to / 16;
}

//判断是否同列
bool xiangqi_t::in_same_column( int from, int to )
{
	return from % 16 == to % 16;
}

//返回向前进一格的位置
int xiangqi_t::get_forward( int from )
{
	return m_turn_to_play == RED ? 
			from - 16 : from + 16;
}

//被将军
bool xiangqi_t::is_jiangjun()
{
	int selfSide = 8 + m_turn_to_play * 8;
	int jsPosition; //找到自己老帅（将）的位置
	
	for ( jsPosition = 0; jsPosition < 256; jsPosition++ )
		if ( m_board[jsPosition] == (selfSide + PIECE_JS) )
			break;
	
	if ( is_jiangjun_by_jujs(jsPosition) 
			|| is_jiangjun_by_ma(jsPosition)
			|| is_jiangjun_by_pao(jsPosition)
			|| is_jiangjun_by_bz(jsPosition) ) 
		return true;
	return false;
}

//被车将军或将帅白脸
bool xiangqi_t::is_jiangjun_by_jujs( int position )
{
	int oppositeSide = 16 - m_turn_to_play * 8;
	
	for ( int i = 0; i < 4; i++ )
	{
		int delta = js_move[i]; //车移动一格时步长与将帅相同
		int to = position + delta;
		while ( in_qipan[to] )
		{
			if ( m_board[to] == 0 )
				to += delta;
			else if ( m_board[to] == oppositeSide + PIECE_JU
					|| m_board[to] == oppositeSide + PIECE_JS )
				return true;
			else
				break;
		}
	}
	return false;
}

//被马将军
bool xiangqi_t::is_jiangjun_by_ma( int position )
{
	int oppositeSide = 16 - m_turn_to_play * 8;
	
	for ( int i = 0; i < 8; i++ )
	{
		int to = position + ma_move[i];
		if ( m_board[to] == oppositeSide + PIECE_MA 
				&& m_board[js_matui[i] + position] == 0 )
			return true;
	}
	return false;
}

//被炮将军
bool xiangqi_t::is_jiangjun_by_pao( int position )
{
	int oppositeSide = 16 - m_turn_to_play * 8;
	for ( int i = 0; i < 4; i++ )
	{
		int delta = js_move[i]; //车移动一格时步长与将帅相同
		int to = position + delta;
		bool flag = false; //找到炮架子标志
		while ( in_qipan[to] )
		{
			if ( m_board[to] == 0 ) //无棋子
				to += delta;
			else //有棋子
			{
				if ( !flag ) //若尚无炮架子，则设为已找到
				{
					flag = true;
					to += delta;
				}
				else if ( m_board[to] == oppositeSide + PIECE_PAO )
					return true; //找到炮架子后第一个子为对方炮
				else
					break;
			}
		}		
	}
	return false;
}

//被兵卒将军
bool xiangqi_t::is_jiangjun_by_bz( int position )
{
	int oppositeSide = 16 - m_turn_to_play * 8;
	if ( m_board[get_forward( position )] == oppositeSide + PIECE_BZ
			|| m_board[position+1] == oppositeSide + PIECE_BZ
			||m_board[position-1] == oppositeSide + PIECE_BZ )
		return true;
	return false;
}

//生成所有走法
void xiangqi_t::generate_all_moves( move m[], int& num )
{
	int x, y, position, selfSide = 8 + m_turn_to_play * 8;
	for(x = BOARD_LEFT; x <= BOARD_RIGHT; x++)
		for(y = BOARD_TOP; y <= BOARD_BOTTOM; y++)
		{
			position = 16 * y + x;
			int qizi_no = m_board[position] - selfSide;
			if ( is_self_piece(position) )
				switch ( qizi_no )
				{
					case PIECE_JS:
						generate_moves_of_js( position, m, num );
						break;
					case PIECE_SHI:
						generate_moves_of_shi( position, m, num );
						break;
					case PIECE_XIANG:
						generate_moves_of_xiang( position, m, num );
						break;
					case PIECE_MA:
						generate_moves_of_ma( position, m, num );
						break;
					case PIECE_JU:
						generate_moves_of_ju( position, m, num );
						break;
					case PIECE_PAO:
						generate_moves_of_pao( position, m, num );
						break;
					case PIECE_BZ:
						generate_moves_of_bz( position, m, num );
						break;
				}
		}
}

//生成将/帅的所有走法 
void xiangqi_t::generate_moves_of_js( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 4; i++ )
	{
		int to = position + js_move[i];
		if ( in_jiugong[to] == 0 )
			continue;
		if ( !is_self_piece(to) ) //非己方棋子
			if ( test_move(position, to) )
				add_move( m, num, position, to );
	}
}

//生成仕/士的所有走法
void xiangqi_t::generate_moves_of_shi( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 4; i++ )
	{
		int to = position + shi_move[i];
		if ( in_jiugong[to] == 0 )
			continue;
		if ( !is_self_piece(to) ) //非己方棋子
			if ( test_move(position, to) )
				add_move( m, num, position, to );
	}
}

//生成相/象的所有走法
void xiangqi_t::generate_moves_of_xiang( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 4; i++ )
	{
		int to = position + xiang_move[i];
		int eye = position + xiangyan[i];
		if ( !in_qipan[to] || at_opposite_side(to)
				|| m_board[eye] )
			continue;
		if ( !is_self_piece(to) ) //非己方棋子
			if ( test_move(position, to) )
				add_move( m, num, position, to );
	}
}

//生成马的所有走法
void xiangqi_t::generate_moves_of_ma( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 8; i++ )
	{
		int to = position + ma_move[i];
		int leg = position + matui[i];
		if ( !in_qipan[to] || m_board[leg] )
			continue;
		if ( !is_self_piece(to) ) //非己方棋子
			if ( test_move(position, to) )
				add_move( m, num, position, to );
	}
}

//生成车的所有走法
void xiangqi_t::generate_moves_of_ju( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 4; i++ )
	{
		int delta = js_move[i];
		int to = position + delta;
		while ( in_qipan[to] )
		{
			if ( m_board[to] == 0 ) //无棋子，可行
			{
				if ( test_move(position, to) )
					add_move( m, num, position, to );
				to += delta;
			}
			else if ( is_opposite_piece( to ) ) //对方的棋子
			{
				if ( test_move(position, to) )
					add_move( m, num, position, to );
				break; //改变方向
			}
			else
				break;
		}
	}
}

//生成炮的所有走法
void xiangqi_t::generate_moves_of_pao( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 4; i++ )
	{
		int delta = js_move[i];
		int to = position + delta;
		int flag = false; //是否找到炮架子标志
		while ( in_qipan[to] )
		{
			if ( m_board[to] == 0 ) //无棋子
			{
				if ( !flag ) //且尚无炮架子，可行
					if ( test_move(position, to) )
						add_move( m, num, position, to );
				to += delta;
			}
			else {
				if ( !flag ) //尚未找到炮架子
				{
					flag = true;
					to += delta;
				}
				else //已有炮架子
				{
					if ( is_opposite_piece(to) ) //对方棋子
						if ( test_move(position, to) )
							add_move( m, num, position, to );
					break;
				}
			}
		}
	}
}


//生成兵/卒的所有走法
void xiangqi_t::generate_moves_of_bz( int position, 
		move m[], int& num )
{
	int to = get_forward( position ); 
	if ( !is_self_piece(to) && in_qipan[to] )
		if ( test_move(position, to) )
			add_move( m, num, position, to );
	if ( at_opposite_side( position ) )
	{
		to = position + 1;
		if ( !is_self_piece(to) && in_qipan[to] )
			if ( test_move(position, to) )
				add_move( m, num, position, to );
		to = position - 1;
		if ( !is_self_piece(to) && in_qipan[to] )
			if ( test_move(position, to) )
				add_move( m, num, position, to );
	}
}

//添加走法到走法数组
void xiangqi_t::add_move( move m[], int& num, 
		int from, int to )				
{
	m[num].from = from;
	m[num].to = to;
	num++;
}

//生成选中棋子的所有走法（测试用）
void xiangqi_t::generate_moves_of_selected( move m[], int& num )
{
	int selfSide = 8 + m_turn_to_play * 8;
	int position = get_position( m_selected_x, m_selected_y );
	int qizi_no = m_board[position] - selfSide;
	
	switch ( qizi_no )
	{
		case PIECE_JS:
			generate_moves_of_js( position, m, num );
			break;
		case PIECE_SHI:
			generate_moves_of_shi( position, m, num );
			break;
		case PIECE_XIANG:
			generate_moves_of_xiang( position, m, num );
			break;
		case PIECE_MA:
			generate_moves_of_ma( position, m, num );
			break;
		case PIECE_JU:
			generate_moves_of_ju( position, m, num );
			break;
		case PIECE_PAO:
			generate_moves_of_pao( position, m, num );
			break;
		case PIECE_BZ:
			generate_moves_of_bz( position, m, num );
			break;
	}
}

//测试走法是否可行（移动后是否被将军）
bool xiangqi_t::test_move( int from, int to )
{
	int eat = 0;
	move tryMove;
	tryMove.from = from;
	tryMove.to = to;
	move_piece( tryMove, eat );
	bool flag = false;
	if ( !is_jiangjun() )
		flag = true;
	back_move_piece( tryMove, eat );
	return flag;
}

//检查是否是死棋
bool xiangqi_t::is_siqi( move m[], int num )
{
	if ( num == 0 )
		return true;
	int eat = 0;
	for ( int i = 0; i < num; i++ )
	{
		move_piece( m[i], eat );
		if ( !is_jiangjun() ) //有走法不被将军
		{
			back_move_piece( m[i], eat );
			return false;
		}
		back_move_piece( m[i], eat );
	}
	return true;
}

//计算机吃子优先策略
int xiangqi_t::comput_eat_first( move m[], int num )
{
	int eat = 0;
	for ( int i = 0; i < num; i++ )
	{
		move_piece( m[i], eat );
		if ( eat )
		{
			back_move_piece( m[i], eat );
			return i;
		}
		back_move_piece( m[i], eat );
	}
	return 0;
}

//最小最大搜索
int xiangqi_t::min_max( int depth )
{
	int bestValue, value, num = 0;
	if ( depth == 0 )
		return m_value_of_board;
	bestValue = ( m_turn_to_play == RED ? 
			-siqi_value : siqi_value );
	move m[128];
	generate_all_moves( m, num );
	int index = 0;
	for ( int i = 0; i < num; i++ ) 
	{
		int eat = 0;
		move_piece( m[i], eat );
		change_side();
		value = min_max( depth - 1 );
		back_move_piece( m[i], eat );
		change_side();
		if ( m_turn_to_play == RED ) 
		{
			if ( value > bestValue ) 
			{
				bestValue = value;
				index = i;
			}
		}
		else {
			if ( value < bestValue ) 
			{
				bestValue = value;
				index = i;
			}
		}
	}
	if ( m_search_depth == 0 )
		m_best_move_index = index;
	return bestValue;
}

//负极大值搜索
int xiangqi_t::nega_max( int depth )
{
	int bestValue, value, num = 0, index = 0;
	if ( depth == 0 )
		return m_turn_to_play == RED ? m_value_of_board : -m_value_of_board;
	bestValue = -siqi_value;
	move m[128];
	generate_all_moves( m, num );
	
	for ( int i = 0; i < num; i++ ) 
	{
		int eat = 0;
		
		move_piece( m[i], eat );
		change_side();
		
		value = -nega_max( depth - 1 );
		
		back_move_piece( m[i], eat );
		change_side();

		if ( value > bestValue ) 
		{
			bestValue = value;
			index = i;
		}
	}
	if ( m_search_depth == 0 )
		m_best_move_index = index;
	return bestValue;
}

//超出边界(Fail-Soft)的Alpha-Beta搜索
int xiangqi_t::alpha_beta( int alpha, int beta, int depth )
{
	int bestValue, value, num = 0, index = 0;
	if ( depth == 0 )
		return quiescent_search( alpha, beta ); 
	bestValue = -siqi_value;
	/*value = read_m_hash_table( depth );
	if ( value != not_value )
		return value;*/
		
	move m[128];
	generate_all_moves( m, num );
	sort_by_historytable( m, num );
	for ( int i = 0; i < num; i++ )
	{
		int eat = 0;
		move_piece( m[i], eat );
		change_side();
		value = -alpha_beta( -beta, -alpha, depth - 1);
		back_move_piece( m[i], eat );
		change_side();
		if ( value > bestValue ) //找到最佳值
		{
			index = i;
			bestValue = value;
			/*write_m_hash_table( bestValue, depth );*/
			if ( value >= beta ) //beta截断
				break;
			if ( value > alpha )
				alpha = value; //缩小alpha-beta边界
		}
	}
	if ( num )
		m_history_table[m[index].from * 256 + m[index].to] += depth * depth;
	if ( m_search_depth == 0 )
		m_best_move_index = index;
	return bestValue;
}

//按历史表排序
void xiangqi_t::sort_by_historytable( move* m, int num )
{
	move temp;
	for ( int i = 0; i < num; i++ )
		for ( int j = i + 1; j < num; j++ )
			if ( m_history_table[m[i].from * 256 + m[i].to] < 
					m_history_table[m[j].from * 256 + m[j].to] )
			{
				temp = m[i];
				m[i] = m[j];
				m[j] = temp;
			}
}

//静态搜索
int xiangqi_t::quiescent_search(int alpha, int beta)
{
	int bestValue = -siqi_value;
	int value = (m_turn_to_play == RED ? m_value_of_board : -m_value_of_board);
	if ( value > bestValue )
	{
		bestValue = value;
		if ( value >= beta )
			return value;
		if ( value > alpha )
			alpha = value;
	}
	if ( m_search_depth > 11 )
		return bestValue;
	int i, num = 0;
	move m[64];
	generate_all_eat_moves( m, num );
	for ( i = 0; i < num; i++ ) 
	{
		int eat = 0;
		move_piece( m[i], eat );
		change_side();
		value = -quiescent_search( -beta, -alpha );
		back_move_piece( m[i], eat );
		change_side();
		if ( value > bestValue ) //找到最佳值
		{
			bestValue = value; 
			if ( value >= beta ) //beta截断
				return value;   
			if ( value > alpha )
				alpha = value; //缩小alpha-beta边界
		}
	}
	/*if ( bestValue == -siqi_value )*/
		/*zgxq_sound[10].play();*/
	return bestValue;
}

//生成所有吃子走法
void xiangqi_t::generate_all_eat_moves( move m[], int& num )
{
	int x, y, position, selfSide = 8 + m_turn_to_play * 8;
	for(x = BOARD_LEFT; x <= BOARD_RIGHT; x++)
		for(y = BOARD_TOP; y <= BOARD_BOTTOM; y++)
		{
			position = 16 * y + x;
			int qizi_no = m_board[position] - selfSide;
			if ( is_self_piece(position) )
				switch ( qizi_no )
				{
					case PIECE_JS:
						generate_eat_moves_of_js( position, m, num );
						break;
					case PIECE_SHI:
						generate_eat_moves_of_shi( position, m, num );
						break;
					case PIECE_XIANG:
						generate_eat_moves_of_xiang( position, m, num );
						break;
					case PIECE_MA:
						generate_eat_moves_of_ma( position, m, num );
						break;
					case PIECE_JU:
						generate_eat_moves_of_ju( position, m, num );
						break;
					case PIECE_PAO:
						generate_eat_moves_of_pao( position, m, num );
						break;
					case PIECE_BZ:
						generate_eat_moves_of_bz( position, m, num );
						break;
				}
		}
}

//生成将/帅的所有吃子走法 
void xiangqi_t::generate_eat_moves_of_js( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 4; i++ )
	{
		int to = position + js_move[i];
		if ( in_jiugong[to] == 0 )
			continue;
		if ( is_opposite_piece(to) ) //对方棋子
			if ( test_move(position, to) )
				add_move( m, num, position, to );
	}
}

//生成仕/士的所有吃子走法
void xiangqi_t::generate_eat_moves_of_shi( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 4; i++ )
	{
		int to = position + shi_move[i];
		if ( in_jiugong[to] == 0 )
			continue;
		if ( is_opposite_piece(to) ) //对方棋子
			if ( test_move(position, to) )
				add_move( m, num, position, to );
	}
}

//生成相/象的所有吃子走法
void xiangqi_t::generate_eat_moves_of_xiang( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 4; i++ )
	{
		int to = position + xiang_move[i];
		int eye = position + xiangyan[i];
		if ( !in_qipan[to] || at_opposite_side(to)
				|| m_board[eye] )
			continue;
		if ( is_opposite_piece(to) ) //对方棋子
			if ( test_move(position, to) )
				add_move( m, num, position, to );
	}
}

//生成马的所有吃子走法
void xiangqi_t::generate_eat_moves_of_ma( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 8; i++ )
	{
		int to = position + ma_move[i];
		int leg = position + matui[i];
		if ( !in_qipan[to] || m_board[leg] )
			continue;
		if ( is_opposite_piece(to) ) //对方棋子
			if ( test_move(position, to) )
				add_move( m, num, position, to );
	}
}

//生成车的所有吃子走法
void xiangqi_t::generate_eat_moves_of_ju( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 4; i++ )
	{
		int delta = js_move[i];
		int to = position + delta;
		while ( in_qipan[to] )
		{
			if ( m_board[to] == 0 ) //无棋子
				to += delta;
			else if ( is_opposite_piece(to) ) //对方棋子
			{
				if ( test_move(position, to) )
					add_move( m, num, position, to );
				break; //改变方向
			}
			else
				break;
		}
	}
}

//生成炮的所有吃子走法
void xiangqi_t::generate_eat_moves_of_pao( int position, 
		move m[], int& num )
{
	for ( int i = 0; i < 4; i++ )
	{
		int delta = js_move[i];
		int to = position + delta;
		int flag = false; //是否找到炮架子标志
		while ( in_qipan[to] )
		{
			if ( m_board[to] == 0 ) //无棋子
				to += delta;
			else 
			{
				if ( !flag ) //尚未找到炮架子
				{
					flag = true;
					to += delta;
				}
				else //已有炮架子
				{
					if ( is_opposite_piece(to) ) //对方棋子
						if ( test_move(position, to) )
							add_move( m, num, position, to );
					break;
				}
			}
		}
	}
}

//生成兵/卒的所有吃子走法
void xiangqi_t::generate_eat_moves_of_bz( int position, 
		move m[], int& num )
{
	int to = get_forward( position ); 
	if ( is_opposite_piece(to) && in_qipan[to] )
		if ( test_move(position, to) )
			add_move( m, num, position, to );
	if ( at_opposite_side( position ) )
	{
		to = position + 1;
		if ( is_opposite_piece(to) && in_qipan[to] )
			if ( test_move(position, to) )
				add_move( m, num, position, to );
		to = position - 1;
		if ( is_opposite_piece(to) && in_qipan[to] )
			if ( test_move(position, to) )
				add_move( m, num, position, to );
	}
}

//产生一个32位随机数
int32_t xiangqi_t::rand32()
{
	return rand() ^ ( (int32_t)rand() << 15 )  
				  ^ ( (int32_t)rand() << 30 );
}

//产生一个64位随机数
int64_t xiangqi_t::rand64()
{
	return rand() ^ ( (int64_t)rand() << 15 )  
				  ^ ( (int64_t)rand() << 30 ) 
				  ^ ( (int64_t)rand() << 45 ) 
				  ^ ( (int64_t)rand() << 60 );
}

//初始化Zobrist键值和校验值
void xiangqi_t::init_zobrist()
{
	srand( time(NULL) );
	
	m_zobrist_player = rand32();
	
	for ( int i = 0; i < 7; i++ )
		for ( int j = 0; j < 256; j++ )
			m_zobrist_table[i][j] = rand32();
	
	m_zobrist_player_check = rand64();
	
	for ( int i = 0; i < 7; i++ )
		for ( int j = 0; j < 256; j++ )
			m_zobrist_table_check[i][j] = rand64();
	
	m_zobrist_key = 0;
	m_zobrist_key_check = 0;
}

//写哈希表
void xiangqi_t::write_hash_table( int value, int depth )
{
	int address = m_zobrist_key & (m_hash_size-1);
	m_hash_table[address].value = value;
	m_hash_table[address].check = m_zobrist_key_check;
	m_hash_table[address].depth = depth;
}

//读哈希表
int xiangqi_t::read_hash_table( int depth )
{
	int address = m_zobrist_key & (m_hash_size-1);
	if ( m_hash_table[address].check == m_zobrist_key_check )
		if ( m_hash_table[address].depth > depth )
			return m_hash_table[address].value;
	return not_value;
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
