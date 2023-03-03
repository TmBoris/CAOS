    .text
    .global product
product:
    push %ebp
    movl %esp, %ebp

    movss N, %xmm0

    movl 8(%ebp), %ecx
    cmpl N, %ecx
    jz exit
    movl %ecx, N
start:
    cmpl one, %ecx
    jl exit
    cmpl four, %ecx
    jge pack
    jmp one_num

one_num:
    movl 12(%ebp), %edx
    movl N, %eax
    subl %ecx, %eax
    movss (%edx, %eax, 4), %xmm3

    movl 16(%ebp), %edx
    movss (%edx, %eax, 4), %xmm4
    subl $1, %ecx

    xor %edx, %edx
    divl two
    cmpl $0, %edx
    jg inversed_pre_end
    jmp pre_end
pack:
    movl 12(%ebp), %edx
    movl N, %eax
    subl %ecx, %eax
    movups (%edx, %eax, 4), %xmm3

    movl 16(%ebp), %edx
    movups (%edx, %eax, 4), %xmm4

    subl $4, %ecx
    jmp pre_end
inversed_pre_end:
    mulps %xmm3, %xmm4

    movl $inversed_ones, %edx
    jmp end
pre_end:
    mulps %xmm3, %xmm4

    movl $ones, %edx
    jmp end
end:
    movups (%edx), %xmm3
    mulps %xmm3, %xmm4

    haddps %xmm4, %xmm4
    haddps %xmm4, %xmm4

    addss %xmm4, %xmm0

    jmp start


exit:

    movl 20(%ebp), %eax
    movss %xmm0, (%eax)

    leave
    ret


    .data
N: .int 0
one: .int 1
two: .int 2
four: .int 4
ones: .float 1
      .float -1
      .float 1
      .float -1
inversed_ones: .float -1
              .float 1
              .float -1
              .float 1