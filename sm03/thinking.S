    .text
    .global main
main:
    push %ebp
    movl %esp, %ebp
    push $3
    push $4
    push $-2147483649
    call my.args.forward
    leave
    ret