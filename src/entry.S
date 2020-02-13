#include "kernel.h"

    .code64
    .text
    .global _start

_start = entry - KERNEL_BASE

    .global entry
entry:
    movq    $main,  %rax
    jmp     *%rax
