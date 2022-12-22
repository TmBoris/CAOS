    .text
    .global main
main:
    movl $0, %eax
    call readi32
    movl %eax, %ecx
    movl %eax, %esi
    mov $arr, %ebx
loop:
    movl $0, %eax
    call readi32
    movl i, %edx
    movw %ax, (%ebx, %edx, 2)
    addl $1, i
    cmpl i, %ecx
    jnz loop
    mov $arr, %ebx
    // push %ebx
    // push %ecx

    // call sort_by_abs

    movl %edx, %eax
    call writei32

loop2:
    movl $0, %eax
    movl j, %edx
    movw (%ebx, %edx, 2), %ax
    call writei32
    addl $1, j
    cmpl j, %esi
    jnz loop2
    call finish

    .data
arr: .skip 1000
i: .int 0
j: .int 0


