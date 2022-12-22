    .text
    .global main
main:
    push %ebp
    movl %esp, %ebp
    movl $dict1, %ecx
    movl $dict2, %edx
loop:
    push %edx
    push %ecx
    call getchar
    push %eax
    call tolower
    addl $4, %esp
    pop %ecx
    pop %edx
    incl (%ecx, %eax, 4)
    cmpl $'\n', %eax
    jnz loop
loop2:
    push %edx
    push %ecx
    call getchar
    push %eax
    call tolower
    addl $4, %esp
    pop %ecx
    pop %edx
    incl (%edx, %eax, 4)
    cmpl $'\n', %eax
    jnz loop2
    movl $0, %eax
    push %esi
loop_eq:
    cmpl $' ', %eax
    jz space
    movl (%edx, %eax, 4), %esi
    cmpl %esi, (%ecx, %eax, 4)
    jnz no
    cmpl $999, %eax
    jz yes
    incl %eax
    jmp loop_eq
space:
    incl %eax
    jmp loop_eq
yes:
    push $Yes
    call puts
    addl $8, %esp
    xor %eax, %eax
    leave
    ret
no:
    push $No
    call puts
    addl $8, %esp
    xor %eax, %eax
    leave
    ret

    .data
dict1: .skip 4000
dict2: .skip 4000
Yes: .string "YES"
No: .string "NO"