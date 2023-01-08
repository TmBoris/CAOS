    .text
    .global main
main:
    movl $0, %eax
    movl $0, %edx
    call readi64
    cmpl $0, %edx
    jl less
    jg gr
    cmpl $0, %eax
    jl gr
    call writei32
    call finish
gr:
    movl $2147483647, %eax
    call writei32
    call finish
less:
    cmpl $-1, %edx
    jz xxx
    movl $-2147483648, %eax
    call writei32
    call finish
xxx:
    cmpl $0, %eax
    jl ok
    movl $-2147483648, %eax
    call writei32
    call finish
ok:
    call writei32
    call finish


