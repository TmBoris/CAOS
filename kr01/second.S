    .text
    .global call_process
call_process:
    push %ebp
    movl $xxx, %ebp
    jmp process
xxx:
    push %eax
    push $d
    call printf
    movl %ebp, %esp
    pop %ebp
    xor %eax, %eax
    ret

    .data
d: .string "%d\n"