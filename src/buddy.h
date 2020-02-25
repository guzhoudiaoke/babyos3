/*
 *	babyos/kernel/buddy.h
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

#ifndef _BUDDY_H_
#define _BUDDY_H_

#include "types.h"
#include "page.h"


#define MAX_ORDER     6
#define MAP_NR(addr)  (((unsigned long)(addr)) >> PAGE_SHIFT)



typedef struct free_list_s {
	struct free_list_s*	next;
	struct free_list_s*	prev;
	uint32*				bitmap;
} free_list_t;

typedef struct free_area_s {
	free_list_t free_list[MAX_ORDER+1];
	uint64		base;
} free_area_t;


class buddy_t {
public:
    buddy_t();
    ~buddy_t();

    void init();
    void dump();

    uint64 alloc_pages(uint32 order);
    void   free_pages(uint64 pa, uint32 order);

    void   inc_page_ref(uint64 phy_addr);
    uint32 dec_page_ref(uint64 phy_addr);
    uint32 get_page_ref(uint64 phy_addr);

    uint32 get_free_page_num();


private:
    void   init_pages();
    uint64 get_buddy(uint64 addr, uint32 mask);
    int    mark_used(uint64 addr, uint32 order);
    uint64 expand(free_list_t* addr, uint32 low, uint32 high);

private:
    page_t*     m_pages;
    free_area_t m_free_area;
    atomic_t    m_free_page_num;
};

#endif
