    .global main
main:
    movl %esp, %ebp
    movl $5, %eax
    push %eax
    movl %ebp, %edx
    movl -4(%ebp), %eax
    call writei32
    movl -4(%edx), %eax
    call writei32
    call finish