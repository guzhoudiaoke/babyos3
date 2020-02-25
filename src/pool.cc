/*
 *	babyos/kernel/pool.cc
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
 *  2020-02-17		created
 */



#include "babyos.h"
#include "buddy.h"
#include "pool.h"


void object_pool_t::init(uint32 obj_size)
{
    if (obj_size == 0) {
        os()->panic("invalid pool size");
    }
	m_obj_size = obj_size;
	m_available = 0;
	m_free_list = NULL;
    m_lock.init();
}

void object_pool_t::free_object_nolock(void* obj)
{
	object_pool_obj_t* o = (object_pool_obj_t*) obj;
	o->m_next = NULL;
	if (m_free_list == NULL) {
		m_free_list = o;
	}
	else {
		o->m_next = m_free_list;
		m_free_list = o;
	}
	m_available++;
}

void object_pool_t::free_object(void* obj)
{
    //os()->uart()->kprintf("free object\n");

    uint64 flags;
    m_lock.lock_irqsave(flags);
    free_object_nolock(obj);
    m_lock.unlock_irqrestore(flags);
}

void* object_pool_t::alloc_from_pool()
{
    //os()->uart()->kprintf("alloc from pool\n");

    uint64 flags;
    m_lock.lock_irqsave(flags);
	if (m_free_list == NULL) {
        os()->uart()->kprintf("alloc from pool alloc\n");
		uint8* mem = (uint8 *) PA2VA(os()->buddy()->alloc_pages(0));
		uint8* end = mem + PAGE_SIZE;
		while (mem + m_obj_size <= end) {
			free_object_nolock(mem);
			mem += m_obj_size;
		}
	}

	void* obj = m_free_list;
	m_free_list = m_free_list->m_next;

	m_available--;
    m_lock.unlock_irqrestore(flags);

	return obj;
}

uint32 object_pool_t::get_available()
{
	return m_available;
}

