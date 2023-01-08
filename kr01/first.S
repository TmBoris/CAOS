    .text
    .global process
exit:
    ret
process:
    movl B, %ecx
    sarl $4, %ecx
    movl A, %eax
    addl A, %eax
    jo exit
    movl $0, %edx
    subl %ecx, %edx
    addl %eax, %edx
    jo exit
    addl A, %edx
    jo exit
    movl %edx, R
    ret


    .data
    .global A, B, R
A: .int 0
B: .int 0
R: .int 0