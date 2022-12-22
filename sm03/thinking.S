    .text
    .global main
main:
    push %ebp
    movl %esp, %ebp
    call getchar
    call writei32
    push %eax
    call tolower
    call writei32
    leave
    ret