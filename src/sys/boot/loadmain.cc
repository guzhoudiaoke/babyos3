/*
 *	babyos/boot/loadmain.cc
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
 *  2020-01-20		created
 */


#include "types.h"
#include "kernel.h"
#include "elf.h"
#include "x86.h"

typedef enum hd_state {
    HD_STATE_READY = 0x40,
    HD_STATE_BUSY  = 0x80,
} hd_state_e;

typedef enum io_cmd_e {
    IO_CMD_READ = 0x20,
} io_cmd_e;

typedef void (*kernel_entry_t)(void);


void wait_disk()
{
    while ((inb(0x1f7) & (HD_STATE_BUSY | HD_STATE_READY)) != HD_STATE_READY) {
        ;
    }
}

void read_sector(void* buf, uint32 lba)
{
    wait_disk();

    outb(0x1f2, 1);                     /* sector num */
    outb(0x1f3, lba & 0xff);
    outb(0x1f4, (lba >> 8)  & 0xff);
    outb(0x1f5, (lba >> 16) & 0xff);
    outb(0x1f6, 0xe0 | ((lba >> 24) & 0xff));
    outb(0x1f7, IO_CMD_READ);

    wait_disk();
    insl(0x1f0, buf, SECT_SIZE / 4);
}

/*
 * pa:     the buffer to read data, will be aligned by SECT_SIZE
 * offset: where to read from disk (byte)
 * size:   how many byte to read
 */
void read_segment(void* pa, uint32 offset, uint32 size)
{
    uint8* p = (uint8 *) pa - (offset % SECT_SIZE);;
    uint32 lba = offset / SECT_SIZE;
    uint8* end = p + size;

    for (; p < end; p += SECT_SIZE, lba++) {
        read_sector(p, lba);
    }
}

extern "C"
void loadmain(void)
{
    char buf[512] = {0};
    elf64_hdr_t* elf = (elf64_hdr_t *) buf;
    read_sector(elf, KERNEL_ELF_LBA);
    if (*((uint32 *)elf->e_ident) != ELF_MAGIC) {
        return;
    }

    /* read segments */
    uint32 elf_offset = SECT_SIZE * KERNEL_ELF_LBA;
    elf64_phdr_t* ph = (elf64_phdr_t *) ((uint8 *)elf + elf->e_phoff);
    for (int i = 0; i < elf->e_phnum; i++, ph++) {
        read_segment((void *) ph->p_paddr, elf_offset + ph->p_offset, ph->p_filesz);
        if (ph->p_memsz >= ph->p_filesz) {
            stosb((void *) (ph->p_paddr + ph->p_filesz), 0, ph->p_memsz - ph->p_filesz);
        }
    }

    /* find entry from elf, and call */
    kernel_entry_t entry = (kernel_entry_t) elf->e_entry;
    entry();
}

