/*
 *	babyos/kernel/bootmem.h
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
 *  2020-02-14		created
 */


#ifndef _BOOTMEM_H_
#define _BOOTMEM_H_


#include "types.h"
#include "page.h"
#include "bootinfo.h"


class bootmem_t {
public:
    bootmem_t();
    ~bootmem_t();

    void init();
    void map_pages(void* va, uint64 pa, uint64 length, uint32 perm);
    uint64 mem_alloc(uint32 size, bool page_align);
    boot_info_t* get_boot_info();
    uint64 get_start_usable_pa();
    uint64 get_end_usable_pa();
    pml4e_t* get_pml4();


private:
    static uint64 early_va2pa(void* va);
    static void*  early_pa2va(uint64 pa);

    void init_boot_info();
    void init_mem_range();
    void init_page_map();

    pdpe_t* get_pdp_table(pml4e_t* pml4_table, void* v);
    pde_t*  get_pd_table(pdpe_t* pdp_table, void* v);
    pde_t*  get_page_table(pde_t* pd_table, void* v);

private:
    boot_info_t      m_boot_info;
    uint64           m_start_pa;
    uint64           m_end_pa;
    uint64           m_pml4_pa;
};


#endif
