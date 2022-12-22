    .text
    .global main
    .global foo
    .global bar
main:
    call bar
    ret
bar:
    push %ebp
    movl %esp, %ebp
    call foo
    leave
    ret

foo:
    push %ebp
    movl %esp, %ebp
    push %eax
    movl j, %eax
    cmpl i, %eax
    jg xxx
    call print_tb
    leave
    ret
xxx:
    addl $1, i
    call bar
    leave
    ret


    .data
i: .int 0
j: .int 3
