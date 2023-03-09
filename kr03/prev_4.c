#include <inttypes.h>
#include "stdbool.h"
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

bool check(uint16_t num) { // это функция из нашей третей таски, только я 24 заменил на 11, т.к. мантисса тут меньше
    if (num == 0) {
        return true;
    }
    while (num % 2 == 0) {
        num /= 2;
    }
    return !(num >> (11));
}

uint16_t sat_mul4_half(uint16_t *f) {
//    sign_bits = 1;
//    exp_bits = 5;
//    mant_bits = 10;
    uint16_t sign = (*f >> 15); // оставили только самый левый бит - бит знака
    uint16_t mant = ((*f << 6) >> (6)); // убрали 6 левых единичек
    uint16_t exp =  ((*f << 1) >> 11); // обрезали сначала самый левый, а затем те, которые справа

    if (exp == ((1 << 6) - 1)) {
        return *f; // так как это означает, что перед нами inf или nan
    }
    bool res = check(*f);
    if (res) {
        return 4 * *f;
    }
    *f |= 0b0111110000000000;
    return *f;
}
