    .text
    .global partition
iforward:
    addl $1, i
    jmp xxx
jbackward:
    subl $1, %edx
    jmp xxx
swap:
    movl i, %ecx
    movw (%ebx, %ecx, 2), %ax
    push %ax
    movl j, %ecx
    movw (%ebx, %ecx, 2), %ax
    movl i, %ecx
    movw %ax, (%ebx, %ecx, 2)
    pop %ax
    movl j, %ecx
    movw %ax, (%ebx, %ecx, 2)
    addl $1, i
    subl $1, %edx
    jmp xxx
mid:
    movl k, %edx
    movl $0, %eax
    ret
quit:
    addl $1, k
    movl k, %eax
    cmpw %di, (%ebx, %eax, 2)
    jge mid
    jmp quit
xxx:
    cmpl i, %edx
    jz quit

    movl i, %ecx
    cmpw %di, (%ebx, %ecx, 2)
    jl iforward

    movl %edx, j
    subl $1, j
    movl j, %ecx
    cmpw %di, (%ebx, %ecx, 2)
    jge jbackward

    jmp swap
partition:
    // посатвить кастыль если длина 1
    movl %esi, %edx
    jmp xxx



    .data
i: .int 0
j: .int 0
k: .int -1
// k = pivot = %edi = %di
// j = size = %edx, in the end put mid in %edx