    .text
    .global process
    .global main
main:
    push %ebp
    movl %esp, %ebp
    push $16
    call call_process
    pop %eax
    leave
    ret


process:
    push %ebp
    mov %esp, %ebp
    push %ebp
    mov %esp, %ebp
    mov $4, %eax
    mov %ebp, %esp
    pop %ebp
    ret