#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int call_func(int eax, int ebx, int ecx, int edx) {
    int ans;
    __asm__("push %%ebp\n\t"
            "push %%edx\n\t"
            "push %%ecx\n\t"
            "push %%ebx\n\t"
            "push %%eax\n\t"
            "call func\n\t"
            "addl $16, %%esp\n\t"
            "pop %%ebp\n\t"
            : "=S"(ans)
            : "a"(eax), "b"(ebx), "c"(ecx), "d"(edx)
            :);
    return ans;
}