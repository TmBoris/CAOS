    .text
    .global supersum
xxx:
    movl size, %esi
    movb $1, (%ecx, %esi, 1)
    incl size
    movl size, %eax
    pop %esi
    ret
quit:
    popf
    jc xxx
    movl size, %eax
    pop %esi
    ret
only_b:
    movl maxj, %edx
    cmpl %edx, j
    jz quit
    movl j, %esi
    movb (%ebx, %esi, 1), %dl

    popf
    adcb $0, %dl
    pushf

    movl size, %esi
    movb %dl, (%ecx, %esi, 1)
    incl j
    incl size
    jmp sum
only_a:
    movl maxi, %edx
    cmpl %edx, i
    jz quit
    movl i, %esi
    movb (%eax, %esi, 1), %dl
    popf
    adcb $0, %dl
    pushf

    movl size, %esi
    movb %dl, (%ecx, %esi, 1)
    incl i
    incl size
    jmp sum
sum:
    movl maxi, %edx
    cmpl %edx, i
    jz only_b
    movl maxj, %edx
    cmpl %edx, j
    jz only_a
    movl i, %esi
    movb (%eax, %esi, 1), %dh
    movl j, %esi
    movb (%ebx, %esi, 1), %dl

    popf
    adcb %dh, %dl
    pushf

    movl size, %esi
    movb %dl, (%ecx, %esi, 1)
    incl i
    incl j
    incl size
    jmp sum

predsum:
    clc
    pushf
    jmp sum
min1:
    decl maxi
    jmp zerodel1
min2:
    decl maxj
    jmp zerodel2
zerodel2:
    cmpl $0, maxj
    jz predsum
    decl maxj
    movl maxj, %edx
    incl maxj
    cmpb $0, (%ebx, %edx, 1)
    jz min2
    jmp predsum
zerodel1:
    cmpl $0, maxi
    jz zerodel2
    decl maxi
    movl maxi, %edx
    incl maxi
    cmpb $0, (%eax, %edx, 1)
    jz min1
    jmp zerodel2
supersum:
    movl %esi, maxi
    movl %edi, maxj
    push %esi
    jmp zerodel1

    .data
maxi: .int 0
maxj: .int 0
i: .int 0
j: .int 0
size: .int 0


// A = %eax
// B = %ebx
// C = %ecx
// sizeA = %esi
// sizeB = %edi

// write bytes in C
// put sizeC in %eax
