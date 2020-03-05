/*
 *	babyos/kernel/timer.h
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
 *  2020-02-18		created
 */



#ifndef _TIMER_H_
#define _TIMER_H_


#include "types.h"
#include "dlist.h"


class timer_t {
public:
    timer_t();
    ~timer_t();

    void init(uint64 expires, uint64 data, void (*func)(uint64));
    bool update();
    dlist_node_t* get_list_node();

public:
    dlist_node_t m_list_node;

private:
	uint64       m_expires;
	uint64       m_data;
	void         (*m_function)(uint64);
};


#endif
