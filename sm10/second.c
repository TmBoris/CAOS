#include "math.h"
#include "stdint.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    union {
        float f;
        unsigned u;
    } un;

    int res;
    while ((res = scanf("%f", &un.f)) > 0) {
        unsigned sign = ((un.u & (1u << 31)) >> 31);
        unsigned exp = ((un.u & (((1 << 8) - 1) << 23)) >> 23);
        unsigned mantissa = (un.u & ((1 << 23) - 1));
        printf("%d %d %x\n", sign, exp, mantissa);
    }
    pow()
    return 0;
}