#include <stdbool.h>

enum {
    TWOFIVEFIVE = 255,
    THREEONE = 31,
    TWOTHREE = 23,
    EIGHT = 8,
};

bool nano(unsigned exp, unsigned mantissa) {
    return (exp == TWOFIVEFIVE) && (mantissa != 0);
}

bool isinfty(unsigned exp, unsigned mantissa) {
    return (exp == TWOFIVEFIVE) && (mantissa == 0);
}

bool isnormalo(unsigned exp) {
    return exp != 0;
}

FPClass fpclassf(float value, int *psign) {
    FPClass ans;

    union {
        float f;
        unsigned u;
    } un;

    un.f = value;
    unsigned sign = ((un.u & (1u << THREEONE)) >> THREEONE);
    unsigned exp = ((un.u & (((1 << EIGHT) - 1) << TWOTHREE)) >> TWOTHREE);
    unsigned mantissa = (un.u & ((1 << TWOTHREE) - 1));
    if (sign && !nano(exp, mantissa)) {
        *psign = 1;
    } else {
        *psign = 0;
    }
    if (nano(exp, mantissa)) {
        ans = FFP_NAN;
        return ans;
    }
    if (isinfty(exp, mantissa)) {
        ans = FFP_INF;
        return ans;
    }
    if (value == 0) {
        ans = FFP_ZERO;
        return ans;
    }
    if (isnormalo(exp)) {
        ans = FFP_NORMALIZED;
        return ans;
    } else {
        ans = FFP_DENORMALIZED;
        return ans;
    }
}
