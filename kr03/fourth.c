#include "stdint.h"
#include <float.h>

enum {
    MANT_MASK = 0b00000000011111111111111111111111,
    MAX_MANT_BIT_MASK = 0b00000000010000000000000000000000,
    SIGN_BIT_SHIFT = 31,
    EXP_SHIFT = 23,
    POWER = 3,
    EIGHT = 8,

};

void sat_mul8(float *f) {
    uint32_t *u = (uint32_t *)f;
    uint32_t sign = *u & (1 << SIGN_BIT_SHIFT);
    uint32_t exp = ((*u << 1) >> (EXP_SHIFT + 1));
    uint32_t mant = *u & MANT_MASK;

    if (exp == 0 && mant == 0) {
        return;
    }

    if (exp != ((1 << EIGHT) - 1)) {
        if (exp == 0) {
            int add = POWER;
            while (add > 0 && (mant & MAX_MANT_BIT_MASK) == 0) {
                mant <<= 1;
                add -= 1;
            }
            exp += add;
            if (add != 0) {
                mant <<= 1;
            }
            mant = mant & MANT_MASK;
            *u = sign + (exp << EXP_SHIFT) + mant;
            *f = *(float *)u;
        } else if (exp + POWER < ((1 << EIGHT) - 1)) {
            *u += (POWER << EXP_SHIFT);
            *f = *(float *)u;
        } else {
            if (sign == 0) {
                *f = FLT_MAX;
            } else {
                *f = -FLT_MAX;
            }
        }
    }
}

// int main() {
//     float f = 1.0f;
//     sat_mul8(&f);
//     if (f == 8.0f) {
//         printf("1");
//     }
// }
