    .text
    .global main
main:
    call readi64
    call writei32
    movl %edx, %eax
    call writei32
    call finish