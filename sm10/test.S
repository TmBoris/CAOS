    .text
    .global product
product:
    push %ebp
    movl %esp, %ebp

    movl N, %edi

    movl 16(%ebp), %edx
    movups (%edx, %edi, 4), %xmm5

    haddps %xmm5, %xmm5
    haddps %xmm5, %xmm5

    movl 20(%ebp), %eax
    movss %xmm5, (%eax)

    leave
    ret




    .data
N: .int 0
one: .int 1
two: .int 2
three: .int 3
four: .int 4
nulls: .int 0
       .int 0
       .int 0
       .int 0
ones: .int 1
      .int -1
      .int 1
      .int -1
f: .int 0
   .int 0
   .int 0
   .int 0
s: .int 0
   .int 0
   .int 0
   .int 0