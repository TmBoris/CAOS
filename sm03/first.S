    .text
    .global main
overflow:
    push    $Overflow
    push    $s
    call    printf
    leave
    pop     %eax
    pop     %ecx
    xor     %eax, %eax
    ret
end:
    movl    -4(%ebp), %ecx
    push    %ecx
    push    $d
    call    printf
    leave
    pop     %eax
    pop     %ecx
    xor     %eax, %eax
    ret
rw:
    leal    -8(%ebp), %eax
    pushl   %eax
    pushl   $decimal
    call    scanf
    cmpl    $0, %eax
    jl      end
    movl    -8(%ebp), %ecx
    addl    %ecx, -4(%ebp)
    jo      overflow
    addl    $8, %esp
    jmp     rw
main:
    push    %ecx
    push    %eax
    pushl   %ebp
    movl    %esp, %ebp
    movl    $0, %ecx
    push    %ecx
    jmp     rw

    .data
decimal:  .string "%d"
s:        .string "%s\n"
d:        .string "%d\n"
Overflow: .string "Overflow"