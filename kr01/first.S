    .text
    .global process
process:
    movl A, %eax
    addl A, %eax
    addl A, %eax
    movl B, %ecx
    sarl $4, %ecx
    subl %ecx, %eax
    movl %eax, R
    ret


    .data
    .global A, B, R
A: .int 0
B: .int 0
R: .int 0