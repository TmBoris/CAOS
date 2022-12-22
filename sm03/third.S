    .text
    .global main
main:
    movl $xxx, %eax
    push %eax
    push %ebp
    movl %esp, %ebp
    jmp process
xxx:
    push %eax
    push $d
    call printf
    addl $8, %esp
    xor %eax, %eax
    ret

    .data
d: .string "%d"