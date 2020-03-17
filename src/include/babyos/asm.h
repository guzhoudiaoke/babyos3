#ifndef _ASM_H_
#define _ASM_H_

#define __bionic_asm_align 16
#define __bionic_asm_custom_entry(f)
#define __bionic_asm_custom_end(f)
#define __bionic_asm_function_type @function


#define ENTRY(f) \
    .text; \
    .globl f; \
    .align __bionic_asm_align; \
    .type f, __bionic_asm_function_type; \
    f: \
    __bionic_asm_custom_entry(f); \
    .cfi_startproc \

#define END(f) \
    .cfi_endproc; \
    .size f, .-f; \
    __bionic_asm_custom_end(f) \


#endif
