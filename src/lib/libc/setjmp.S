
.global _setjmp1
.type _setjmp1, 2
_setjmp1:
    mov 4(%rsp), %eax
    mov %rbx, 0(%rax)
    mov %rsi, 8(%rax)
    mov %rdi, 16(%rax)
    mov %rbp, 24(%rax)
    lea 4(%rsp), %rcx
    mov %rcx, 32(%rax)
    mov (%rsp), %rcx
    mov %rcx, 40(%rax)
    xor %rax, %rax
    ret

.global longjmp1
.type longjmp1, 2
longjmp1:
    mov 8(%rsp), %rdx
    mov 16(%rsp), %rax
    mov 0(%rdx), %rbx
    mov 8(%rdx), %rsi
    mov 16(%rdx), %rdi
    mov 24(%rdx), %rbp
    mov 32(%rdx), %rcx
    mov %rcx, %rsp
    mov 40(%rdx), %rcx
    test %rax, %rax
    jnz  .nonzero
    mov 1, %rax
.nonzero:
    jmp *%rcx

