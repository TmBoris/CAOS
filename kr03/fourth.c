#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool check(uint16_t num) {
    if (num == 0) {
        return true;
    }
    while (num % 2 == 0) {
        num /= 2;
    }
    return !(num >> (24));
}

void sat_mul8(float *f) {
    uint32_t *u = (uint32_t *)f;
    uint32_t sign = *u & (1 << 31);
    uint32_t exp = ((*u << 1) >> 24);

    if (exp != ((1 << 8) - 1)) {
        bool res = check(*u);
        float tmp;
        if (res) {
            exp *= 8;
            *u &= 0b10000000011111111111111111111111;
            *u |= (exp << 23);

            uint32_t new_sign = *u & (1 << 31);
            uint32_t new_exp = ((*u << 1) >> 24);
            uint32_t new_mant = ..
            tmp = new_sign * 2 **
        }

        if (sign) {
            tmp = -340282346638528859811704183484516925440.f;
        } else {
            tmp = 340282346638528859811704183484516925440.f;
        }
        f = &tmp;
    }
}

int main() {
    float f = 1.0f;
    sat_mul8(&f);
    if (f == 8.0f) {
        printf("1");
    }
}
