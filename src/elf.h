/*
 *	babyos/kernel/elf.h
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
 *  2020-01-21		created
 */


#ifndef _ELF_H_
#define _ELF_H_

#include "types.h"

// these structs copy from linux kernel 
// include/uapi/linux/elf.h

#define EI_NIDENT	16

/* 64-bit ELF base types. */
typedef uint64	Elf64_Addr;
typedef uint16	Elf64_Half;
typedef int16	Elf64_SHalf;
typedef uint64	Elf64_Off;
typedef int32	Elf64_Sword;
typedef uint32	Elf64_Word;
typedef uint64	Elf64_Xword;
typedef int64	Elf64_Sxword;


#define ELF_MAGIC   0x464c457FU   // "\x7FELF"
#define PT_LOAD     1

typedef struct elf64_hdr {
    unsigned char	e_ident[EI_NIDENT];	/* ELF "magic number" */
    Elf64_Half e_type;
    Elf64_Half e_machine;
    Elf64_Word e_version;
    Elf64_Addr e_entry;		/* Entry point virtual address */
    Elf64_Off  e_phoff;		/* Program header table file offset */
    Elf64_Off  e_shoff;		/* Section header table file offset */
    Elf64_Word e_flags;
    Elf64_Half e_ehsize;
    Elf64_Half e_phentsize;
    Elf64_Half e_phnum;
    Elf64_Half e_shentsize;
    Elf64_Half e_shnum;
    Elf64_Half e_shstrndx;
} elf64_hdr_t;

typedef struct elf64_phdr {
    Elf64_Word  p_type;
    Elf64_Word  p_flags;
    Elf64_Off   p_offset;		/* Segment file offset */
    Elf64_Addr  p_vaddr;		/* Segment virtual address */
    Elf64_Addr  p_paddr;		/* Segment physical address */
    Elf64_Xword p_filesz;		/* Segment size in file */
    Elf64_Xword p_memsz;		/* Segment size in memory */
    Elf64_Xword p_align;		/* Segment alignment, file & memory */
} elf64_phdr_t;

#endif

