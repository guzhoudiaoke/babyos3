/*
 *	babyos/kernel/vm.h
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
#include "vm.h"
#include "x86.h"
#include "string.h"
//#include "signal.h"


void vmm_t::init()
{
    m_mmap = NULL;
    m_pml4_table = NULL;
}

int32 vmm_t::copy(const vmm_t& vmm)
{
    m_pml4_table = copy_pml4_table(vmm.m_pml4_table);
    if (m_pml4_table == NULL) {
        return -1;
    }

    asm volatile("movq %0, %%cr3": :"r" (VA2PA(m_pml4_table)));

    if (copy_vma(vmm.m_mmap)) {
        return -1;
    }

    return 0;
}

pte_t* vmm_t::copy_page_table(pte_t* page_table)
{
    /* alloc pdp_table */
    pte_t* ret = (pte_t *) PA2VA(os()->buddy()->alloc_pages(0));
    if (ret == NULL) {
        return NULL;
    }

    memcpy(ret, page_table, PAGE_SIZE);

    /* copy pdp table entries */
    for (uint32 i = 0; i < PTRS_PER_PDPE; i++) {
        uint64 pte = page_table[i];
        if (!(pte & PTE_P)) {
            continue;
        }

        uint64 pa = (pte & (PAGE_MASK));
        os()->buddy()->inc_page_ref(pa);
        ret[i] = page_table[i] &= (~PTE_W);
    }

    return ret;
}

pde_t* vmm_t::copy_pd_table(pde_t* pd_table)
{
    /* alloc pdp_table */
    pde_t* ret = (pde_t *) PA2VA(os()->buddy()->alloc_pages(0));
    if (ret == NULL) {
        return NULL;
    }

    memcpy(ret, pd_table, PAGE_SIZE);

    /* copy pdp table entries */
    for (uint32 i = 0; i < PTRS_PER_PDPE; i++) {
        uint64 pde = pd_table[i];
        if (!(pde & PTE_P)) {
            continue;
        }

        pte_t* src = (pte_t *) PA2VA(pde & PAGE_MASK);
        pte_t* copy = copy_page_table(src);
        if (copy == NULL) {
            return NULL;
        }

        ret[i] = VA2PA(copy) | (pde & (~PAGE_MASK));
    }

    return ret;
}

pdpe_t* vmm_t::copy_pdp_table(pdpe_t* pdp_table)
{
    /* alloc pdp_table */
    pdpe_t* ret = (pdpe_t *) PA2VA(os()->buddy()->alloc_pages(0));
    if (ret == NULL) {
        return NULL;
    }

    memcpy(ret, pdp_table, PAGE_SIZE);

    /* copy pdp table entries */
    for (uint32 i = 0; i < PTRS_PER_PML4E; i++) {
        uint64 pdpe = pdp_table[i];
        if (!(pdpe & PTE_P)) {
            continue;
        }

        pde_t* src = (pde_t *) PA2VA(pdpe & PAGE_MASK);
        pde_t* copy = copy_pd_table(src);
        if (copy == NULL) {
            return NULL;
        }

        ret[i] = VA2PA(copy) | (pdpe & (~PAGE_MASK));
    }

    return ret;
}

pml4e_t* vmm_t::copy_pml4_table(pml4e_t* pml4_table)
{
    /* alloc pml4_table */
    pml4e_t* ret = (pml4e_t *) PA2VA(os()->buddy()->alloc_pages(0));
    if (ret == NULL) {
        return NULL;
    }

    memcpy(ret, pml4_table, PAGE_SIZE);

    /* copy pml4 table entries */
    for (uint32 i = 0; i < PML4E_PER_PAGE/2; i++) {
        uint64 pml4e = pml4_table[i];
        ret[i] = pml4e;

        if (!(pml4e & PTE_P)) {
            continue;
        }

        pdpe_t* src = (pdpe_t *) PA2VA(pml4e & PAGE_MASK);
        pdpe_t* copy = copy_pdp_table(src);
        if (copy == NULL) {
            goto failed;
        }
        ret[i] = VA2PA(copy) | (pml4e & (~PAGE_MASK));
    }

    return ret;

failed:
    os()->panic("failed to copy pml4 table, panic for now\n");

    /* TODO: destroy allocated memory */
    return NULL;
}

int32 vmm_t::copy_vma(vm_area_t* mmap)
{
    vm_area_t* tail = NULL;
    vm_area_t* p = mmap;
    while (p != NULL) {
        vm_area_t* vma = (vm_area_t *) os()->get_obj_pool(VMA_POOL)->alloc_from_pool();
        if (vma == NULL) {
            return -1;
        }

        *vma = *p;
        vma->m_next = NULL;

        if (tail == NULL) {
            m_mmap = vma;
        }
        else {
            tail->m_next = vma;
        }
        tail = vma;

        p = p->m_next;
    }

    return 0;
}

/* Look up the first VMA which satisfies  addr < vm_end,  NULL if none. */
vm_area_t* vmm_t::find_vma(uint64 addr)
{
    vm_area_t* vma = m_mmap;
    while (vma != NULL) {
        if (addr < vma->m_end) {
            return vma;
        }
        vma = vma->m_next;
    }
    return NULL;
}

vm_area_t* vmm_t::find_vma(uint64 addr, vm_area_t*& prev)
{
    prev = NULL;
    vm_area_t* vma = m_mmap;
    while (vma != NULL) {
        if (addr < vma->m_end) {
            return vma;
        }
        prev = vma;
        vma = vma->m_next;
    }
    return NULL;
}

uint32 vmm_t::insert_vma(vm_area_t* vma)
{
    vm_area_t* prev = NULL;
    vm_area_t* p = m_mmap;
    while (p != NULL) {
        if (p->m_start >= vma->m_end) {
            break;
        }
        prev = p;
        p = p->m_next;
    }

    if (prev != NULL && prev->m_end > vma->m_start) {
        os()->console()->kprintf(RED,
                                 "insert_vma: inserting: [%x, %x], overlaped with [%x, %x]\n",
                                 vma->m_start, vma->m_end, prev->m_start, prev->m_end);
        return -1;
    }

    vma->m_next = p;
    if (prev != NULL) {
        prev->m_next = vma;
    }
    else {
        m_mmap = vma;
    }

    /* merge prev and vma */
    if (prev != NULL && prev->m_end == vma->m_start) {
        if (prev->m_page_prot == vma->m_page_prot && prev->m_flags == vma->m_flags) {
            prev->m_end = vma->m_end;
            prev->m_next = p;
            os()->get_obj_pool(VMA_POOL)->free_object(vma);
            vma = prev;
        }
    }

    /* merge vma and p */
    if (p != NULL && vma->m_end == p->m_start) {
        if (vma->m_page_prot == p->m_page_prot && vma->m_flags == p->m_flags) {
            vma->m_end = p->m_end;
            vma->m_next = p->m_next;
            os()->get_obj_pool(VMA_POOL)->free_object(p);
        }
    }

    return 0;
}

uint64 vmm_t::get_unmapped_area(uint64 len)
{
    uint32 addr = VM_UNMAPPED_BASE;

    vm_area_t* vma = find_vma(addr);
    while (vma != NULL) {
        if (USER_VM_SIZE - len < addr) {
            return 0;
        }

        if (addr + len <= vma->m_start) {
            return addr;
        }
        addr = vma->m_end;
        vma = vma->m_next;
    }

    return addr;
}

/*
 * for now, the mmap should:
 *	1) if MAP_FIXED, addr should align with PAGE_SIZE
 *	2) [addr, addr+len] not intersect with a vma already exist
 */
uint64 vmm_t::do_mmap(uint64 addr, uint64 len, uint32 prot, uint32 flags)
{
    /* make len align with PAGE_SIZE */
    len = PAGE_ALIGN(len);

    /* len is 0, nothing to do */
    if (len == 0) {
        return addr;
    }

    /* out of range */
    if (len > USER_VM_SIZE || addr > USER_VM_SIZE || addr > USER_VM_SIZE - len) {
        return -1;
    }

    /* if MAP_FIXED, the addr should align with PAGE_SIZE */
    if (flags & MAP_FIXED) {
        if (addr & ~PAGE_MASK) {
            return -1;
        }

        /* check [addr, addr+len] not in a vm_area */
        vm_area_t* p = find_vma(addr);
        if (p != NULL && addr + len > p->m_start) {
            os()->console()->kprintf(RED, "do_mmap: addr: %p is overlaped with vma: [%p, %p]\n", 
                    addr, p->m_start, p->m_end);
            return -1;
        }
    }
    else {
        addr = get_unmapped_area(len);
        if (addr == 0) {
            os()->console()->kprintf(RED, "do_mmap: failed to get_unmaped_area\n");
            return -1;
        }
    }

    /* alloc a vma from pool */
    vm_area_t* vma = (vm_area_t*) os()->get_obj_pool(VMA_POOL)->alloc_from_pool();
    if (vma == NULL) {
        os()->console()->kprintf(RED, "do_mmap: failed to alloc vma\n");
        return -1;
    }

    /* setup vma */
    vma->m_start = addr;
    vma->m_end = addr + len;
    vma->m_flags = (prot & (VM_READ | VM_WRITE | VM_EXEC));
    vma->m_page_prot = prot;
    vma->m_next = NULL;

    /* insert vma into list, and do merge */
    if (insert_vma(vma)) {
        os()->console()->kprintf(RED, "do_mmap: failed to insert vma: [%x, %x]\n", vma->m_start, vma->m_end);
        return -1;
    }

    return addr;
}

uint32 vmm_t::remove_vma(vm_area_t* vma, vm_area_t* prev)
{
    if (prev != NULL) {
        prev->m_next = vma->m_next;
    }
    else {
        m_mmap = vma->m_next;
    }

    os()->get_obj_pool(VMA_POOL)->free_object(vma);

    return 0;
}

int32 vmm_t::do_munmap(uint64 addr, uint64 len)
{
    /* addr should align with PAGE_SIZE */
    if ((addr & ~PAGE_MASK) || addr > USER_VM_SIZE || len > USER_VM_SIZE-addr) {
        return -1;
    }

    /* len is 0, nothing to do */
    if ((len = PAGE_ALIGN(len)) == 0) {
        return 0;
    }

    /* find the vma, addr < vma->m_end */
    vm_area_t* prev = NULL;
    vm_area_t* vma = find_vma(addr, prev);
    if (vma == NULL) {
        return -1;
    }

    /* make sure m_start <= addr< addr+len <= m_end */
    if (addr < vma->m_start || addr+len > vma->m_end) {
        return -1;
    }

    /* alloc a new vma, because the vma may split to 2 vma, such as:
     * [start, addr, addr+len, end] => [start, addr], [addr+len, end] */
    vm_area_t* vma_new = (vm_area_t*) os()->get_obj_pool(VMA_POOL)->alloc_from_pool();
    if (vma_new == NULL) {
        return -1;
    }

    /* set up the new vma and link to list */
    vma_new->m_start = addr+len;
    vma_new->m_end = vma->m_end;
    vma->m_end = addr;
    vma_new->m_next = vma->m_next;
    vma->m_next = vma_new;

    /* check if first part need to remove */
    if (vma->m_start == vma->m_end) {
        remove_vma(vma, prev);
        vma = prev;
    }

    /* check if second part need to remove */
    if (vma_new->m_start == vma_new->m_end) {
        remove_vma(vma_new, vma);
    }

    /* need to unmap the physical page */
    free_page_range(addr, addr+len);

    return 0;
}

int32 vmm_t::do_protection_fault(vm_area_t* vma, uint64 addr, bool write)
{
    uint64 pa = vmm_t::va_to_pa(current->m_vmm.get_pml4_table(), (void *)addr);
    if (pa == -1ull) {
        os()->console()->kprintf(RED, "protection fault, no physical page found\n");
        return -1;
    }

    /* this is a write protection fault, but this vma can't write */
    if (write && !(vma->m_flags & VM_WRITE)) {
        os()->console()->kprintf(RED, "protection fault, ref count: %u!\n",
                os()->buddy()->get_page_ref(pa));
        return -1;
    }

    /* not shared */
    if (os()->buddy()->get_page_ref(pa) == 1) {
        make_pte_write((void *) addr);
        return 0;
    }

    /* this page is shared, now only COW can share page */
    uint64 p = os()->buddy()->alloc_pages(0);
    memcpy(PA2VA(p), PA2VA(pa & PAGE_MASK), PAGE_SIZE);

    os()->buddy()->free_pages(pa, 0);
    vmm_t::map_pages(m_pml4_table, (void*) addr, p, PAGE_SIZE, PTE_W | PTE_U);

    return 0;
}

void vmm_t::send_sig_segv()
{
    //os()->get_process_mgr()->send_signal_to(current->m_pid, SIG_SEGV);
}

/*
 * bit 0: 0 no page found, 1 protection fault
 * bit 1: 0 read, 1 write
 * bit 2: 0 kernel, 1 user
 */
uint32 vmm_t::do_page_fault(trap_frame_t* frame)
{
    uint64 addr = 0x0;
    __asm__ volatile("movq %%cr2, %0" : "=r" (addr));

    vm_area_t* vma = find_vma(addr);

    /* not find the vma or out of range */
    if (vma == NULL || vma->m_start > addr) {
        if (frame->err & 0x4) {
            if (vma != NULL && (vma->m_flags & VM_STACK) && addr + 64 >= frame->rsp) {
                expand_stack(vma, addr);
                goto good_area;
            }
        }

        goto sig_segv;
    }

good_area:
    /* find a vma and the addr in this vma */
    if (vma != NULL && vma->m_start <= addr) {
        if (frame->err & 0x1) {
            return do_protection_fault(vma, addr, (uint64) (frame->err & 2));
        }

        /* no page found */
        uint64 pa = os()->buddy()->alloc_pages(0);
        addr = (addr & PAGE_MASK);
        vmm_t::map_pages(m_pml4_table, (void*) addr, pa, PAGE_SIZE, PTE_W | PTE_U);
    }

    return 0;

sig_segv:
    os()->console()->kprintf(RED, "cpu: %u, process: %u, segment fault, addr: %lx, err: %lx, cs: %p, rip: %p, rsp: %p\n",
                             os()->cpu(), current->m_pid, addr, frame->err, frame->cs, frame->rip, frame->rsp);

    os()->uart()->kprintf("do_page_fault sgement fault, cpu: %u, process: %u, addr: %lx, err: %lx,"
                          "cs: %p, rip: %p, rsp: %p\n",
                          os()->cpu(), current->m_pid, addr, frame->err, frame->cs, frame->rip, frame->rsp);

    //send_sig_segv();
    current->exit();
    return -1;
}

pml4e_t* vmm_t::get_pml4_table()
{
    return m_pml4_table;
}

void vmm_t::set_pml4_table(pml4e_t* pml4_table)
{
    m_pml4_table = pml4_table;
}

void vmm_t::make_pte_write(void* va)
{
    if ((uint64) va >= KERNEL_BASE) {
        return;
    }

    pml4e_t pml4e = m_pml4_table[PML4E_INDEX(va)];
    if (!(pml4e & PTE_P)) {
        return;
    }

    pdpe_t* pdp_table = (pdpe_t *) (PA2VA(pml4e & PAGE_MASK));
    pdpe_t pdpe = pdp_table[PDPE_INDEX(va)];
    if (!(pdpe & PTE_P)) {
        return;
    }

    pde_t* pd_table = (pde_t *) (PA2VA(pdpe & PAGE_MASK));
    pde_t pde = pd_table[PDE_INDEX(va)];
    if (!(pde & PTE_P)) {
        return;
    }

    pte_t* page_table = (pte_t *) PA2VA(pde & PAGE_MASK);
    pte_t pte = page_table[PTE_INDEX(va)];
    if (!(pte & PTE_P)) {
        return;
    }

    page_table[PTE_INDEX(va)] |= PTE_W;

    asm volatile("movq %0, %%cr3": :"r" (VA2PA(m_pml4_table)));
}

int32 vmm_t::expand_stack(vm_area_t* vma, uint64 addr)
{
    addr &= PAGE_MASK;
    vma->m_start = addr;

    return 0;
}

void vmm_t::free_page_range(uint64 start, uint64 end)
{
    uint64 addr = start & PAGE_MASK;
    while (addr < end) {
        uint64 pa = vmm_t::va_to_pa(current->m_vmm.get_pml4_table(), (void *) addr);
        if (pa != -1ull) {
            os()->buddy()->free_pages(pa, 0);
        }

        addr += PAGE_SIZE;
    }
    asm volatile("movq %0, %%cr3": :"r" (VA2PA(m_pml4_table)));
}

void vmm_t::free_pdp_table(pdpe_t* pdp_table)
{
    for (uint32 i = 0; i < PTRS_PER_PDPE; i++) {
        uint64 pdpe = pdp_table[i];
        if (!(pdpe & PTE_P)) {
            continue;
        }

        uint64 pa = pdpe & PAGE_MASK;
        pde_t* pd_table = (pde_t *) (PA2VA(pa));
        free_pd_table(pd_table);

        os()->buddy()->free_pages(pa, 0);
        pdp_table[i] = 0;
    }
}

void vmm_t::free_pd_table(pde_t* pd_table)
{
    for (uint32 i = 0; i < PTRS_PER_PDE; i++) {
        uint64 pde = pd_table[i];
        if (!(pde & PTE_P)) {
            continue;
        }

        uint64 pa = pde & PAGE_MASK;
        os()->buddy()->free_pages(pa, 0);
        pd_table[i] = 0;
    }
}

void vmm_t::free_pml4_table()
{
    for (uint32 i = 0; i < PML4E_PER_PAGE/2; i++) {
        uint64 pml4e = m_pml4_table[i];
        if (!(pml4e & PTE_P)) {
            continue;
        }

        uint64 pa = pml4e & PAGE_MASK;
        pdpe_t* pdp_table = (pdpe_t *) (PA2VA(pa));
        free_pdp_table(pdp_table);

        os()->buddy()->free_pages(pa, 0);
        m_pml4_table[i] = 0;
    }

    asm volatile("movq %0, %%cr3": :"r" (VA2PA(m_pml4_table)));
}

void vmm_t::release()
{
    /* 1. pages */
    vm_area_t* vma = m_mmap;
    while (vma != NULL) {
        free_page_range(vma->m_start, vma->m_end);
        vma = vma->m_next;
    }

    /* 2. page table */
    free_pml4_table();

    /* 3. vmas */
    vma = m_mmap;
    while (vma != NULL) {
        vm_area_t* del = vma;
        vma = vma->m_next;
        os()->get_obj_pool(VMA_POOL)->free_object(del);
    }
    m_mmap = NULL;
}

void vmm_t::map_pages(pml4e_t *pml4_table, void *va, uint64 pa, uint64 size, uint32 perm)
{
    uint8 *v = (uint8 *) (((uint64)va) & PAGE_MASK);
    uint8 *e = (uint8 *) (((uint64)va + size) & PAGE_MASK);
    pa = (pa & PAGE_MASK);

    while (v < e) {
        pdpe_t* pdp_table = get_pdp_table(pml4_table, v);
        pde_t* pd_table = get_pd_table(pdp_table, v);
        pte_t* page_table = get_page_table(pd_table, v);

        for (uint32 i = PTE_INDEX(v); i < PTRS_PER_PDE; i++) {
            if (v >= e) {
                break;
            }
            page_table[i] = pa | PTE_P | perm;
            v += PAGE_SIZE;
            pa += PAGE_SIZE;
        }
    }

    asm volatile("movq %0, %%cr3": :"r" (VA2PA(pml4_table)));
}

pdpe_t* vmm_t::get_pdp_table(pml4e_t* pml4_table, void* v)
{
    pml4e_t pml4e = pml4_table[PML4E_INDEX(v)];
    pdpe_t* pdp_table = NULL;
    if ((pml4e) & PTE_P) {
        pdp_table = (pdpe_t *) (PA2VA(pml4e & PAGE_MASK));
    }
    else {
        uint64 pdpt_pa = os()->buddy()->alloc_pages(0);
        pdp_table = (pdpe_t *) PA2VA(pdpt_pa);
        memset(pdp_table, 0, PAGE_SIZE);
        pml4_table[PML4E_INDEX(v)] = (pdpt_pa | PTE_P | PTE_W | PTE_U);
    }

    return pdp_table;
}

pde_t* vmm_t::get_pd_table(pdpe_t* pdp_table, void* v)
{
    pdpe_t pdpe = pdp_table[PDPE_INDEX(v)];
    pde_t* pd_table = NULL;
    if ((pdpe) & PTE_P) {
        pd_table = (pde_t *) (PA2VA(pdpe & PAGE_MASK));
    }
    else {
        uint64 pdt_pa = os()->buddy()->alloc_pages(0);
        pd_table = (pde_t *) PA2VA(pdt_pa);
        memset(pd_table, 0, PAGE_SIZE);
        pdp_table[PDPE_INDEX(v)] = (pdt_pa | PTE_P | PTE_W | PTE_U);
    }

    return pd_table;
}

pte_t* vmm_t::get_page_table(pde_t* pd_table, void* v)
{
    pde_t pde = pd_table[PDE_INDEX(v)];
    pte_t* page_table = NULL;
    if ((pde) & PTE_P) {
        page_table = (pte_t *) (PA2VA(pde & PAGE_MASK));
    }
    else {
        uint64 pt_pa = os()->buddy()->alloc_pages(0);
        page_table = (pte_t *) PA2VA(pt_pa);
        memset(page_table, 0, PAGE_SIZE);
        pd_table[PDE_INDEX(v)] = (pt_pa | PTE_P | PTE_W | PTE_U);
    }

    return page_table;
}

uint64 vmm_t::va_to_pa(pml4e_t* pml4_table, void* va)
{
    if ((uint64) va >= KERNEL_BASE) {
        return VA2PA(va);
    }

    pml4e_t pml4e = pml4_table[PML4E_INDEX(va)];
    if (!(pml4e & PTE_P)) {
        return -1;
    }

    pdpe_t* pdp_table = (pdpe_t *) (PA2VA(pml4e & PAGE_MASK));
    pdpe_t pdpe = pdp_table[PDPE_INDEX(va)];
    if (!(pdpe & PTE_P)) {
        return -1;
    }

    pde_t* pd_table = (pde_t *) (PA2VA(pdpe & PAGE_MASK));
    pde_t pde = pd_table[PDE_INDEX(va)];
    if (!(pde & PTE_P)) {
        return -1;
    }

    pte_t* page_table = (pte_t *) PA2VA(pde & PAGE_MASK);
    pte_t pte = page_table[PTE_INDEX(va)];
    if (!(pte & PTE_P)) {
        return -1;
    }

    uint64 offset = (uint64) va - ((uint64) va & PAGE_MASK);
    return (page_table[PTE_INDEX(va)] & PAGE_MASK) + offset;
}

