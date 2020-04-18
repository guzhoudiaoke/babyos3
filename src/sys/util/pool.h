/*
 *	babyos/kernel/pool.h
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


#ifndef _POOL_H_
#define _POOL_H_

#include "babyos/types.h"
#include "spinlock.h"


#define SMALL_POOL_SIZE 64


typedef struct object_pool_obj_s {
	struct object_pool_obj_s*	m_next;
} object_pool_obj_t;


class object_pool_t {
public:
	void init(uint32 obj_size);
	void free_object_nolock(void* obj);
	void free_object(void* obj);
	void* alloc_from_pool();
	uint32 get_available();

private:
	uint32				m_obj_size;
	uint32				m_available;
	object_pool_obj_t*	m_free_list;
    spinlock_t          m_lock;
};


#endif
