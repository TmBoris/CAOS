 .text
    .global partition
mid:
    movl k, %edx
    ret
quit:
    addl $1, k
    movl k, %eax
    subl $1, %eax
    sall $1, %eax
    cmpw (%eax, %ebx), %di
    jz mid
    jmp quit
swap:
    movw 2(%ecx, %ebx), %ax
    push %ax
    movw (%ecx, %ebx), %ax
    movw %ax, 2(%ecx, %ebx)
    pop %ax
    movw %ax, (%ecx, %ebx)
    jmp second
second:
    addl $1, j
    cmpl j, %esi
    jz partition
    movl j, %ecx
    subl $1, %ecx
    sall $1, %ecx
    movw (%ecx, %ebx), %ax
    cmpw %ax, 2(%ecx, %ebx)
    jl swap
    jmp second
partition:
    movl $0, j
    addl $1, i
    cmpl i, %esi
    jg second
    jz quit
    jmp partition

    .data
i: .int 0
j: .int 0
k: .int 0