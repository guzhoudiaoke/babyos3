/*
 *	babyos/sys/device/framebuffer.cc
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
 *  2020-03-29		created
 */


#include "framebuffer.h"
#include "babyos.h"
#include "fb_ioctl.h"
#include "errno.h"
#include "kmath.h"
#include "kernel.h"


static int framebuffer_ioctl(inode_t* inode, int cmd, uint64 arg)
{
    return os()->framebuffer()->ioctl(cmd, arg);
}

/***********************************************************************************/

framebuffer_t::framebuffer_t()
{
}

framebuffer_t::~framebuffer_t()
{
}

void framebuffer_t::init()
{
    os()->get_dev(DEV_FRAMEBUFFER)->ioctl = framebuffer_ioctl;
}

int framebuffer_t::ioctl(int cmd, uint64 arg)
{
    uint64* p = (uint64 *)arg;

    switch (cmd) {
    case FB_GET_WIDTH:
        *p = os()->vbe()->width();
        break;
    case FB_GET_HEIGHT:
        *p = os()->vbe()->height();
        break;
    case FB_GET_DEPTH:
        *p = os()->vbe()->depth();
        break;
    case FB_MAP:
    {
        uint32 vram_size = os()->vbe()->vram_size();
        uint8* vram = os()->vbe()->vram();

        uint64 vaddr = current->m_vmm.do_mmap(0ull,
                                              vram_size,
                                              PROT_READ | PROT_WRITE | PROT_EXEC,
                                              0);
        if (vaddr == -1ull) {
            return -ENOMEM;
        }

        pml4e_t* pml4_table = current->m_vmm.get_pml4_table();
        vmm_t::map_pages(pml4_table, 
                         (void *) vaddr, 
                         (uint64) V2IO(vram), 
                         vram_size, 
                         PTE_W | PTE_U);
        *p = vaddr;
        break;
    }
    case FB_SWAP_BUFFER:
        os()->vbe()->swap_buffer();
        break;
    default:
        return -EINVAL;
    }

    return 0;
}