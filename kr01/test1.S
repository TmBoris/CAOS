    .text
    .global main

main:
    call readi32
    movl %eax, A
    call readi32
    movl %eax, B

    call process

    movl R, %eax
    call writei32
    call finish