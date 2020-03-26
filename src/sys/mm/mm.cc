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
#include "babyos.h"
#include "signal.h"


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

    m_cache_cache.create(sizeof(kmem_cache_t), 8, 1);
    m_vma_cache.create(sizeof(vm_area_t), 8, 1);
    m_pipe_cache.create(sizeof(pipe_t), 8, 1);
    m_siginfo_cache.create(sizeof(siginfo_t), 8, 1);

    uint32 size = 32;
    uint32 order = 1;
    for (int i = 0; i < MAX_CACHE_OF_SIZE_INDEX; i++) {
        kmem_cache_t* cache = (kmem_cache_t *) m_cache_cache.alloc();
        if (size > 1024) {
            order++;
        }
        cache->create(size, 8, order);
        m_cache_with_size[i].init(size, cache);
        size *= 2;
    }
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

kmem_cache_t* mm_t::siginfo_cache()
{
    return &m_siginfo_cache;
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

    return nullptr;
}

void mm_t::kfree(void* p)
{
    if (p == nullptr) {
        return;
    }

    uint64 flags;
    local_irq_save(flags);
    kmem_cache_t* cache = get_page_cache(V2P(p));
    cache->free(p);
    local_irq_restore(flags);
}

void mm_t::set_page_cache(uint64 pa, kmem_cache_t* ca)
{
    page_t* page = m_buddy.get_page(pa);
    page->cache = (uint64) ca;
}

void mm_t::set_page_slab(uint64 pa, slab_t* slab)
{
    page_t* page = m_buddy.get_page(pa);
    page->slab = (uint64) slab;
}

kmem_cache_t* mm_t::get_page_cache(uint64 pa)
{
    page_t* page = m_buddy.get_page(pa);
    return (kmem_cache_t *) page->cache;
}

slab_t* mm_t::get_page_slab(uint64 pa)
{
    page_t* page = m_buddy.get_page(pa);
    return (slab_t *) page->slab;
}

