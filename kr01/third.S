    .text
    .global main
main:
    call readi64
    cmpl $0, %edx
    jl less
    jg gr
    call writei32
    call finish
gr:
    movl $2147483647, %eax
    call writei32
    call finish
less:
    movl $-2147483648, %eax
    call writei32
    call finish