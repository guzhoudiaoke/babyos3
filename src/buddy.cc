/*
 *	babyos/kernel/buddy.cc
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
 *  2020-02-15		created
 */


#include "buddy.h"
#include "babyos.h"
#include "page.h"
#include "math.h"
#include "x86.h"


static inline void add_to_head(free_list_t* head, free_list_t * entry)
{
    entry->prev = head;
    entry->next = head->next;
    head->next->prev = entry;
    head->next = entry;
}

static inline void remove_head(free_list_t * entry)
{
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;
}


buddy_t::buddy_t()
{
}

buddy_t::~buddy_t()
{
}

void buddy_t::init_pages()
{
    uint64 mem_end = os()->bootmem()->get_end_usable_pa();
    uint32 page_num = (uint64) (mem_end) / PAGE_SIZE;
    uint32 size = page_num * sizeof(page_t);
    m_pages = (page_t *) PA2VA(os()->bootmem()->mem_alloc(size, 0));

    for (uint32 i = 0; i < page_num; i++) {
        atomic_set(&m_pages[i].ref, 1);
    }
}

void buddy_t::init()
{
    uint64 mem_start = os()->bootmem()->get_start_usable_pa();
    uint64 mem_end = os()->bootmem()->get_end_usable_pa();
    uint64 mask = PAGE_MASK;
    uint32 bitmap_size = 0;

    /* init bitmap for free areas */
    for (int i = 0; i <= MAX_ORDER; i++) {
        m_free_area.free_list[i].prev = m_free_area.free_list[i].next = &m_free_area.free_list[i];
        mask += mask;
        mem_end = ((uint64)mem_end) & mask;
        bitmap_size = (uint32 (mem_end - mem_start)) >> (PAGE_SHIFT + i);
        bitmap_size = (bitmap_size + 7) >> 3;
        bitmap_size = (bitmap_size + sizeof(uint32) - 1) & ~(sizeof(uint32)-1);

        uint64 pa = os()->bootmem()->mem_alloc(bitmap_size, false);
        m_free_area.free_list[i].bitmap = (uint32 *) PA2VA(pa);
        memset(m_free_area.free_list[i].bitmap, 0, bitmap_size);
    }

    os()->uart()->kprintf("init bitmap done\n");

    /* init ref count of all pages */
    init_pages();
    os()->uart()->kprintf("init pages done\n");

    /* mark free area bases as start of boot mem, and free boot mem to buddy */
    mem_start = os()->bootmem()->get_start_usable_pa();
    m_free_area.base = (mem_start + ~mask) & mask;

    /* free boot mem to buddy */
    atomic_set(&m_free_page_num, 0);
    uint64 pa = m_free_area.base;
    for (; pa < mem_end; pa += PAGE_SIZE) {
        free_pages(pa, 0);
    }

    os()->uart()->kprintf("free boot mem done\n");
}

uint64 buddy_t::expand(free_list_t* addr, uint32 low, uint32 high)
{
    uint32 size = PAGE_SIZE << high;
    while (low < high) {
        high--;
        size >>= 1;
        add_to_head(m_free_area.free_list+high, addr);
        mark_used(VA2PA(addr), high);
        addr = (free_list_t *) (size + (uint64) addr);
    }

    uint64 pa = VA2PA(addr);
    uint64 p = pa;
    for (uint32 i = 0; i < math_t::pow(2, low); i++, p += PAGE_SIZE) {
        inc_page_ref(p);
    }
    return pa;
}

uint64 buddy_t::alloc_pages(uint32 order)
{
    atomic_sub(math_t::pow(2, order), &m_free_page_num);
    os()->uart()->kprintf("alloc order: %u, free page num: %u\n", order, atomic_read(&m_free_page_num));

    free_list_t* queue = m_free_area.free_list + order;
    uint32 new_order = order;
    do {
        free_list_t* next = queue->next;
        if (queue != next) {
            queue->next = next->next;
            next->next->prev = queue;
            mark_used((uint64) VA2PA(next), new_order);
            return expand(next, order, new_order);
        }
        new_order++;
        queue++;
    } while (new_order <= MAX_ORDER);

    return 0;
}

void buddy_t::free_pages(uint64 pa, uint32 order)
{
    // dec the ref count, if it's not 0, don't free the pages
    if (!dec_page_ref(pa)) {
        return;
    }

    atomic_add(math_t::pow(2, order), &m_free_page_num);

    uint64 address = (uint64) pa;
    uint32 index = MAP_NR(address - (uint64)m_free_area.base) >> (1 + order);
    uint64 mask = PAGE_MASK << order;

    address &= mask;
    while (order < MAX_ORDER) {
        if (!change_bit(index, m_free_area.free_list[order].bitmap)) {
            break;
        }

        uint64 buddy = get_buddy(address, mask);
        remove_head((free_list_t *)PA2VA(buddy));
        order++;
        index >>= 1;
        mask <<= 1;
        address &= mask;
    }

    //os()->uart()->kprintf("free %p to order: %u\n", pa, order);
    add_to_head(m_free_area.free_list+order, (free_list_t *) PA2VA(address));
}

void buddy_t::inc_page_ref(uint64 phy_addr)
{
    page_t* page = &m_pages[phy_addr >> PAGE_SHIFT];
    atomic_inc(&page->ref);
}

uint32 buddy_t::dec_page_ref(uint64 phy_addr)
{
    page_t* page = &m_pages[phy_addr >> PAGE_SHIFT];
    if (page->ref.counter <= 0) {
        //os()->panic("ref count <= 0 when dec ref");
    }
    return atomic_dec_and_test(&page->ref);
}

uint32 buddy_t::get_page_ref(uint64 phy_addr)
{
    page_t* page = &m_pages[phy_addr >> PAGE_SHIFT];
    return atomic_read(&page->ref);
}

uint64 buddy_t::get_buddy(uint64 addr, uint32 mask)
{
    uint64 buddy = ((addr - (uint64)m_free_area.base) ^ (-mask)) + (uint64)m_free_area.base;
    return buddy;
}

int buddy_t::mark_used(uint64 addr, uint32 order)
{
    return change_bit(MAP_NR(addr - (uint64)m_free_area.base) >> (1+order),
                      m_free_area.free_list[order].bitmap);
}

void buddy_t::dump()
{
    for (int i = 0; i <= MAX_ORDER; i++) {
        free_list_t* free_list = &m_free_area.free_list[i];
        free_list_t* p = free_list->next;
        os()->uart()->kprintf("free list %d: ", i);
        while (p != free_list) {
            os()->uart()->kprintf("%p->", p);
            p = p->next;
        }
        os()->uart()->kprintf("\n");
    }
}
