/*
 *	babyos/kernel/bootmem.cc
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


#include "bootmem.h"
#include "babyos.h"
#include "kernel.h"
#include "x86.h"


extern uint8 _begin[];   // defined by kernel.ld
extern uint8 _data[];    // defined by kernel.ld
extern uint8 _end[];     // defined by kernel.ld


bootmem_t::bootmem_t()
{
}

bootmem_t::~bootmem_t()
{
}

void bootmem_t::init()
{
    init_boot_info();
    init_mem_range();
    init_page_map();
}

void bootmem_t::init_boot_info()
{
    m_boot_info.video_info = (video_info_t *) early_pa2va(VIDEO_INFO_ADDR);
    m_boot_info.mem_layout = (memory_layout_t *) early_pa2va(MEM_INFO_ADDR);
    m_boot_info.asc16_font = (void *) early_pa2va(FONT_ASC16_ADDR);
}

void bootmem_t::init_mem_range()
{
    /* set the end of kernel code/data as start of boot memory */
    m_start_pa = (uint64)_end - KERNEL_LOAD_BASE;
    m_end_pa = m_start_pa;

    os()->uart()->kprintf("the memory info from int 0x15, eax=0xe820:\n");
    os()->uart()->kprintf("type\t\taddress\t\t\tlength\n");

    /* print mem layout */
    for (uint32 i = 0; i < m_boot_info.mem_layout->num_of_range; i++) {
        address_range_t* range = &m_boot_info.mem_layout->ranges[i];
        uint64 addr = ((uint64)range->base_addr_high << 32) + range->base_addr_low;
        uint64 end = addr + ((uint64)range->length_high << 32) + range->length_low;
        os()->uart()->kprintf("0x%8x\t0x%16lx\t0x%16lx\n",
                              range->type, addr, end);

        if (range->type == 1 && end > m_end_pa) {
            m_end_pa = end;
        }
    }

    os()->uart()->kprintf("usable usable mem range: [0x%lx, 0x%lx)\n",
                          m_start_pa, m_end_pa);
}

uint64 bootmem_t::mem_alloc(uint32 size, bool page_align)
{
    if (page_align) {
        m_start_pa = PAGE_ALIGN(m_start_pa);
    }

    uint64 p = m_start_pa;
    m_start_pa += size;

    return p;
}

pdpe_t* bootmem_t::get_pdp_table(pml4e_t* pml4_table, void* v)
{
    pml4e_t pml4e = pml4_table[PML4E_INDEX(v)];
    pdpe_t* pdp_table = NULL;
    if ((pml4e) & PTE_P) {
        pdp_table = (pdpe_t *) (early_pa2va(pml4e & PAGE_MASK));
    }
    else {
        uint64 pdpt_pa = mem_alloc(PAGE_SIZE, true);
        pdp_table = (pdpe_t *) early_pa2va(pdpt_pa);
        memset(pdp_table, 0, PAGE_SIZE);
        pml4_table[PML4E_INDEX(v)] = (pdpt_pa | PTE_P | PTE_W | PTE_U);
    }

    return pdp_table;
}

pde_t* bootmem_t::get_pd_table(pdpe_t* pdp_table, void* v)
{
    pdpe_t pdpe = pdp_table[PDPE_INDEX(v)];
    pde_t* pd_table = NULL;
    if ((pdpe) & PTE_P) {
        pd_table = (pde_t *) (early_pa2va(pdpe & PAGE_MASK));
    }
    else {
        uint64 pdt_pa = mem_alloc(PAGE_SIZE, true);
        pd_table = (pde_t *) early_pa2va(pdt_pa);
        memset(pd_table, 0, PAGE_SIZE);
        pdp_table[PDPE_INDEX(v)] = (pdt_pa | PTE_P | PTE_W | PTE_U);
    }

    return pd_table;
}

pte_t* bootmem_t::get_page_table(pde_t* pd_table, void* v)
{
    pde_t pde = pd_table[PDE_INDEX(v)];
    pte_t* page_table = NULL;
    if ((pde) & PTE_P) {
        page_table = (pte_t *) (early_pa2va(pde & PAGE_MASK));
    }
    else {
        uint64 pt_pa = mem_alloc(PAGE_SIZE, true);
        page_table = (pte_t *) early_pa2va(pt_pa);
        memset(page_table, 0, PAGE_SIZE);
        pd_table[PDE_INDEX(v)] = (pt_pa | PTE_P | PTE_W | PTE_U);
    }

    return page_table;
}

void bootmem_t::map_pages(void *va, uint64 pa, uint64 length, uint32 perm)
{
    uint8 *v = (uint8 *) (((uint64)va) & PAGE_MASK);
    uint8 *e = (uint8 *) (((uint64)va + length) & PAGE_MASK);
    pa = (pa & PAGE_MASK);
    os()->uart()->kprintf("map: [%p, %p) -> [%16lx, %16lx)\n",
                          v, e, pa, pa+length);

    pml4e_t* pml4_table = (pml4e_t *)early_pa2va(m_pml4_pa);
    while (v < e) {
        pdpe_t* pdp_table = get_pdp_table(pml4_table, v);
        pde_t* pd_table = get_pd_table(pdp_table, v);
        pte_t* page_table = get_page_table(pd_table, v);

        os()->uart()->kprintf("%16lx, pml4_table[%3d]=%16lx, pdp_table[%3d]=%16lx, pd_table[%3d]=%16lx\n",
                              v,
                              PML4E_INDEX(v), pml4_table[PML4E_INDEX(v)],
                              PDPE_INDEX(v), pdp_table[PDPE_INDEX(v)],
                              PDE_INDEX(v), pd_table[PDE_INDEX(v)]);

        for (uint32 i = PTE_INDEX(v); i < PTRS_PER_PDE; i++) {
            if (v >= e) {
                break;
            }
            page_table[i] = pa | PTE_P | perm;
            v += PAGE_SIZE;
            pa += PAGE_SIZE;
        }
    }
}

void bootmem_t::init_page_map()
{
    /* alloc a page for pml4 - the top level of page table */
    m_pml4_pa = (uint64)mem_alloc(PAGE_SIZE, true);
    memset(early_pa2va(m_pml4_pa), 0, PAGE_SIZE);

    /* map first 1MB: [KERNEL_LOAD_BASE, _begin) -> [0, 1MB) */
    map_pages((void*)KERNEL_LOAD_BASE,           /* va */
              0,                                 /* pa */
              (uint64)_begin - KERNEL_LOAD_BASE, /* length */
              0);

    /* map kernel code: [_begin, _data] -> [1MB, V2P(_data)] */
    map_pages((void*)KERNEL_LOAD_ADDR,           /* va */
              (uint64)_begin - KERNEL_LOAD_BASE, /* pa */
              (uint64)(_data) - (uint64)(_begin),/* length */
              PTE_W);

    /* map kernel data: [_data, _end] -> [V2P(_data), V2P(_end)] */
    map_pages((void*) _data,                    /* va */
              (uint64)_data - KERNEL_LOAD_BASE, /* pa */
              (uint64)_end - (uint64)_data,     /* length */
              PTE_W);

    /* map all normal memory */
    uint64 pa = (uint64)_end - KERNEL_LOAD_BASE;
    for (uint32 i = 0; i < m_boot_info.mem_layout->num_of_range; i++) {
        address_range_t* range = &m_boot_info.mem_layout->ranges[i];
        uint64 begin = ((uint64)range->base_addr_high << 32) + range->base_addr_low;
        uint64 end = begin + ((uint64)range->length_high << 32) + range->length_low;
        if (range->type == 1 && begin <= pa && pa <= end) {
            map_pages(PA2VA(pa),  /* va */
                      pa,         /* pa */
                      end - pa,   /* length */
                      PTE_W);
            pa = end;
        }
    }

    /* map video vram */
    video_info_t* video_info = m_boot_info.video_info;
    uint32 bytes = video_info->width*video_info->height*video_info->bits_per_pixel/3;
    map_pages(PA2VA(video_info->vram_base_addr),  /* va */
              video_info->vram_base_addr,         /* pa */
              bytes,                              /* length */
              PTE_P | PTE_W);

    /* set pml4 to cr3 */
    set_cr3(m_pml4_pa);
}

uint64 bootmem_t::early_va2pa(void* va)
{
    return (uint64)va - KERNEL_LOAD_BASE;
}

void* bootmem_t::early_pa2va(uint64 pa)
{
    return (void *)(pa + KERNEL_LOAD_BASE);
}

boot_info_t* bootmem_t::get_boot_info()
{
    return &m_boot_info;
}

uint64 bootmem_t::get_start_usable_pa()
{
    return m_start_pa;
}

uint64 bootmem_t::get_end_usable_pa()
{
    return m_end_pa;
}

pml4e_t* bootmem_t::get_pml4()
{
    os()->uart()->kprintf("bootmem pml4: %p\n", PA2VA(m_pml4_pa));
    return (pml4e_t *) PA2VA(m_pml4_pa);
}
