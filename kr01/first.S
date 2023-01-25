    .text
    .global process
exit:
    ret
process:
    movl B, %ecx
    sarl $4, %ecx
    movl stack, %eax
    addl stack, %eax
    jo exit
    movl $0, %edx
    subl %ecx, %edx
    addl %eax, %edx
    jo exit
    addl stack, %edx
    jo exit
    movl %edx, R
    ret


    .data
    .global stack, B, R
stack: .int 0
B: .int 0
R: .int 0