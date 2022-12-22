    .text
    .global print_tb
MAIN:
    movl $mn, %ecx
    push %ecx
    call puts // вывели название функции
    pop %ecx

    pop %eax // вернули все на место, что брали поиспользовать
    pop %edx
    pop %ecx
    leave
    ret
FOO:
    push %edx // чтобы puts его нам не попортил

    movl $fo, %ecx
    push %ecx
    call puts // вывели название функции
    pop %ecx

    pop %edx
    movl (%edx), %edx // перешли в old_ebp
    jmp reg
BAR: // все аналогично FOO
    push %edx

    movl $ba, %ecx
    push %ecx
    call puts
    pop %ecx

    pop %edx
    movl (%edx), %edx
    jmp reg
compar:
    movl $mn, %ecx
    cmpl %eax, 5(%ecx)
    jz MAIN // сравнили ret adr с началом main

    movl $fo, %ecx
    cmpl %eax, 4(%ecx)
    jz FOO // с началом foo

    movl $ba, %ecx
    cmpl %eax, 4(%ecx)
    jz BAR // с началом bar

    subl $1, %eax
    jmp compar
reg:
    movl 4(%edx), %eax // нашли return adress
    jmp compar
print_tb:
    push %ebp
    movl %esp, %ebp // пролог

    push %ecx
    push %edx
    push %eax // запушем пару регистров, чтобы ими пользоваться

    push $ptb
    call puts
    pop %edx // вывели название функции, в которую пришли - print_tb

    movl %ebp, %edx // переложили в ругой регистр, чтобы не портить
    movl $mn, %ecx
    movl $main, 5(%ecx)
    movl $fo, %ecx
    movl $foo, 4(%ecx)
    movl $ba, %ecx
    movl $bar, 4(%ecx) // составили табличку имя функции:адрес_начала

    jmp reg


    .data
mn: .string "main"
    .int 0
fo: .string "foo"
    .int 0
ba: .string "bar"
    .int 0
ptb: .string "print_tb"
