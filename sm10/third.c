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

enum { ISNOVAZDRAVSTVUITE = 24 };

bool check(uint32_t num) {
    if (num == 0) {
        return true;
    }
    while (num % 2 == 0) {
        num /= 2;
    }
    return !(num >> (ISNOVAZDRAVSTVUITE));
}

int main() {
    int res;
    uint32_t num;
    while ((res = scanf("%u", &num)) > 0) {
        printf("%d\n", check(num));
    }
}