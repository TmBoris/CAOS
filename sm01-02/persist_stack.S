    .text
    .global main
quit:
    call finish
putnull:
    movl $-1, 4(%edx)
    jmp main
push:
    call readi32
    push %eax
    call readi32
    movl i, %edx
    movl %eax, (%ecx, %edx, 8) // write the value in the next cell of vec
    sall $3, %edx
    addl %ecx, %edx
    movl i, %edi
    movl %edx, (%esi, %edi, 4) // write the address of the cell in next array cell
    pop %eax
    cmpl $0, %eax
    jz putnull // if it is the solution.S element in Stack, put -1 in second part of vec cell
    movl (%esi, %eax, 4), %edi
    movl %edi, 4(%edx) // if not put an address of previous element

    jmp main
xxx:
    movl $-1, %eax
    call writei32
    subl $1, i
    jmp main
pop:
    call readi32
    movl (%esi, %eax, 4), %edi
    cmpl $-1, %edi // if Stack is empty, just write -1
    jz xxx
    movl (%edi), %eax // if not empty write the highest element
    call writei32
    movl i, %edx
    push %ecx
    movl 4(%edi), %ecx
    movl %ecx, (%esi, %edx, 4) // put the address of the previous element
    pop %ecx
    jmp main
main:
    movl $arr, %esi
    movl $vec, %ecx
    movl $-1, (%esi) // -1 meands that in that moment Stack is empty
    addl $1, i
    call readi32
    jc quit // leave the programm, if take the carry flag
    cmp $1, %eax
    jz pop
    jnz push



    .data         //   _     _     _
arr: .skip 4004   //  |_|   |_|   |_|  ...  solution.S array
vec: .skip 8008   //  _|_   _|_   _|_       pointers
i: .int 0         // |_|_| |_|_| |_|_| ...  second array
                  // tryed to drow the picture of two arrays