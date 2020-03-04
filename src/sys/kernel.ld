/*
*	babyos/kernel/kernel.h
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


OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(main)


SECTIONS
{
    . = 0xffffffff80100000;

    PROVIDE(_begin = .);

    .text : {
        *(.text .rela.text .stub .text.* .gnu.linkonce.t.*)
    }

    PROVIDE(_etext = .);

    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    . = ALIGN(0x1000);

    PROVIDE(_data = .);

    .data : {
        *(.data)
    }

    PROVIDE(_edata = .);

    .bss : {
        *(.bss)
    }

    . = ALIGN(0x1000);

    PROVIDE(_end = .);

    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack)
    }
}

