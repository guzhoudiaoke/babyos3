#
# guzhoudiaoke@126.com
# 2020-01-20
#

#include "kernel.h"

.section .text
.global _start

.org 0
_start:
	jmp		main


main:
	movl	$(SEG_KDATA<<3),		%eax
	movw	%ax,					%ds
	movw	%ax,					%es
	movw	%ax,					%fs
	movw	%ax,					%gs
	movw	%ax,					%ss
	movl	$STACK_PM_BOTTOM,		%esp

    movl    (VIDEO_INFO_ADDR+8),    %esi
    addl    $((300*1024+400)*3+2),  %esi
    movl    $0x100,                 %ecx
1:
    movb    $0xff,                  %al
    movb    %al,                    (%esi)
    addl    $3,                     %esi
    decl    %ecx
    cmpl    $0x0,                   %ecx
    jne 1b

2:
	call loadmain
    hlt
	jmp		2b
