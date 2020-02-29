/*
 *	babyos/kernel/dlist.h
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
 *  2020-02-29		created
 */


#ifndef _DLIST_H_
#define _DLIST_H_



#include "types.h"



class dlist_node_t {
public:
    void init() {
        m_prev = NULL;
        m_next = NULL;
    }

    dlist_node_t* prev() {
        return m_prev;
    }

    dlist_node_t* next() {
        return m_next;
    }

    dlist_node_t* m_prev;
    dlist_node_t* m_next;
};


class dlist_t {
public:
    dlist_t();
    ~dlist_t();

    void init();
    bool empty();
    dlist_node_t* head();
    dlist_node_t* tail();
    uint64 length();
    void add_tail(dlist_node_t* node);
    void add_head(dlist_node_t* node);
    void insert(dlist_node_t* pos, dlist_node_t* node);
    dlist_node_t* remove(dlist_node_t* node);
    dlist_node_t* remove_head();
    dlist_node_t* remove_tail();

private:
    dlist_node_t* m_head;
    dlist_node_t* m_tail;
    uint64        m_length;
};


#endif
