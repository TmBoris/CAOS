    .text
    .global partition
iforward:
    addl $1, i
    jmp partition
jbackward:
    subl $1, %esi
    jmp partition
swap:
    movl i, %ecx
    movl (%ebx, %ecx, 4), %eax
    push %eax
    movl j, %ecx
    movl (%ebx, %ecx, 4), %eax
    movl i, %ecx
    movl %eax, (%ebx, %ecx, 4)
    pop %eax
    movl j, %ecx
    movl %eax, (%ebx, %ecx, 4)
    addl $1, i
    subl $1, %esi
    jmp partition
mid:
    movl k, %edx
    ret
quit:
    addl $1, k
    movl k, %eax
    subl $1, %eax
    cmpl %edi, (%ebx, %eax, 4)
    jge mid
    jmp quit
partition:
    cmpl i, %esi
    jz quit
    movl i, %ecx
    cmpl %edi, (%ebx, %ecx, 4)
    jl iforward
    movl %esi, j
    subl $1, j
    movl j, %ecx
    cmpl %edi, (%ebx, %ecx, 4)
    jge jbackward
    jmp swap


    .data
i: .int 0
j: .int 0
k: .int 0
// k = pivot = %edi = %di
// j = size = %esi