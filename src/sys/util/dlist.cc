/*
 *	babyos/kernel/dlist_t.h
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


#include "dlist.h"


dlist_t::dlist_t()
{
}

dlist_t::~dlist_t()
{
}

void dlist_t::init()
{
    m_head = NULL;
    m_tail = NULL;
    m_length = 0;
}

bool dlist_t::empty()
{
    return m_length == 0;
}

dlist_node_t* dlist_t::head()
{
    return m_head;
}

dlist_node_t* dlist_t::tail()
{
    return m_tail;
}

uint64 dlist_t::length()
{
    return m_length;
}

void dlist_t::add_tail(dlist_node_t* node)
{
    node->m_next = NULL;
    node->m_prev = m_tail;

    if (m_tail == NULL) {
        /* empty list */
        m_head = node;
        m_tail = node;
    }
    else {
        m_tail->m_next = node;
        m_tail = node;
    }

    m_length++;
}

void dlist_t::add_head(dlist_node_t* node)
{
    node->m_prev = NULL;
    node->m_next = m_head;

    if (m_head == NULL) {
        /* empty list */
        m_head = node;
        m_tail = node;
    }
    else {
        m_head->m_prev = node;
        m_head = node;
    }

    m_length++;
}

void dlist_t::insert(dlist_node_t* pos, dlist_node_t* node)
{
    if (pos->m_prev == NULL) {
        add_head(node);
    }
    else {
        node->m_prev = pos->m_prev;
        pos->m_prev->m_next = node;
        pos->m_prev = node;
        node->m_next = pos;
        m_length--;
    }
}

dlist_node_t* dlist_t::remove(dlist_node_t* node)
{
    if (node == NULL) {
        return node;
    }

    if (node->m_next == NULL) {
        /* this is the tail of list */
        this->m_tail = node->m_prev;
    }
    else {
        /* not tail */
        node->m_next->m_prev = node->m_prev;
    }

    if (node->m_prev == NULL) {
        /* this is head of list */
        this->m_head = node->m_next;
    }
    else {
        /* not head */
        node->m_prev->m_next = node->m_next;
    }

    this->m_length--;

    return node;
}

dlist_node_t* dlist_t::remove_head()
{
    dlist_node_t* head = m_head;
    if (head != NULL) {
        head = remove(head);
    }

    return head;
}

dlist_node_t* dlist_t::remove_tail()
{
    dlist_node_t* tail = m_tail;
    if (tail != NULL) {
        tail = remove(tail);
    }

    return tail;
}
