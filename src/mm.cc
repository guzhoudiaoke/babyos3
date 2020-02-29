/*
 *	babyos/kernel/mm.cc
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


#include "mm.h"
#include "vm.h"
#include "pipe.h"


mm_t::mm_t()
{
}

mm_t::~mm_t()
{
}

void mm_t::init()
{
    m_bootmem.init();
    m_buddy.init();

    m_vma_cache.create(sizeof(vm_area_t));
    m_pipe_cache.create(sizeof(pipe_t));

    m_cache_with_size[0].init(32, NULL);
    m_cache_with_size[1].init(64, NULL);
    m_cache_with_size[2].init(128, NULL);
    m_cache_with_size[3].init(256, NULL);
    m_cache_with_size[3].init(512, NULL);
    m_cache_with_size[3].init(1024, NULL);
    //m_cache_with_size[3].init(2048, NULL);
    //m_cache_with_size[3].init(4096, NULL);
    //m_cache_with_size[3].init(8192, NULL);
    //m_cache_with_size[3].init(16384, NULL);
    //m_cache_with_size[3].init(32768, NULL);
    //m_cache_with_size[3].init(65536, NULL);
}

void* mm_t::boot_mem_alloc(uint32 size, bool page_align)
{
    return P2V(m_bootmem.mem_alloc(size, page_align));
}

bootmem_t* mm_t::bootmem()
{
    return &m_bootmem;
}

kmem_cache_t* mm_t::vma_cache()
{
    return &m_vma_cache;
}

kmem_cache_t* mm_t::pipe_cache()
{
    return &m_pipe_cache;
}

uint64 mm_t::alloc_pages(uint32 order)
{
    return m_buddy.alloc_pages(order);
}

void mm_t::free_pages(uint64 pa, uint32 order)
{
    m_buddy.free_pages(pa, order);
}

void mm_t::inc_page_ref(uint64 pa)
{
    m_buddy.inc_page_ref(pa);
}

uint32 mm_t::dec_page_ref(uint64 pa)
{
    return m_buddy.dec_page_ref(pa);
}

uint32 mm_t::get_page_ref(uint64 pa)
{
    return m_buddy.get_page_ref(pa);
}

uint32 mm_t::get_free_page_num()
{
    return m_buddy.get_free_page_num();
}


void* mm_t::kmalloc(uint64 size)
{
    for (uint32 i = 0; i < MAX_CACHE_OF_SIZE_INDEX; i++) {
        if (m_cache_with_size[i].m_size >= size) {
            return m_cache_with_size[i].m_cache->alloc();
        }
    }

    return NULL;
}

void mm_t::kfree(void* p)
{
    if (p == NULL) {
        return;
    }

    uint64 flags;
    local_irq_save(flags);
    slab_t* slab = (slab_t *) ((uint64)p & (~(PAGE_SIZE-1)));
    kmem_cache_t* cache = slab->m_cache;
    cache->free(p);
    local_irq_restore(flags);
}
