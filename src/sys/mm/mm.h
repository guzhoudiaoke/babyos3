/*
 *	babyos/kernel/mm.h
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
 *  2020-02-28		created
 */


#ifndef _MM_H_
#define _MM_H_


#include "bootmem.h"
#include "buddy.h"
#include "slab.h"


#define MAX_CACHE_OF_SIZE_INDEX 12


class mm_t {
public:
    mm_t();
    ~mm_t();

    void init();

    bootmem_t* bootmem();
    kmem_cache_t* vma_cache();
    kmem_cache_t* pipe_cache();
    kmem_cache_t* siginfo_cache();

    void*  boot_mem_alloc(uint32 size, bool page_align);
    uint64 alloc_pages(uint32 order);
    void   free_pages(uint64 pa, uint32 order);
    uint32 get_free_page_num();

    void   inc_page_ref(uint64 phy_addr);
    uint32 dec_page_ref(uint64 phy_addr);
    uint32 get_page_ref(uint64 phy_addr);

    void* kmalloc(uint64 size);
    void  kfree(void* p);

    void set_page_cache(uint64 pa, kmem_cache_t* ca);
    void set_page_slab(uint64 pa, slab_t* slab);
    kmem_cache_t* get_page_cache(uint64 pa);
    slab_t* get_page_slab(uint64 pa);

private:
    bootmem_t     m_bootmem;
    buddy_t       m_buddy;

    kmem_cache_t  m_cache_cache;
    kmem_cache_t  m_vma_cache;
    kmem_cache_t  m_pipe_cache;
    kmem_cache_t  m_siginfo_cache;
    cache_sizes_t m_cache_with_size[MAX_CACHE_OF_SIZE_INDEX];
};


#endif
