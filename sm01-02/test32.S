    .text
    .global main
main:
    call readi32
    movl %eax, %edi
    mov $arr, %ebx
loop:
    movl q, %ecx
    movl %ecx, (%ebx)
    add $4, %ebx
    subl $1, q
    cmpl $0, q
    jnz loop
    movl $9, %esi
    mov $arr, %ebx
    call partition
    movl %edx, %eax
    call writei32
    mov $arr, %ecx
loop2:
    movl (%ecx), %eax
    call writei32
    add $4, %ecx
    subl $1, f
    cmpl $0, f
    jnz loop2
    call finish

    .data
arr: .skip 40
q: .int 9
f: .int 9


