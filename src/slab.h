/*
 *	babyos/kernel/slab.h
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




#ifndef _SLAB_H_
#define _SLAB_H_


#include "types.h"
#include "dlist.h"
#include "kernel.h"
#include "spinlock.h"


class kmem_cache_t;
class slab_t {
public:
    static const uint32 END = -1;

    dlist_node_t  m_list_node;
    uint64        m_color_off;
    void*         m_mem;
    uint32        m_in_use;
    uint32        m_first_free;
    kmem_cache_t* m_cache;
};


class kmem_cache_t {
public:
    kmem_cache_t();
    ~kmem_cache_t();

    void create(uint32 size);
    void destroy();

    void* alloc();
    void  free(void* obj);

private:
    slab_t* get_slab();
    void* alloc_one();
    void  free_one(void* obj);
    void* slab_alloc(slab_t* slab);
    void  grow();
    void  init_objs(slab_t* slab);
    void  estimate();

private:
    dlist_t    m_slabs_partial;
    dlist_t    m_slabs_full;
    dlist_t    m_slabs_free;

    uint32     m_objsize;
    uint32     m_num_per_slab;
    uint32     m_gfporder;
    spinlock_t m_lock;
};

class cache_sizes_t {
public:
    void init(uint64 size, kmem_cache_t* cache);

    uint64 m_size;
    kmem_cache_t* m_cache;
};


#endif
