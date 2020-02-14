/*
 * guzhoudiaoke@126.com
 * 2020-01-21
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

