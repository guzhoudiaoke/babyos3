#ifndef _ASM_H_
#define _ASM_H_

#define __babyos_asm_align 16
#define __babyos_asm_custom_entry(f)
#define __babyos_asm_custom_end(f)
#define __babyos_asm_function_type @function


#define ENTRY(f) \
    .text; \
    .globl f; \
    .align __babyos_asm_align; \
    .type f, __babyos_asm_function_type; \
    f: \
    __babyos_asm_custom_entry(f); \
    .cfi_startproc \

#define END(f) \
    .cfi_endproc; \
    .size f, .-f; \
    __babyos_asm_custom_end(f) \



#endif
