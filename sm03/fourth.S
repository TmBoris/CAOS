    .text
    .global sort_by_abs
neg_f:
    movw $0, %cx
    subw (%eax), %cx
    cmpw $0, 12(%ebp)
    jl neg_s
    movl 12(%ebp), %eax
    movw (%eax), %dx
    subw %dx, %cx
    movw %cx, %ax
    cmpw $0, %ax
    jl less
    jg greater
    jz equal
neg_s:
    movw $0, %dx
    subw (%eax), %dx
    subw %dx, %cx
    movw %cx, %ax
    cmpw $0, %ax
    jl less
    jg greater
    jz equal
compar:
    push %ebp
    movl %esp, %ebp
    push %edx
    push %ecx
    movl 8(%ebp), %eax
    cmpw $0, (%eax)
    jl neg_f
    movw (%eax), %cx
    movl 12(%ebp), %eax
    cmpw $0, (%eax)
    jl neg_s
    movw (%eax), %dx
    subw %dx, %cx
    movw %cx, %ax
    cmpw $0, %ax
    jl less
    jg greater
    jz equal
less:
    movl $-1, %eax
    pop %ecx
    pop %edx
    leave
    ret
greater:
    movl $1, %eax
    pop %ecx
    pop %edx
    leave
    ret
equal:
    movl $0, %eax
    pop %ecx
    pop %edx
    leave
    ret
sort_by_abs:
    push %ebp
    movl %esp, %ebp
    push %ecx
    push $compar
    movl $2, %ecx
    push %ecx
    movl 12(%ebp), %ecx
    push %ecx
    movl 8(%ebp), %ecx
    push %ecx
    call qsort
    addl $16, %esp
    pop %ecx
    leave
    ret

    .data
width: .int 2