    .text
    .global spin_lock
    .global spin_unlock

spin_lock:
    push %ebp
    movl %esp, %ebp
    movl $1, %eax
loop:
    movl 8(%ebp), %ecx
    xchg %eax, (%ecx)
    test %eax, %eax
    jz loop_end
    pause
    jmp loop

loop_end:
    movl $1, (%ecx)
    leave
    ret

spin_unlock:
    push %ebp
    movl %esp, %ebp
    movl 8(%ebp), %ecx
    movl $0, (%ecx)
    leave
    ret