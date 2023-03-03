    .text
    .global okay
okay:
    push %ebp
    movl %esp, %ebp
    movl 24(%ebp), %eax
    movsd 16(%ebp), %xmm2
    mulsd %xmm2, %xmm2
    mulsd %xmm2, %xmm2
    movsd 8(%ebp), %xmm1
    mulsd %xmm1, %xmm1
    mulsd %xmm1, %xmm1
    addsd %xmm1, %xmm2
    sqrtsd %xmm2, %xmm1
    movsd %xmm1, (%eax)
    leave
    ret